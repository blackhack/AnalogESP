/*
    AnalogESP - This library helps a little bit with the erratic behavior of ESP32 ADC using IDF provided calibration.
    Copyright(C) 2021 Blackhack <davidaristi.0504@gmail.com>

    This library is free software; you can redistribute itand /or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110 - 1301
    USA
 */

#include "AnalogESP.h"

AnalogESP::AnalogESP(uint8_t pin, uint32_t vref /*= 0*/)
{
    _pin = pin;
    _defaultVref = vref;
    _adc_characteristics = new esp_adc_cal_characteristics_t;
}

bool AnalogESP::IsVrefNeeded()
{
    bool eFuse_vref = (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK);
    bool eFuse_tp = (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK);

    if (!eFuse_vref && !eFuse_tp)
    {
        #if DEBUG_PRINTS
            Serial.println("AnalogESP> eFuse values are not available, Vref is needed!");
        #endif

        return true;
    }

    #if DEBUG_PRINTS
        Serial.printf("AnalogESP> eFuse values are available, Vref is not needed! - eFuse_vref %d, eFuse_tp %d\n", uint32_t(eFuse_vref), uint32_t(eFuse_tp));
    #endif

    return false;
}

bool AnalogESP::begin(adc_bits_width_t resolution /*= ADC_WIDTH_BIT_12*/, adc_atten_t attenuation /*= ADC_ATTEN_DB_11*/)
{
    _analogChannel = PinToADC_Channel(_pin, &_ready);

    if (!_ready)
    {
        #if DEBUG_PRINTS
            Serial.printf("AnalogESP> Error, pin %d is not an adc channel\n", _pin); 
        #endif

        return false;
    }

    adc1_config_width(resolution);
    adc1_config_channel_atten(_analogChannel, attenuation);
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, attenuation, resolution, _defaultVref, _adc_characteristics);

    #if DEBUG_PRINTS
        Serial.printf("AnalogESP> Adc channel %d in pin %d, successfully initialized\n", _analogChannel, _pin);
        Serial.print("AnalogESP> Calibration type: ");
        Serial.println(val_type); 
    #endif

    return true;
}

uint32_t AnalogESP::mVoltageRead(uint16_t samples /*= 64*/)
{
    if (!_ready)
        return 0;

    uint32_t sum = 0;
    for (int x = 0; x < samples; x++)
        sum += adc1_get_raw(_analogChannel);
    sum /= samples;

    return esp_adc_cal_raw_to_voltage(sum, _adc_characteristics);
}

double AnalogESP::VoltageRead(uint16_t samples /*= 64*/)
{
    return mVoltageRead(samples) / 1000.0;
}

uint32_t AnalogESP::AnalogRead(uint16_t samples /*= 64*/)
{
    if (!_ready)
        return 0;

    uint32_t sum = 0;
    for (int x = 0; x < samples; x++)
        sum += adc1_get_raw(_analogChannel);
    sum /= samples;

    return sum;
}

adc1_channel_t AnalogESP::PinToADC_Channel(uint8_t pin, bool* ok /*= nullptr*/)
{
    adc1_channel_t chan;
    if (ok)
        *ok = true;

    switch (pin)
    {
        case 32: chan = ADC1_CHANNEL_4; break;
        case 33: chan = ADC1_CHANNEL_5; break;
        case 34: chan = ADC1_CHANNEL_6; break;
        case 35: chan = ADC1_CHANNEL_7; break;
        case 37: chan = ADC1_CHANNEL_1; break;
        case 38: chan = ADC1_CHANNEL_2; break;
        case 39: chan = ADC1_CHANNEL_3; break;
        default:
        {
            if (ok)
                *ok = false;
            chan = ADC1_CHANNEL_MAX;
            break;
        }
    }

    return chan;
}
