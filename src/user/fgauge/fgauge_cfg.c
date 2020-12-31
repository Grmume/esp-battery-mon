#include "fgauge_cfg.h"

const fgauge_cfg_type fgauge_cfg = 
{
    .bat = 
    {
        .battery_type = FGAUGE_LEADACID,
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
            .type = FGAUGE_CHARGE,
            .endofcharge_cfg = 
            {
                .voltage_thd_mV = 13500,
                .current_thd_mA = 400
            }
        },
        {
            .powermtr_channel = POWERMTR_CFG_CHN_MAINSCHARGER,
            .type = FGAUGE_CHARGE,
            .endofcharge_cfg = 
            {
                .voltage_thd_mV = 13500,
                .current_thd_mA = 300
            }
        },
        {
            .powermtr_channel = POWERMTR_CFG_CHN_LOADS,
            .type = FGAUGE_DISCHARGE,
            .endofcharge_cfg = 
            {
                .voltage_thd_mV = 0, /* Unused. */
                .current_thd_mA = 0  /* Unused. */
            }
        }
    }
};
