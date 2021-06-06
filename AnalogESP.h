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

#ifndef _ANALOGESP_h
#define _ANALOGESP_h

#include "arduino.h"
#include "esp_adc_cal.h"

// Set true to see some debug information
// Disable to save some flash memory
#define DEBUG_PRINTS false

class AnalogESP
{
public:
    AnalogESP(uint8_t pin, uint32_t vref = 0);

    ~AnalogESP()
    {
        delete _adc_characteristics;
    }

    /**
     * @brief Checks if ADC calibration values are burned into eFuse
     *
     * This function checks if ADC reference voltage or Two Point values have been
     * burned to the eFuse of the current ESP32
     *
     * @return
     *      - true: Not eFuses values present, Vref is needed
     *      - false: There is eFuse values, Vref is no needed and will be ignored
     */
    bool IsVrefNeeded();

    /**
     * @brief Initialize adc in the specified pin
     *
     * This function check and initialize the adc calibration in the channel corresponding to
     * the pin specified during object initialization
     *
     * @note Refer to https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/peripherals/adc.html
     *       for info about resolution and attenuation parameters
     *
     * @param[in]   resolution  Set the resolution of the adc, see adc_bits_width_t struc in adc.h
     * @param[in]   atenuation  Set the attenuation of the adc, see adc_atten_t struc in adc.h
     *
     * @return
     *      - true: Adc calibration initialized successful
     *      - false: The specified pin is not an adc1 channel
     */
    bool begin(adc_bits_width_t resolution = ADC_WIDTH_BIT_12, adc_atten_t attenuation = ADC_ATTEN_DB_11);

    /**
     * @brief Read voltage (mV) based on adc calibration
     *
     * This function reads the adc channel and return a voltage (mV) value
     * based in the adc calibration aplied in the begin function
     *
     * @note Refer to https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/peripherals/adc.html
     *       for info about adc calibration
     *
     * @param[in]   samples  Amount of Multisampling, in the documentation a value of 64 is used,
     *                       should be greater than 0
     *
     * @return      Voltage in mV
     */
    uint32_t mVoltageRead(uint16_t samples = 64);

    /**
     * @brief Read voltage (V) based on adc calibration
     *
     * This function reads the adc channel and return a voltage (V) value
     * based in the adc calibration aplied in the begin function
     *
     * @note Refer to https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/peripherals/adc.html
     *       for info about adc calibration
     *
     * @param[in]   samples  Amount of Multisampling, in the documentation a value of 64 is used,
     *                       should be greater than 0
     *
     * @return      Voltage in V
     */
    double VoltageRead(uint16_t samples = 64);

     /**
     * @brief Read the adc channel
     *
     * This function reads the adc channel and return a raw value
     * based on the resolution and attenuation
     *
     * @note Calibration does not affect the returned value
     *
     * @param[in]   samples  Amount of Multisampling, in the documentation a value of 64 is used,
     *                       should be greater than 0
     *
     * @return      Adc raw value
     */
    uint32_t AnalogRead(uint16_t samples = 64);

     /**
     * @brief Convert GPIO pin to adc1 channel
     *
     * This function finds the correspondent adc1 channel for
     * the specified pin
     *
     * @note This function is used internally, but can be used is the user wants
     *
     * @param[in]   pin    Adc1 pin
     * @param[out]  ok     Optional pointer to a boolean variable, indicates if
     *                     the pin is a valid adc1 channel
     *
     * @return      ADC1 channel number
     */
    adc1_channel_t PinToADC_Channel(uint8_t pin, bool* ok = nullptr);

private:
    bool _ready;
    uint32_t _defaultVref;
    uint8_t _pin;
    adc1_channel_t _analogChannel;
    esp_adc_cal_characteristics_t* _adc_characteristics;
};

#endif
