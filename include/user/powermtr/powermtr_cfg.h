#ifndef __POWERMTR_CFG_H__
#define __POWERMTR_CFG_H__
#include "c_types.h"
#include "ina3221_cfg.h"

/* ---- Static types ---- */
typedef enum
{
    POWERMTR_POSITIVE = 0,
    POWERMTR_NEGATIVE
}powermtr_polaritytype;

typedef struct
{
    powermtr_polaritytype polarity;
    ina3221_channelref_type ina_channel; 
    uint32 shuntvalue_microohm;    
}powermtr_channelcfg_type;

typedef uint8 powermtr_channelref_type;

/* ---- Configuration dependent types ---- */
#define POWERMTR_CFG_CHANNEL_COUNT 3

#define POWERMTR_CFG_CHN_SOLARALTERNATOR 0
#define POWERMTR_CFG_CHN_MAINSCHARGER    1
#define POWERMTR_CFG_CHN_LOADS           2

typedef struct
{
    powermtr_channelcfg_type channels[POWERMTR_CFG_CHANNEL_COUNT];
}powermtr_cfg_type;

/* Precompile configuration. */
extern const powermtr_cfg_type powermtr_cfg;

#endif /*__USER_POWERMETER_CFG_H__ */