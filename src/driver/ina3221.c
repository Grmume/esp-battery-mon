#include "ina3221.h"
#include "ets_sys.h"
#include "driver/i2c_master.h"


/* Register addresses. */
#define REG_CFG_ADDR            0x00

#define REG_CHN1_SHNT_ADDR      0x01
#define REG_CHN1_BUS_ADDR       0x02
#define REG_CHN2_SHNT_ADDR      0x03
#define REG_CHN2_BUS_ADDR       0x04
#define REG_CHN3_SHNT_ADDR      0x05
#define REG_CHN3_BUS_ADDR       0x06

#define REG_CHN1_CRIT_ADDR      0x07
#define REG_CHN1_WARN_ADDR      0x08
#define REG_CHN2_CRIT_ADDR      0x09
#define REG_CHN2_WARN_ADDR      0x0A
#define REG_CHN3_CRIT_ADDR      0x0B
#define REG_CHN3_WARN_ADDR      0x0C

#define REG_SHNT_SUM_ADDR       0x0D
#define REG_SHNT_SUM_LIM_ADDR   0x0E
#define REG_MASK_EN_ADDR        0x0F

#define REG_PWR_VLD_UPPR_ADDR   0x10
#define REG_PWR_VLD_LOWR_ADDR   0x11

LOCAL ina3221_cfg_type current_cfg;
LOCAL ina3221_ctrllr_info_type ctrllr_info[INA3221_CFG_CONTROLLER_COUNT];
LOCAL ina3221_chn_data_type chn_data[INA3221_CFG_CHANNEL_COUNT];

LOCAL ICACHE_FLASH_ATTR bool read_register(uint8 ctrllr_idx, uint8 register_addr, uint16* data);

bool ICACHE_FLASH_ATTR ina3221_init(ina3221_cfg_type* cfg)
{
    /* Save the current configuration. */
    current_cfg = *cfg;

    /* Write configuration register. */
    for(uint8 ctrllr_idx=0;ctrllr_idx < INA3221_CFG_CONTROLLER_COUNT;ctrllr_idx++)
    {
        const ina3221_ctrllr_cfg_type* ctrllr = &current_cfg.controllers[ctrllr_idx];
        
        /* Determine the used channels of this controller. */
        ctrllr_info[ctrllr_idx].chn_idx[0] = -1;
        ctrllr_info[ctrllr_idx].chn_idx[1] = -1;
        ctrllr_info[ctrllr_idx].chn_idx[2] = -1;

        /* Iterate over channels. */
        for(uint8 chn_idx=0;chn_idx < INA3221_CFG_CHANNEL_COUNT;chn_idx++)
        {
            if(current_cfg.channels[chn_idx].ctrllr_idx == ctrllr_idx)
            {
                ctrllr_info[ctrllr_idx].chn_idx[current_cfg.channels[chn_idx].ctrllr_chn] = chn_idx;
            }
        }

        i2c_master_gpio_init();

        i2c_master_start();
        i2c_master_writeByte((uint8)((ctrllr->address) << 1U));
        if(!i2c_master_checkAck())
        {
            os_printf("no ack on tx addr\n");
            i2c_master_stop();
            return false;
        }

        /* Write configuration register. */
        uint16 cfg_reg = INA3221_OPERMODE_SHNTBUS_CONT;
        cfg_reg |= (((uint32)ctrllr->shunt_convtime) << 3U);
        cfg_reg |= (((uint32)ctrllr->bus_convtime) << 6U);
        cfg_reg |= (((uint32)ctrllr->avgmode) << 9U);

        /* Determine channel enable register value. */
        uint8 chn_en = (ctrllr_info[ctrllr_idx].chn_idx[2] >= 0) |
                       ((ctrllr_info[ctrllr_idx].chn_idx[1] >= 0) << 1U) |
                       ((ctrllr_info[ctrllr_idx].chn_idx[0] >= 0) << 2U);

        cfg_reg |= (((uint32)chn_en) << 12U);    

        i2c_master_writeByte((uint8)cfg_reg);
        if (!i2c_master_checkAck())
        {
            os_printf("no ack on tx config reg byte0\n");
            i2c_master_stop();
            return false;
        }

        i2c_master_writeByte((uint8)(cfg_reg << 8U));
        if (!i2c_master_checkAck())
        {
            os_printf("no ack on tx config reg byte1\n");
            i2c_master_stop();
            return false;
        }

        i2c_master_stop();
    }

    return true;
}

void ICACHE_FLASH_ATTR ina3221_main(void)
{
    /* Read newest measurements from each controller. */
    for(uint8 ctrllr_idx=0;ctrllr_idx < INA3221_CFG_CONTROLLER_COUNT;ctrllr_idx++)
    {
        const ina3221_ctrllr_cfg_type* ctrllr_cfg = &current_cfg.controllers[ctrllr_idx];
        const ina3221_ctrllr_info_type* ctrllr_info = &ctrllr_info[ctrllr_idx];

        /* --- Read Channel 1 (if used) --- */
        if(ctrllr_info->chn_idx[0] >= 0)
        {
            if(!read_register(ctrllr_idx, REG_CHN1_SHNT_ADDR, &(chn_data[ctrllr_info->chn_idx[0]].shuntvoltage)))
            {
                os_printf("error reading chn1 shunt\n");
            }
            if(!read_register(ctrllr_idx, REG_CHN1_BUS_ADDR, &(chn_data[ctrllr_info->chn_idx[0]].busvoltage)))
            {
                os_printf("error reading chn1 bus\n");
            }
        }

        /* --- Read Channel 2 (if used) --- */
        if(ctrllr_info->chn_idx[1] >= 0)
        {
            if(!read_register(ctrllr_idx, REG_CHN2_SHNT_ADDR, &(chn_data[ctrllr_info->chn_idx[1]].shuntvoltage)))
            {
                os_printf("error reading chn2 shunt\n");
            }
            if(!read_register(ctrllr_idx, REG_CHN2_BUS_ADDR, &(chn_data[ctrllr_info->chn_idx[1]].busvoltage)))
            {
                os_printf("error reading chn2 bus\n");
            }
        }

        /* --- Read Channel 3 (if used) --- */
        if(ctrllr_info->chn_idx[2] >= 0)
        {
            if(!read_register(ctrllr_idx, REG_CHN3_SHNT_ADDR, &(chn_data[ctrllr_info->chn_idx[2]].shuntvoltage)))
            {
                os_printf("error reading chn3 shunt\n");
            }
            if(!read_register(ctrllr_idx, REG_CHN3_BUS_ADDR, &(chn_data[ctrllr_info->chn_idx[2]].busvoltage)))
            {
                os_printf("error reading chn3 bus\n");
            }
        }  
    }
}

ina3221_chn_data_type* ICACHE_FLASH_ATTR user_ina3221_getdata(uint8 chn_idx)
{
    if(chn_idx < INA3221_CFG_CHANNEL_COUNT)
    {
        return &(chn_data[chn_idx]);
    }   
    else
    {
        /* Invalid channel. */
    }
}

LOCAL ICACHE_FLASH_ATTR bool read_register(uint8 ctrllr_idx, uint8 register_addr, uint16* data)
{
    const ina3221_ctrllr_cfg_type* ctrllr_cfg = &current_cfg.controllers[ctrllr_idx];
    const ina3221_ctrllr_info_type* ctrllr_info = &ctrllr_info[ctrllr_idx];

    /* --- Set register pointer --- */
    i2c_master_start();

    /* Write slave address and indicate a write operation. */
    i2c_master_writeByte((uint8)((ctrllr_cfg->address) << 1U));
    if(!i2c_master_checkAck())
    {
        os_printf("no ack on tx addr\n");
        i2c_master_stop();
        return false;
    }

    /* Write register address. */
    i2c_master_writeByte(register_addr);
    if (!i2c_master_checkAck())
    {
        os_printf("no ack on tx reg address\n");
        i2c_master_stop();
        return false;
    }

    i2c_master_stop();


    /* --- Read register value (2bytes) --- */
    i2c_master_start();

    uint16 reg_val = 0U;

    /* Write slave address and indicate a read operation. */
    i2c_master_writeByte((uint8)((((ctrllr_cfg->address) << 1U) | 1U)));
    if(!i2c_master_checkAck())
    {
        os_printf("no ack on tx addr\n");
        i2c_master_stop();
        return false;
    }

    /* Read MSB. */
    reg_val = ((uint16)i2c_master_readByte()) << 8U;

    i2c_master_send_ack();

    /* Read LSB. */
    reg_val |= (uint16)i2c_master_readByte();

    /* Finalize transmission with a nack. */
    i2c_master_send_nack();

    i2c_master_stop();

    /* Write value to output (in atomic access). */
    *data = reg_val;

    return true;
}
