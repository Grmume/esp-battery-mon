#include "user_powermeter_cfg.h"

const powermtr_cfg_type powermtr_cfg = 
{
    .channels = 
    {
        {
            /* POWERMTR_CFG_CHN_SOLARALTERNATOR */
            .polarity = POWERMTR_POSITIVE,
            .ina_channel = INA3221_CFG_CHN_SOLARALTERNATOR,
            .shuntvalue_microohm = 1500 /* 50A, 75mV Shunt. */ 
        },
        {
            /* POWERMTR_CFG_CHN_MAINSCHARGER */
            .polarity = POWERMTR_POSITIVE,
            .ina_channel = INA3221_CFG_CHN_MAINSCHARGER,
            .shuntvalue_microohm = 3750 /* 20A, 75mV Shunt. */ 
        },
        {
            /* POWERMTR_CFG_CHN_LOADS */
            .polarity = POWERMTR_POSITIVE,
            .ina_channel = INA3221_CFG_CHN_LOADS,
            .shuntvalue_microohm = 2500 /* 30A, 75mV Shunt. */ 
        }
    }
};