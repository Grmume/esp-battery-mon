#ifndef __POWERMTR_H__
#define __POWERMTR_H__
#include "powermtr_cfg.h"

typedef struct
{
    sint32 current_mA;
    uint16 voltage_mV;
}powermtr_chndata_type;

bool powermtr_init(powermtr_cfg_type* cfg);
void powermtr_main(void);
powermtr_chndata_type* powermtr_read_channel(powermtr_channelref_type chn);


#endif /* __POWERMTR_H__*/