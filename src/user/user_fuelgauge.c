#include "user_fuelgauge.h"
#include "user_powermeter.h"
#include "ets_sys.h"
typedef struct
{
    uint16 voltage_mV[FUELGG_CFG_HISTORY_SIZE];
    sint32 current_mA[FUELGG_CFG_HISTORY_SIZE];
    uint16 current_idx;
    uint16 value_count;
}history_type;

typedef struct
{
    history_type history;
    uint64 coulomb_cnt;           /* Depleted charge since last end of charge (fully charged) in micro coulomb. */
    uint8 soc;                    /* 0..255 */
    uint8 soh;                    /* 0..255 */
}livedata_type;

LOCAL fuelgg_cfg_type* current_cfg;

LOCAL livedata_type livedata;
LOCAL uint16 relaxing_cntr;

LOCAL void add_to_history(uint16 voltage_mV, sint32 current_mA);
LOCAL bool get_from_history(uint16 index, uint16* voltage_mV, sint32* current_mA);
LOCAL void detect_endofcharge(void);

bool user_fuelgg_init(fuelgg_cfg_type* cfg)
{
    current_cfg = cfg;

    /* TODO Clear history. */
}

void user_fuelgg_main(void)
{
    powermtr_chndata_type* data = user_powermtr_read_batterydata();
    uint16 last_u;
    sint32 last_i;
    if(get_from_history(0, &last_u, &last_i))
    {

        /* Check if current is below threshold. */
        if((data->current_mA < current_cfg->ocv_current_thd_mA) &&
           (data->current_mA > -(current_cfg->ocv_current_thd_mA)))
        {
            const uint16 cntr_detect_thd = (current_cfg->ocv_detect_holdoff_ms) / FUELGG_CFG_CALLCYCLE_MS;
            
            if(relaxing_cntr > cntr_detect_thd)
            {
                /* Check whether the voltage is stable. */
                uint16 voltage_delta = (last_u > (data->voltage_mV)) ? last_u-(data->voltage_mV) : (data->voltage_mV)-last_u;
                if(voltage_delta < current_cfg->ocv_delta_thd_mV)
                {
                    /* Battery is relaxed. */
                    /* Use OCV curve to determine the state of charge. */
                    /* Correct the coulomb counter. */
                }
            }
            else
            {
                /* Keep counting. */
                relaxing_cntr++;
            }
        }
        else
        {
            relaxing_cntr = 0;
        }

        /* Coulomb counting. */
        livedata.coulomb_cnt += (data->current_mA) * FUELGG_CFG_CALLCYCLE_MS;
    }
    else
    {
        /* No previous data available. Assume battery was active. */
        relaxing_cntr = 0;
    }

    /* Calculate SOC */
    uint64 current_capacity = livedata.soh * current_cfg->bat.nominal_capacity_Ah * 60U * 60U * 1000U * 1000U;
    livedata.soc = ((current_capacity - livedata.coulomb_cnt)*255U) / current_capacity;

    add_to_history(data->voltage_mV, data->current_mA);
}

LOCAL void add_to_history(uint16 voltage_mV, sint32 current_mA)
{
    livedata.history.voltage_mV[livedata.history.current_idx] = voltage_mV;
    livedata.history.current_mA[livedata.history.current_idx] = current_mA;

    livedata.history.current_idx = (livedata.history.current_idx + 1) % FUELGG_CFG_HISTORY_SIZE;
    livedata.history.value_count++;
}

LOCAL bool get_from_history(uint16 index, uint16* voltage_mV, sint32* current_mA)
{
    if(index > livedata.history.value_count - 1)
    {
        /* Not that much data has been recorded in the history. */
        return false;
    }
    else
    {
        uint8 last_index = (livedata.history.current_idx - 1) % FUELGG_CFG_HISTORY_SIZE;
        *voltage_mV = livedata.history.voltage_mV[last_index];
        *current_mA = livedata.history.current_mA[last_index];
        return true;
    }
}

LOCAL void detect_endofcharge()
{
    for(uint8 idx=0;idx<FUELGG_CFG_CURRENT_CHANNEL_COUNT;idx++)
    {
        fuelgg_current_channel_type* chn = &(current_cfg->current_channels[idx]);
        if(chn->type == FUELGG_CHARGING || chn->type == FUELGG_BIDIRECTIONAL)
        {
            powermtr_chndata_type* data = user_powermtr_read_channel(chn->powermtr_channel);
            /* Check if the measured voltage is higher than the end of charge detection threshold. */
            if(data->voltage_mV > chn->endofcharge_cfg.voltage_thd_mV)
            {
                if(data->current_mA < chn->endofcharge_cfg.current_thd_mA)
                {
                    
                }
            }
        }
    } 
}