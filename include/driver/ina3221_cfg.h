
#ifndef __INA3221_CFG_H__
#define __INA3221_CFG_H__
#include "c_types.h"

/* ---- Static types ---- */
typedef enum
{
    INA3221_AVG_1 =     0,
    INA3221_AVG_4 =     1,
    INA3221_AVG_16 =    2,
    INA3221_AVG_64 =    3,
    INA3221_AVG_128 =   4,
    INA3221_AVG_256 =   5,
    INA3221_AVG_512 =   6,
    INA3221_AVG_1024 =  7
}ina3221_avgmode_type;

typedef enum
{
    INA3221_CONVT_140US =     0,
    INA3221_CONVT_204US =     1,
    INA3221_CONVT_332US =     2,
    INA3221_CONVT_588US =     3,
    INA3221_CONVT_1100US =    4,
    INA3221_CONVT_2116US =    5,
    INA3221_CONVT_4156US =    6,
    INA3221_CONVT_8244US =    7
}ina3221_convtime_type;

typedef enum
{
    INA3221_OPERMODE_POWERDOWN =    0,
    INA3221_OPERMODE_SHNT_SNG =     1,
    INA3221_OPERMODE_BUS_SNG =      2,
    INA3221_OPERMODE_SHNTBUS_SNG =  3,
    INA3221_OPERMODE_POWERDOWN2 =   4,
    INA3221_OPERMODE_SHNT_CONT =    5,
    INA3221_OPERMODE_BUS_CONT =     6,
    INA3221_OPERMODE_SHNTBUS_CONT = 7 /* This driver always operations in the continuous mode. */
}ina3221_opermode_type;

typedef struct
{
    uint16 address;
    ina3221_avgmode_type avgmode;
    ina3221_convtime_type bus_convtime;
    ina3221_convtime_type shunt_convtime;
}ina3221_ctrllr_cfg_type;

typedef struct
{
    uint8 ctrllr_idx;
    uint8 ctrllr_chn;
}ina3221_chn_cfg_type;

typedef uint8 ina3221_channelref_type;


/* ---- Configuration dependent types ---- */
#define INA3221_CFG_CONTROLLER_COUNT    1

#define INA3221_CFG_CHANNEL_COUNT       3

#define INA3221_CFG_CHN_SOLARALTERNATOR 0
#define INA3221_CFG_CHN_MAINSCHARGER    1
#define INA3221_CFG_CHN_LOADS           2

typedef struct
{
    ina3221_ctrllr_cfg_type controllers[INA3221_CFG_CONTROLLER_COUNT];
    ina3221_chn_cfg_type channels[INA3221_CFG_CHANNEL_COUNT];
}ina3221_cfg_type;

/* Precompile configuration. */
extern const ina3221_cfg_type ina3221_cfg;

#endif /* __INA3221_CFG_H__ */