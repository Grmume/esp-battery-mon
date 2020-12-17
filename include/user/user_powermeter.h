#ifndef __USER_POWERMETER_H__
#define __USER_POWERMETER_H__
#include "user_powermeter_cfg.h"

typedef struct
{
    sint32 current_mA;
    uint16 voltage_mV;
}powermtr_chndata_type;

bool user_powermtr_init(powermtr_cfg_type* cfg);
void user_powermtr_main(void);
powermtr_chndata_type* user_powermtr_read_channel(powermtr_channelref_type chn);


#endif /* __USER_POWERMETER_H__*/