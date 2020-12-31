#include "powermtr.h"
#include "ina3221.h"
#include "ets_sys.h"

LOCAL powermtr_cfg_type* current_cfg; 
LOCAL powermtr_chndata_type chn_data[POWERMTR_CFG_CHANNEL_COUNT];

bool powermtr_init(powermtr_cfg_type* cfg)
{
    current_cfg = cfg;
}

void powermtr_main(void)
{
    /* ---- Determine voltage and current for each channel ---- */
    for(uint8 aux_chn=0;aux_chn<POWERMTR_CFG_CHANNEL_COUNT;aux_chn++)
    {
        ina3221_chn_data_type* data = ina3221_read(current_cfg->channels[aux_chn].ina_channel);
        
        /* Calculate busvoltage. */
        chn_data[aux_chn].voltage_mV = (data->busvoltage)*8U;

        /* Calculate voltage over shunt (40µV/bit). */
        sint32 shuntvoltage_microvolt = (sint32)(data->shuntvoltage)*40;

        /* Calculate current. */
        chn_data[aux_chn].current_mA = (shuntvoltage_microvolt*1000)/((sint32)(current_cfg->channels[aux_chn].shuntvalue_microohm));
    }
}

powermtr_chndata_type* powermtr_read_channel(powermtr_channelref_type chn)
{
    return &(chn_data[chn]);
}