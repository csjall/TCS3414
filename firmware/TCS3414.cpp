//
// Copyright (C) 2015 Johann Scholtz
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY,  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL DALLAS SEMICONDUCTOR BE LIABLE FOR ANY CLAIM, DAMAGES
// OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
#include "TCS3414.h"

boolean TCS3414::begin()
{
    Wire.setSpeed(CLOCK_SPEED_400KHZ);
    Wire.begin();

    AutoEnable autoEnable(this);
    getInformation(&_partNumber, &_revisionNumber);
    if (_partNumber != 1)
    {
        return false;
    }

    setGain(TCS3414_GAIN_1X, TCS3414_PRESCALARMODE_DIVIDE_BY_1);
    setIntegrationTime(TCS3414_INTEGRATIONTIME_12MS);

    return true;
}

uint8_t TCS3414::getPartNumber() const
{
    return _partNumber;
}

uint8_t TCS3414::getRevisionNumber() const
{
    return _revisionNumber;
}

void TCS3414::setIntegrationTime(uint8_t integrationTime)
{
    AutoEnable autoEnable(this);
    write8(TCS3414_REGISTER_TIMING, integrationTime);
    _integrationTime = integrationTime;
}

void TCS3414::setGain(uint8_t gain, uint8_t prescaler)
{
    AutoEnable autoEnable(this);
    write8(TCS3414_REGISTER_GAIN, (gain << 4) | prescaler);
}

void TCS3414::getData(uint16_t *red, uint16_t *green, uint16_t *blue, uint16_t *clear)
{
    AutoEnable autoEnable(this);
    waitForIntegrationTime();
    getData(TCS3414_REGISTER_GREEN_LOW_BYTE, green);
    getData(TCS3414_REGISTER_RED_LOW_BYTE, red);
    getData(TCS3414_REGISTER_BLUE_LOW_BYTE, blue);
    getData(TCS3414_REGISTER_CLEAR_LOW_BYTE, clear);
}

void TCS3414::getInformation(uint8_t *partNumber, uint8_t *revisionNumber)
{
    uint8_t id = read8(TCS3414_REGISTER_ID);
    if (partNumber)
    {
        *partNumber = (id >> 4) & 0xF;
    }
    if (revisionNumber)
    {
        *revisionNumber = id & 0xF;
    }
}

void TCS3414::enable()
{
    if (++_enableCount == 1)
    {
        write8(TCS3414_REGISTER_CONTROL, 0x3);  // power up the device and enable ADCs
    }
}

void TCS3414::disable()
{
    if (--_enableCount == 0)
    {
        write8(TCS3414_REGISTER_CONTROL, 0x0); // power down the device and disable ADCs
    }
}

void TCS3414::write8(uint8_t registerAddress, uint8_t value)
{
    Wire.beginTransmission(_address);
    Wire.write(0x80 | registerAddress);
    Wire.write(value);
    Wire.endTransmission();
}

uint8_t TCS3414::read8(uint8_t registerAddress)
{
    Wire.beginTransmission(_address);
    Wire.write(0x80 | registerAddress);
    Wire.endTransmission();

    uint8_t length = sizeof(uint8_t);
    Wire.requestFrom(_address, length);
    return Wire.read();
}

uint16_t TCS3414::read16(uint8_t registerAddress)
{
    uint16_t x; uint16_t t;

    Wire.beginTransmission(_address);
    Wire.write(0x80 | registerAddress);
    Wire.endTransmission();

    uint8_t length = sizeof(uint16_t);
    Wire.requestFrom(_address, length);
    t = Wire.read();
    x = Wire.read();
    x <<= 8;
    x |= t;
    return x;
}

void TCS3414::waitForIntegrationTime()
{
    // Wait for the integration time last specified in setIntegrationTime.
    if (_integrationTime == TCS3414_INTEGRATIONTIME_12MS)
    {
        delay(14);
    }
    else if (_integrationTime == TCS3414_INTEGRATIONTIME_100MS)
    {
        delay(102);
    }
    else if (_integrationTime == TCS3414_INTEGRATIONTIME_400MS)
    {
        delay(403);
    }
    else
    {
        while (1);
    }
}

void TCS3414::getData(uint8_t registerAddress, uint16_t *data)
{
    if (data)
    {
        *data = read16(registerAddress | 0x20); // Query 16-bits from the specified register address.
    }
}
