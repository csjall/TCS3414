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
#ifndef TCS3414_h
#define TCS3414_h

#include <application.h>

enum
{
    TCS3414_REGISTER_CONTROL = 0x0,
    TCS3414_REGISTER_TIMING,
    TCS3414_REGISTER_INTERRUPT,
    TCS3414_REGISTER_INTERRUPTSOURCE,
    TCS3414_REGISTER_ID,
    TCS3414_REGISTER_GAIN = 0x7,
    TCS3414_REGISTER_LOW_THRESH_LOW_BYTE,
    TCS3414_REGISTER_LOW_THRESH_HIGH_BYTE,
    TCS3414_REGISTER_HIGH_THRESH_LOW_BYTE,
    TCS3414_REGISTER_HIGH_THRESH_HIGH_BYTE,
    TCS3414_REGISTER_GREEN_LOW_BYTE = 0x10,
    TCS3414_REGISTER_GREEN_HIGH_BYTE,
    TCS3414_REGISTER_RED_LOW_BYTE,
    TCS3414_REGISTER_RED_HIGH_BYTE,
    TCS3414_REGISTER_BLUE_LOW_BYTE,
    TCS3414_REGISTER_BLUE_HIGH_BYTE,
    TCS3414_REGISTER_CLEAR_LOW_BYTE,
    TCS3414_REGISTER_CLEAR_HIGH_BYTE,
};

enum
{
    TCS3414_INTEGRATIONTIME_12MS = 0x0,
    TCS3414_INTEGRATIONTIME_100MS,
    TCS3414_INTEGRATIONTIME_400MS,
};

enum
{
    TCS3414_GAIN_1X = 0x0,
    TCS3414_GAIN_4X,
    TCS3414_GAIN_16X,
    TCS3414_GAIN_64X,
};

enum
{
    TCS3414_PRESCALARMODE_DIVIDE_BY_1 = 0x0,
    TCS3414_PRESCALARMODE_DIVIDE_BY_2,
    TCS3414_PRESCALARMODE_DIVIDE_BY_4,
    TCS3414_PRESCALARMODE_DIVIDE_BY_8,
    TCS3414_PRESCALARMODE_DIVIDE_BY_16,
    TCS3414_PRESCALARMODE_DIVIDE_BY_32,
    TCS3414_PRESCALARMODE_DIVIDE_BY_64,
};

// This class provides implementation for a TCS3414 digital color sensor connected
// to a Spark Core WiFi development kit. This implementation uses the I2C bus to
// configure the sensor and acquire 16-bit red, green, blue, and clear channels.
class TCS3414
{
public:
    TCS3414(uint8_t address = 0x39) :
      _address(address),
      _enableCount(0)
    {
    }

    boolean begin();
    uint8_t getPartNumber() const;
    uint8_t getRevisionNumber() const;
    void setIntegrationTime(uint8_t integrationTime);
    void setGain(uint8_t gain, uint8_t prescaler);
    void getData(uint16_t *red, uint16_t *green, uint16_t *blue, uint16_t *clear);

private:
    uint8_t _address;
    uint8_t _partNumber;
    uint8_t _revisionNumber;
    uint8_t _integrationTime;
    uint8_t _enableCount;

    void getInformation(uint8_t *partNumber, uint8_t *revisionNumber);
    void enable();
    void disable();
    void write8(uint8_t registerAddress, uint8_t value);
    uint8_t read8(uint8_t registerAddress);
    uint16_t read16(uint8_t registerAddress);
    void waitForIntegrationTime();
    void getData(uint8_t registerAddress, uint16_t *data);

    class AutoEnable
    {
    private:
        TCS3414 *_TCS3414;

    public:
        AutoEnable(TCS3414 *TCS3414) :
          _TCS3414(TCS3414)
        {
            _TCS3414->enable();
        }

        ~AutoEnable()
        {
            _TCS3414->disable();
        }
    };
};

#endif
