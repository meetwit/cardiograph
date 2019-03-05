/*
Arduino-MAX30100 oximetry / heart rate integrated sensor library
Copyright (C) 2016  OXullo Intersecans <x@brainrapers.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "myiic.h"
#include "max30100.h"

uint16_t rawIRValue;
uint16_t rawRedValue;

void MAX30100_Begin(void)
{
		MAX30100_RESET();
    MAX30100_SetMode(DEFAULT_MODE);
    MAX30100_SetLedsPulseWidth(DEFAULT_PULSE_WIDTH);
    MAX30100_SetSamplingRate(DEFAULT_SAMPLING_RATE);
    MAX30100_SetLedsCurrent(DEFAULT_IR_LED_CURRENT, DEFAULT_RED_LED_CURRENT);
    MAX30100_SetHighresModeEnabled(TRUE);
	
				i2cWrite(MAX30100_I2C_ADDRESS,0x06,0x0B);        //设置模式
        i2cWrite(MAX30100_I2C_ADDRESS,0x01,0xF0);        //开启中断
        i2cWrite(MAX30100_I2C_ADDRESS,0x09,0x33);        //LED设置
        i2cWrite(MAX30100_I2C_ADDRESS,0x07,0x43);					
        i2cWrite(MAX30100_I2C_ADDRESS,0x02,0x00);    // set FIFO write Pointer reg = 0x00 for clear it
        i2cWrite(MAX30100_I2C_ADDRESS,0x03,0x00);    // set Over Flow Counter  reg = 0x00 for clear it
        i2cWrite(MAX30100_I2C_ADDRESS,0x04,0x00);    // set FIFO Read Pointer  reg = 0x00 for clear it
}

void MAX30100_RESET(void)
{
	i2cWrite(MAX30100_I2C_ADDRESS, MAX30100_REG_MODE_CONFIGURATION, MAX30100_MC_RESET|0x00);
}

void MAX30100_SetMode(Mode mode)
{
    i2cWrite(MAX30100_I2C_ADDRESS, MAX30100_REG_MODE_CONFIGURATION, mode);
}

void MAX30100_SetLedsPulseWidth(LEDPulseWidth ledPulseWidth)
{
    uint8_t previous;
	i2cRead(MAX30100_I2C_ADDRESS, MAX30100_REG_SPO2_CONFIGURATION, 1, &previous);
	i2cWrite(MAX30100_I2C_ADDRESS, MAX30100_REG_SPO2_CONFIGURATION, (previous & 0xfc) | ledPulseWidth);
}

void MAX30100_SetSamplingRate(SamplingRate samplingRate)
{
    uint8_t previous;
	i2cRead(MAX30100_I2C_ADDRESS, MAX30100_REG_SPO2_CONFIGURATION, 1, &previous);
    i2cWrite(MAX30100_I2C_ADDRESS, MAX30100_REG_SPO2_CONFIGURATION, (previous & 0xe3) | (samplingRate << 2));
}

void MAX30100_SetLedsCurrent(LEDCurrent irLedCurrent, LEDCurrent redLedCurrent)
{
		i2cWrite(MAX30100_I2C_ADDRESS, MAX30100_REG_LED_CONFIGURATION, redLedCurrent << 4 | irLedCurrent);
}

void MAX30100_SetHighresModeEnabled(unsigned char enabled)
{	
    uint8_t previous;
		i2cRead(MAX30100_I2C_ADDRESS, MAX30100_REG_SPO2_CONFIGURATION, 1, &previous);
    if (enabled) {
        i2cWrite(MAX30100_I2C_ADDRESS, MAX30100_REG_SPO2_CONFIGURATION, previous | MAX30100_SPC_SPO2_HI_RES_EN);
    } else {
        i2cWrite(MAX30100_I2C_ADDRESS, MAX30100_REG_SPO2_CONFIGURATION, previous & ~MAX30100_SPC_SPO2_HI_RES_EN);
    }
}

void MAX30100_Update(void)
{
    MAX30100_ReadFifoData();
}

uint8_t MAX30100_ReadID(void)
{
	uint8_t id;
	i2cRead(MAX30100_I2C_ADDRESS, MAX30100_REG_PART_ID, 1, &id);
//	if(id!=0x11)
//		return 1;
//	else
//		return 0;
	return id;
}

//meetwit
uint8_t MAX30100_Read(u8 reg)
{
	uint8_t id;
	i2cRead(MAX30100_I2C_ADDRESS, reg, 1, &id);
//	if(id!=0x11)
//		return 1;
//	else
//		return 0;
	return id;
}

//meetwit
uint8_t MAX30100_write(u8 reg,u8 value)
{
	return i2cWrite(MAX30100_I2C_ADDRESS, reg, value);
}



void MAX30100_ReadFifoData(void)
{
    uint8_t buffer[4];

    i2cRead(MAX30100_I2C_ADDRESS, MAX30100_REG_FIFO_DATA, 4, buffer);

    // Warning: the values are always left-aligned
    rawIRValue = (buffer[0] << 8) | buffer[1];
    rawRedValue = (buffer[2] << 8) | buffer[3];
}

