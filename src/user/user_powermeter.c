#include "user_powermeter.h"
#include "ina3221.h"
#include "ets_sys.h"

LOCAL powermtr_cfg_type* current_cfg; 
LOCAL powermtr_chndata_type chn_data[POWERMTR_CFG_CHANNEL_COUNT];

bool user_powermtr_init(powermtr_cfg_type* cfg)
{
    current_cfg = cfg;
}

void user_powermtr_main(void)
{
    /* ---- Determine battery voltage and current ---- */
    uint32 vbat_adc_sum = 0;
    sint32 ibat_mA_sum = 0;
    for(uint8 batt_chn=0;batt_chn<POWERMTR_CFG_BATTERY_CHANNEL_COUNT;batt_chn++)
    {
        ina3221_chn_data_type* data = ina3221_read(current_cfg->battery_channels[batt_chn].ina_channel);
        vbat_adc_sum += (data->busvoltage);
        /* Calculate voltage over shunt (40µV/bit). */
        sint32 shuntvoltage_microvolt = (sint32)(data->shuntvoltage)*40;
        /* Sum currents going in and out of the battery. */
        ibat_mA_sum += (shuntvoltage_microvolt*1000)/((sint32)(current_cfg->battery_channels[batt_chn].shuntvalue_microohm));
    }
    /* Divide sum of adc values by LSB value (8mV per bit) and channel count. */
    battery_data.voltage_mV = vbat_adc_sum / (8U * (uint32)POWERMTR_CFG_BATTERY_CHANNEL_COUNT);

    /* Set battery current. */
    battery_data.current_mA = ibat_mA_sum;


    /* ---- Determine auxiliary voltage and current ---- */
    for(uint8 aux_chn=0;aux_chn<POWERMTR_CFG_CHANNEL_COUNT;aux_chn++)
    {
        ina3221_chn_data_type* data = ina3221_read(current_cfg->channels[aux_chn].ina_channel);
        
        /* Calculate busvoltage. */
        chn_data[aux_chn].voltage_mV = (data->busvoltage)*8U;

        /* Calculate voltage over shunt (40µV/bit). */
        sint32 shuntvoltage_microvolt = (sint32)(data->shuntvoltage)*40;

        /* Calculate current. */
        aux_data[aux_chn].current_mA = (shuntvoltage_microvolt*1000)/((sint32)(current_cfg->channels[aux_chn].shuntvalue_microohm));
    }
}

powermtr_chndata_type* user_powermtr_read_channel(powermtr_channelref_type chn)
{
    return &(chn_data[chn]);
}