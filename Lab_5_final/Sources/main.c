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

unsigned int ROT_DIR; //0 for CW, 1 for CCW, PB2
unsigned int ROT_SPD; //0 for 22.5, 1 for 180 deg, PB3
unsigned long i;

void clockwise_fct(int);
void counterclockwise_fct(int);

int main(void)
{
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;	//Enable Port B Clock Gate Control
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;	//Enable Port D Clock Gate Control

	PORTB_GPCLR = 0x0C0100;	//PB[3:2] for GPIO
	PORTD_GPCLR = 0x3F0100;	//PD[5:0] for GPIO 0011 1111 0100

	GPIOB_PDDR = 0x0000000;	//Set PORTB for Input
	GPIOD_PDDR = 0x000003F;	//Set PORTD for Output

	while (1) {
		ROT_DIR = GPIOB_PDIR & 0x04;	//PB2
		ROT_SPD = GPIOB_PDIR & 0x08;	//PB3

		if(ROT_DIR == 0x04){
			clockwise_fct(ROT_SPD); //Call clockwise direction while reading the appropriate speed
		}
		else {
			counterclockwise_fct(ROT_SPD); //Call counterclockwise direction while reading the appropriate speed
		}
	}

	for (;;){}

    return 0;
}

void clockwise_fct(int speed) {

	if (speed == 0x08) {
//Full-Step
		GPIOD_PDOR = 0x36;
		for(i = 0; i < 10000; i++);
		GPIOD_PDOR = 0x35;
		for(i = 0; i < 10000; i++);
		GPIOD_PDOR = 0x39;
		for(i = 0; i < 10000; i++);
		GPIOD_PDOR = 0x3A;
		for(i = 0; i < 10000; i++);
	} else {
//Half-Step
		GPIOD_PDOR = 0x32;
		for(i = 0; i < 10000; i++);
		GPIOD_PDOR = 0x36;
		for(i = 0; i < 10000; i++);
		GPIOD_PDOR = 0x34;
		for(i = 0; i < 10000; i++);
		GPIOD_PDOR = 0x35;
		for(i = 0; i < 10000; i++);
		GPIOD_PDOR = 0x31;
		for(i = 0; i < 10000; i++);
		GPIOD_PDOR = 0x39;
		for(i = 0; i < 10000; i++);
		GPIOD_PDOR = 0x38;
		for(i = 0; i < 10000; i++);
		GPIOD_PDOR = 0x3A;
		for(i = 0; i < 10000; i++);
	}
}

void counterclockwise_fct(int speed) {

	if (speed == 0x08) {
//Full-Step
		GPIOD_PDOR = 0x3A;
		for(i = 0; i < 10000; i++);
		GPIOD_PDOR = 0x39;
		for(i = 0; i < 10000; i++);
		GPIOD_PDOR = 0x35;
		for(i = 0; i < 10000; i++);
		GPIOD_PDOR = 0x36;
		for(i = 0; i < 10000; i++);
	} else {
//Half-Step
		GPIOD_PDOR = 0x3A;
		for(i = 0; i < 10000; i++);
		GPIOD_PDOR = 0x38;
		for(i = 0; i < 10000; i++);
		GPIOD_PDOR = 0x39;
		for(i = 0; i < 10000; i++);
		GPIOD_PDOR = 0x31;
		for(i = 0; i < 10000; i++);
		GPIOD_PDOR = 0x35;
		for(i = 0; i < 10000; i++);
		GPIOD_PDOR = 0x34;
		for(i = 0; i < 10000; i++);
		GPIOD_PDOR = 0x36;
		for(i = 0; i < 10000; i++);
		GPIOD_PDOR = 0x32;
		for(i = 0; i < 10000; i++);
	}
}
