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

unsigned short ADC0_read16b(void);
unsigned short ADC1_read16b(void);
void Servo_move(void);

unsigned int photo1 = 0;
unsigned int photo2 = 0;
int i = 0;
int a = 0;
int low, high;

int main(void)
{
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;	// 0x8000000u; Enable ADC0 Clock
	SIM_SCGC3 |= SIM_SCGC3_ADC1_MASK;	//Enable ADC1 Clock
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;	//Port C Clock Enable
	SIM_SCGC3 |= SIM_SCGC3_FTM3_MASK;	//FTM3 clock enable
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;	/*Enable Port D Clock Gate Control*/

	PORTD_GPCLR = 0x00FF0100; //Configure PD[0:7] for GPIO

	ADC0_CFG1 = 0x0C; //Configure ADC for 16 bits, and to use bus clock.
	ADC0_SC1A = 0x1F; //Disable the ADC module;

	ADC1_CFG1 = 0x0C; //Configure ADC for 16 bits, and to use bus clock.
	ADC1_SC1A = 0x1F; //Disable the ADC module;

	PORTC_PCR10 = 0x300; //PORT C Pin 10 as FTM3_CH6 (ALT3)
	FTM3_MODE = 0x5; //Enable FTM3
	FTM3_MOD = 0xFFFF;
	FTM3_SC = 0x0D; //System clock / 64

	GPIOD_PDDR = 0xFF; //PORTD [7:0] output


	while (1) {
		//adc
		photo1 = ((unsigned int)ADC0_read16b()*33)/0xFFFF;
		photo2 = ((unsigned int)ADC1_read16b()*33)/0xFFFF;

		Servo_move();
		if(photo1>photo2){
			GPIOD_PDOR = 0x01;
			a = 45;
		}else if(photo1<photo2){
			GPIOD_PDOR = 0x02;
			a = 135;
		}else{
			GPIOD_PDOR = 0x03;
			a = 90;
		}
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

	/*
	 * 0 -> 6400 low, 300 high
	 * 45 -> 6200 low, 500 high
	 * 90 -> 5750 low, 850 high
	 * 135 -> 5000 low, 1200 high
	 * 180 -> 4160 low, 1550 high
	 */
	if(a == 45){
		low = 6200;
		high = 500;
	}else if(a == 90){
		low = 5750;
		high = 850;
	}else if(a == 135){
		low = 5000;
		high = 1200;

	}

//	int low = 6400;
//	int high = 300;

	FTM3_C6SC = 0x1C; // Output-compare; Set output
	FTM3_C6V = FTM3_CNT + low; // 18ms LOW
	while(!(FTM3_C6SC & 0x80));
	FTM3_C6SC &= ~(1 << 7);

	FTM3_C6SC = 0x18; // Output-compare; Clear output
	FTM3_C6V = FTM3_CNT + high; // 1-2ms HIGH
	while(!(FTM3_C6SC & 0x80));
	FTM3_C6SC &= ~(1 << 7);
	for (int i = 0; i < 100000; i++); // S/W Delay



//	for(i = 0; i <= 60; ++i)
//	{
//		//Make servo rotate
//		FTM3_C6SC = 0x1C; // Output-compare; Set output
//		FTM3_C6V = FTM3_CNT + low;
//		while(!(FTM3_C6SC & 0x80));
//		FTM3_C6SC &= ~(1 << 7);
//
//		FTM3_C6SC = 0x18; // Output-compare; Clear output
//		FTM3_C6V = FTM3_CNT + high; // 1-2ms HIGH
//		while(!(FTM3_C6SC & 0x80));
//		FTM3_C6SC &= ~(1 << 7);
//		for (int i = 0; i < 10000; i++); // S/W Delay
//
//		low -=20;
//		high +=20;
//		for (int i = 0; i < 50000; i++); // S/W Delay
//	}
//	for(i = 60; i >= 0; --i)
//	{
//		//Make servo rotate in opposite direction
//		FTM3_C6SC = 0x1C; // Output-compare; Set output
//		FTM3_C6V = FTM3_CNT + low;
//		while(!(FTM3_C6SC & 0x80));
//		FTM3_C6SC &= ~(1 << 7);
//
//		FTM3_C6SC = 0x18; // Output-compare; Clear output
//		FTM3_C6V = FTM3_CNT + high; // 1-2ms HIGH
//		while(!(FTM3_C6SC & 0x80));
//		FTM3_C6SC &= ~(1 << 7);
//		for (int i = 0; i < 10000; i++); // S/W Delay
//
//		low +=20;
//		high -=20;
//		for (int i = 0; i < 80000; i++); // S/W Delay
//	}

}

unsigned short ADC0_read16b(void)
{
	ADC0_SC1A=0x00;
	while(ADC0_SC2 & ADC_SC2_ADACT_MASK);
	while(!(ADC0_SC1A & ADC_SC1_COCO_MASK));
	return ADC0_RA;

}

unsigned short ADC1_read16b(void)
{
	ADC1_SC1A=0x00;
	while(ADC1_SC2 & ADC_SC2_ADACT_MASK);
	while(!(ADC1_SC1A & ADC_SC1_COCO_MASK));
	return ADC1_RA;

}



////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
