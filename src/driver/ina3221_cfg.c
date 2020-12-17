#include "ina3221_cfg.h"

const ina3221_cfg_type ina3221_cfg = 
{
    /* Controllers. */
    .controllers = 
    {
        {
            .address = 0x00,
            .avgmode = INA3221_AVG_1024,
            .bus_convtime = INA3221_CONVT_8244US,
            .shunt_convtime = INA3221_CONVT_8244US
        }
    },
    
    /* Channels. */
    .channels = 
    {
        /* INA3221_CFG_CHN_SOLARALTERNATOR */
        {
            .ctrllr_idx = 0,
            .ctrllr_chn = 0
        },
        /* INA3221_CFG_CHN_MAINSCHARGER */
        {
            .ctrllr_idx = 0,
            .ctrllr_chn = 1
        },
        /* INA3221_CFG_CHN_LOADS */
        {
            .ctrllr_idx = 0,
            .ctrllr_chn = 2
        }
    }
}; 