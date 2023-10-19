# Deprecated

This library is now deprecated because IDF has changed the ADC calibration scheme and because, for a few years now, Arduino ESP32 has implemented calibration directly in its API if you call analogReadMilliVolts() instead of analogRead(). Therefore there's no good reason to update this library to the new scheme.

# AnalogESP
This library helps a little bit with the erratic behavior of ESP32 ADC using IDF provided calibration.
The calibration help with the non-linearity, but the ADC is still unable to measure values close to the min and the max range.

See https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc_calibration.html for more information.

This library can used the follow channels-pins for the adc:

    CHANNEL: ADC1_CHANNEL_1 - PIN: 37
    CHANNEL: ADC1_CHANNEL_2 - PIN: 38
    CHANNEL: ADC1_CHANNEL_3 - PIN: 39
    CHANNEL: ADC1_CHANNEL_4 - PIN: 32
    CHANNEL: ADC1_CHANNEL_5 - PIN: 33
    CHANNEL: ADC1_CHANNEL_6 - PIN: 34
    CHANNEL: ADC1_CHANNEL_7 - PIN: 35
