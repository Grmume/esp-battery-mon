#ifndef __FGAUGE_CFG_H__
#define __FGAUGE_CFG_H__

#include "c_types.h"
#include "powermtr_cfg.h"

typedef enum
{
    FGAUGE_LEADACID,
    FGAUGE_LIFEPO
}fgauge_battery_type;

typedef struct
{
    fgauge_battery_type battery_type;
    uint16 nominal_capacity_Ah;
}fgauge_battery_cfg_type;

typedef struct
{
    uint16 voltage_thd_mV;
    uint16 current_thd_mA;
}fgauge_endofcharge_cfg_type;

typedef enum
{
    FGAUGE_CHARGE,
    FGAUGE_DISCHARGE,
    FGAUGE_BIDIRECTIONAL
}fgauge_channel_type;


typedef struct
{
    powermtr_channelref_type powermtr_channel;
    fgauge_channel_type type;
    fgauge_endofcharge_cfg_type endofcharge_cfg;
}fgauge_current_channel_type;


/* Precompile configuration. */
#define FGAUGE_CFG_HISTORY_SIZE  2
#define FGAUGE_CFG_CALLCYCLE_MS  100

#define FGAUGE_CFG_CURRENT_CHANNEL_COUNT     3
typedef struct
{
    fgauge_battery_cfg_type bat;
    uint16 ocv_estim_holdoff_s;
    uint16 ocv_detect_holdoff_ms;
    uint8 ocv_delta_thd_mV;
    uint8 ocv_current_thd_mA;
    fgauge_current_channel_type current_channels[FGAUGE_CFG_CURRENT_CHANNEL_COUNT];
}fgauge_cfg_type;


extern const fgauge_cfg_type fgauge_cfg;


#endif /*__FGAUGE_CFG_H__ */