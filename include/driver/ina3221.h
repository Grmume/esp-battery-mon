#ifndef __INA3221_H__
#define __INA3221_H__

#include "user_config.h"
#include "ina3221_cfg.h"

typedef struct
{
    uint16 busvoltage;      /* ADC Value. LSB: 8mV */
    sint16 shuntvoltage;    /* ADC Value. LSB: 40µV */
}ina3221_chn_data_type;

typedef struct
{
    sint8 chn_idx[3];
}ina3221_ctrllr_info_type;

ina3221_chn_data_type* ina3221_read(uint8 chn_idx);
bool ina3221_init(ina3221_cfg_type* cfg);
void ina3221_main(void);

#endif /*__INA3221_H__ */