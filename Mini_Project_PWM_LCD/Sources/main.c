/* ###################################################################
 **     Filename    : main.c
 **     Project     : Mini_Project_PWM_LCD
 **     Processor   : MK64FN1M0VLL12
 **     Version     : Driver 01.01
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2021-11-30, 12:03, # CodeGen: 0
 **     Abstract    :
 **         Main module.
 **         This module contains user's application code.
 **     Settings    :
 **     Contents    :
 **         No public methods
 **
 ** ###################################################################*/
/*!
 ** @file main.c
 ** @version 01.01
 ** @brief
 **         Main module.
 **         This module contains user's application code.
 */
/*!
 **  @addtogroup main_module main module documentation
 **  @{
 */
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "Pins1.h"
#include "CsIO1.h"
#include "IO1.h"
#include "SM1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PDD_Includes.h"
#include "Init_Config.h"
#include "MK64F12.h"
/* User includes (#include below this line is not maintained by Processor Expert) */

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
unsigned short ADC0_read16b(void);
unsigned short ADC1_read16b(void);
unsigned int photo1 = 0;
unsigned int photo2 = 0;
int i = 0;
unsigned char write[512];

int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	/* Write your local variable definition here */
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK; //Enable Port B CGC
	PORTB_GPCLR = 0x0C0100;
	GPIOB_PDDR = 0xFF; //Set PORTB for output

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

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	/* Write your code here */
	/* FIXME */
	int len, high, low;
	LDD_TDeviceData *SM1_DeviceData;
	SM1_DeviceData = SM1_Init(NULL);

	printf("Hello\n");

	while(1){
		printf("Hi from for loop \n");
		photo1 = ((unsigned int)ADC0_read16b()*33)/0xFFFF;
		photo2 = ((unsigned int)ADC1_read16b()*33)/0xFFFF;

		printf("Photoresistor 1 value in decimal \t: %4d\n", photo1);
		len = sprintf(write, "Photoresistor 1  value in decimal \t: %4d\n",photo1);
		//Set Port B Pin 2 to High
		GPIOB_PDOR = 0x04;
		SM1_SendBlock(SM1_DeviceData, &write, len);

		printf("Photoresistor 2 value in decimal \t: %4d\n", photo2);
		len = sprintf(write, "Photoresistor 1  value in decimal \t: %4d\n",photo2);
		//Set Port B Pin 2 to High
		GPIOB_PDOR = 0x04;
		SM1_SendBlock(SM1_DeviceData, &write, len);

		//Servo_move();
		if(photo1>photo2){
			GPIOD_PDOR = 0x01;
			FTM3_C6SC = 0x1C; // Output-compare; Set output
			FTM3_C6V = FTM3_CNT + 6200; // 18ms LOW
			while(!(FTM3_C6SC & 0x80));
			FTM3_C6SC &= ~(1 << 7);

			FTM3_C6SC = 0x18; // Output-compare; Clear output
			FTM3_C6V = FTM3_CNT + 500; // 1-2ms HIGH
			while(!(FTM3_C6SC & 0x80));
			FTM3_C6SC &= ~(1 << 7);
			for (int i = 0; i < 100000; i++); // S/W Delay

		}else if(photo1<photo2){
			GPIOD_PDOR = 0x02;
			//135
			FTM3_C6SC = 0x1C; // Output-compare; Set output
			FTM3_C6V = FTM3_CNT + 5000; // 18ms LOW
			while(!(FTM3_C6SC & 0x80));
			FTM3_C6SC &= ~(1 << 7);

			FTM3_C6SC = 0x18; // Output-compare; Clear output
			FTM3_C6V = FTM3_CNT + 1200; // 1-2ms HIGH
			while(!(FTM3_C6SC & 0x80));
			FTM3_C6SC &= ~(1 << 7);
			for (int i = 0; i < 100000; i++); // S/W Delay
		}else{
			GPIOD_PDOR = 0x03;
			//90
			FTM3_C6SC = 0x1C; // Output-compare; Set output
			FTM3_C6V = FTM3_CNT + 5750; // 18ms LOW
			while(!(FTM3_C6SC & 0x80));
			FTM3_C6SC &= ~(1 << 7);

			FTM3_C6SC = 0x18; // Output-compare; Clear output
			FTM3_C6V = FTM3_CNT + 580; // 1-2ms HIGH
			while(!(FTM3_C6SC & 0x80));
			FTM3_C6SC &= ~(1 << 7);
			for (int i = 0; i < 100000; i++); // S/W Delay
		}
	}

	/*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
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
/*!
 ** @}
 */
/*
 ** ###################################################################
 **
 **     This file was created by Processor Expert 10.5 [05.21]
 **     for the Freescale Kinetis series of microcontrollers.
 **
 ** ###################################################################
 */
