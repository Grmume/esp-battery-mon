#ifndef __USER_FUELGAUGE_CFG_H__
#define __USER_FUELGAUGE_CFG_H__

#include "c_types.h"
#include "user_powermeter_cfg.h"

typedef enum
{
    FUELGG_LEADACID,
    FUELGG_LIFEPO
}fuelgg_battery_type;

typedef struct
{
    fuelgg_battery_type battery_type;
    uint16 nominal_capacity_Ah;
}fuelgg_battery_cfg_type;

typedef struct
{
    uint16 voltage_thd_mV;
    uint16 current_thd_mA;
}fuelgg_endofcharge_cfg_type;

typedef struct
{
    powermtr_channelref_type powermtr_channel;
    fuelgg_endofcharge_cfg_type endofcharge_cfg;
}fuelgg_charger_channel_type;


/* Precompile configuration. */
#define FUELGG_CFG_HISTORY_SIZE  2
#define FUELGG_CFG_CALLCYCLE_MS  100

#define FUELGG_CFG_CHG_CHANNEL_COUNT     2
#define FUELGG_CFG_DISCHG_CHANNEL_COUNT  1
typedef struct
{
    fuelgg_battery_cfg_type bat;
    uint16 ocv_estim_holdoff_s;
    uint16 ocv_detect_holdoff_ms;
    uint8 ocv_delta_thd_mV;
    uint8 ocv_current_thd_mA;
    fuelgg_charger_channel_type chg_channels[FUELGG_CFG_CHG_CHANNEL_COUNT];
    powermtr_channelref_type dischg_channels[FUELGG_CFG_DISCHG_CHANNEL_COUNT];
}fuelgg_cfg_type;


extern const fuelgg_cfg_type fuelgg_cfg;


#endif /*__USER_FUELGAUGE_CFG_H__ */