/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_device_registers.h"

void Servo_move(void);
int i = 0;
/*
 *position 0 -> 333 Low, 6333 High
 *position 45 ->2966 low, 3700 High
 *position 90 -> 5600 low, 1000 High
 *position 135 -> 4880 low, 1786 high
 *position 180 -> 4160 low, 1550 high
 */

//int pos0low = 333;
//int pos0high = 6333;
//int pos45low = 2966;
//int pos45high = 3700;
//int pos90low = 5600;
//int pos90high = 1786;
//int pos135low = 4880;
//int pos135high = 1786;
//int pos180low = 4160;
//int pos180high = 1550;

int main(void)
{
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; //Port C Clock Enable
	SIM_SCGC3 |= SIM_SCGC3_FTM3_MASK; //FTM3 clock enable

	PORTC_PCR10 = 0x300; //PORT C Pin 10 as FTM3_CH6 (ALT3)
	FTM3_MODE = 0x5; //Enable FTM3
	FTM3_MOD = 0xFFFF;
	FTM3_SC = 0x0D; //System clock / 64

	while (1) {
		Servo_move();
	}

	return 0;
}

void Servo_move(void)
{
	//20ms period
	//50Hz period
	/*Generate 50Hz waveform with 1-2ms Duty Cycle
	 *PTC10 pin
	 *PTC10 System clock is 21MHz
	 *Set Prescaler to 64 -> 1 timer tick duration
	 *1 sec / (21MHz / 64) = 3us
	 *# of ticks that the output signal are high and low
	 *HIGH for 1ms -> 1000/3 = 333
	 *HIGH for 2ms -> 2000/3 = 666
	 *LOW for 18ms -> 18000/3 = 6000
	 *LOW for 19ms -> 19000/3 = 6333
	 *Low+HIGH = 6666
	 */
	int low = 6500;
	int high = 300;

//	FTM3_C6SC = 0x1C; // Output-compare; Set output
//	FTM3_C6V = FTM3_CNT + 6500; // 18ms LOW
//	while(!(FTM3_C6SC & 0x80));
//	FTM3_C6SC &= ~(1 << 7);
//
//	FTM3_C6SC = 0x18; // Output-compare; Clear output
//	FTM3_C6V = FTM3_CNT + 300; // 1-2ms HIGH
//	while(!(FTM3_C6SC & 0x80));
//	FTM3_C6SC &= ~(1 << 7);
//	for (int i = 0; i < 10000; i++); // S/W Delay

	for(i = 0; i <= 60; ++i)
	{
		//Make servo rotate
		FTM3_C6SC = 0x1C; // Output-compare; Set output
		FTM3_C6V = FTM3_CNT + low;
		while(!(FTM3_C6SC & 0x80));
		FTM3_C6SC &= ~(1 << 7);

		FTM3_C6SC = 0x18; // Output-compare; Clear output
		FTM3_C6V = FTM3_CNT + high; // 1-2ms HIGH
		while(!(FTM3_C6SC & 0x80));
		FTM3_C6SC &= ~(1 << 7);
		for (int i = 0; i < 10000; i++); // S/W Delay

		low -=20;
		high +=20;
		for (int i = 0; i < 50000; i++); // S/W Delay
	}
	for(i = 60; i >= 0; --i)
	{
		//Make servo rotate in opposite direction
		FTM3_C6SC = 0x1C; // Output-compare; Set output
		FTM3_C6V = FTM3_CNT + low;
		while(!(FTM3_C6SC & 0x80));
		FTM3_C6SC &= ~(1 << 7);

		FTM3_C6SC = 0x18; // Output-compare; Clear output
		FTM3_C6V = FTM3_CNT + high; // 1-2ms HIGH
		while(!(FTM3_C6SC & 0x80));
		FTM3_C6SC &= ~(1 << 7);
		for (int i = 0; i < 10000; i++); // S/W Delay

		low +=20;
		high -=20;
		for (int i = 0; i < 80000; i++); // S/W Delay
	}

}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
