#ifndef __FGAUGE_CHARGECYCLE_H__
#define __FGAUGE_CHARGECYCLE_H__

#include "c_types.h"
#include "fgauge_cfg.h"

typedef enum
{
    FGAUGE_DISCHARGING = 0,
    FGAUGE_CHARGING,
    FGAUGE_FULLYCHARGED,
}fgauge_chargestate_type;

void fgauge_chargecycle_init(fgauge_cfg_type* cfg);
fgauge_chargestate_type fgauge_chargecycle_main(void);

#endif /* __FGAUGE_CHARGECYCLE_H__ */