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

#ifndef __MAX30100_H
#define __MAX30100_H

#include "MAX30100_Registers.h"
#include "stm32f10x.h"


#define DEFAULT_MODE                MAX30100_MODE_HRONLY
#define DEFAULT_SAMPLING_RATE       MAX30100_SAMPRATE_50HZ
#define DEFAULT_PULSE_WIDTH         MAX30100_SPC_PW_1600US_16BITS
#define DEFAULT_RED_LED_CURRENT     MAX30100_LED_CURR_11MA
#define DEFAULT_IR_LED_CURRENT      MAX30100_LED_CURR_11MA

		extern uint16_t rawIRValue;
		extern uint16_t rawRedValue;


	void MAX30100_RESET(void);
    void MAX30100_Begin(void);
    void MAX30100_SetMode(Mode mode);
    void MAX30100_SetLedsPulseWidth(LEDPulseWidth ledPulseWidth);
    void MAX30100_SetSamplingRate(SamplingRate samplingRate);
    void MAX30100_SetLedsCurrent(LEDCurrent irLedCurrent, LEDCurrent redLedCurrent);
    void MAX30100_SetHighresModeEnabled(unsigned char enabled);
    void MAX30100_Update(void);

    uint8_t MAX30100_ReadID(void);
    void MAX30100_ReadFifoData(void);
		
		uint8_t MAX30100_Read(u8 reg);
		uint8_t MAX30100_write(u8 reg,u8 value);

#endif
