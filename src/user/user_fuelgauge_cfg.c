#include "user_fuelgauge_cfg.h"

const fuelgg_cfg_type fuelgg_cfg = 
{
    .bat = 
    {
        .battery_type = FUELGG_LEADACID,
        .nominal_capacity_Ah = 60
    },
    .ocv_delta_thd_mV = 50,
    .ocv_current_thd_mA = 200,
    .ocv_detect_holdoff_ms = 1000,
    .ocv_estim_holdoff_s = 240,
    .current_channels = 
    {
        {
            .powermtr_channel = POWERMTR_CFG_CHN_SOLARALTERNATOR,
            .type = FUELGG_CHARGING,
            .endofcharge_cfg = 
            {
                .voltage_thd_mV = 13500,
                .current_thd_mA = 400
            }
        },
        {
            .powermtr_channel = POWERMTR_CFG_CHN_MAINSCHARGER,
            .type = FUELGG_CHARGING,
            .endofcharge_cfg = 
            {
                .voltage_thd_mV = 13500,
                .current_thd_mA = 300
            }
        },
        {
            .powermtr_channel = POWERMTR_CFG_CHN_LOADS,
            .type = FUELGG_DISCHARGING,
            .endofcharge_cfg = 
            {
                .voltage_thd_mV = 0, /* Unused. */
                .current_thd_mA = 0  /* Unused. */
            }
        }
    }
};