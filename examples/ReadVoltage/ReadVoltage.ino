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

AnalogESP analog(32); // Pin 32

void setup()
{
    Serial.begin(115200);
    analog.begin();
}

void loop()
{
    double voltage = analog.VoltageRead();
    Serial.println(voltage);

    delay(1000);
}
