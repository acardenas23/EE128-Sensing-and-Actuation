/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : CsIO1.c
**     Project     : Mini_Project_PWM_LCD
**     Processor   : MK64FN1M0VLL12
**     Component   : ConsoleIO
**     Version     : Component 01.016, Driver 01.00, CPU db: 3.00.000
**     Repository  : Kinetis
**     Compiler    : GNU C Compiler
**     Date/Time   : 2021-11-30, 12:10, # CodeGen: 1
**     Abstract    :
**         This component generates low-level methods for redirecting console I/O to the selected UART.
**         These methods are typically used by printf()/scanf() methods.
**     Settings    :
**          Component name                                 : CsIO1
**          Serial_LDD_Link                                : ConsoleIO_Serial_LDD
**          Wait until at least one char is received       : yes
**          New line sequence settings                     : 
**            Rx new line sequence.                        : CR
**            Tx new line sequence.                        : CRLF
**     Contents    :
**         No public methods
**
**     Copyright : 1997 - 2015 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file CsIO1.c
** @version 01.00
** @brief
**         This component generates low-level methods for redirecting console I/O to the selected UART.
**         These methods are typically used by printf()/scanf() methods.
*/         
/*!
**  @addtogroup CsIO1_module CsIO1 module documentation
**  @{
*/         

/* MODULE CsIO1. */

#include "IO_Map.h"
#include "stdio.h"
#include <stdarg.h>
#include "UART_PDD.h"

#ifdef _EWL_STDINT_H

/*
** ===================================================================
**     Method      :  CsIO1___read_console (component ConsoleIO)
**
**     Description :
**         __read_console
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
int __read_console(__file_handle handle, unsigned char* buffer, size_t * count)
{
  size_t CharCnt = 0x00;

  (void)handle;                        /* Parameter is not used, suppress unused argument warning */
  for (;*count > 0x00; --*count) {
    if ((UART_PDD_ReadInterruptStatusReg(UART0_BASE_PTR) & UART_S1_RDRF_MASK) == 0x00) { /* Any data in receiver buffer */
      if (CharCnt != 0x00) {           /* No, at least one char received? */
        break;                         /* Yes, return received char(s) */
      } else {                         /* Wait until a char is received */
        while ((UART_PDD_ReadInterruptStatusReg(UART0_BASE_PTR) & UART_S1_RDRF_MASK) == 0x00) {};
      }
    }
    CharCnt++;                         /* Increase char counter */
    /* Save character received by UARTx device into the receive buffer */
    *buffer = (unsigned char)UART_PDD_GetChar8(UART0_BASE_PTR);
    /* Stop reading if CR (Ox0D) character is received */
    if (*buffer == 0x0DU) {            /* New line character (CR) received ? */
      *buffer = '\n';                  /* Yes, convert LF to '\n' char. */
      break;                           /* Stop loop and return received char(s) */
    }
    buffer++;                          /* Increase buffer pointer */
  }
  *count = CharCnt;
  return (__no_io_error);
}

/*
** ===================================================================
**     Method      :  CsIO1___write_console (component ConsoleIO)
**
**     Description :
**         __write_console
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
int __write_console(__file_handle handle, unsigned char* buffer, size_t* count)
{
  size_t CharCnt = 0x00;

  (void)handle;                        /* Parameter is not used, suppress unused argument warning */
  for (;*count > 0x00; --*count) {
    /* Wait until UART is ready for saving a next character into the transmit buffer */
    while ((UART_PDD_ReadInterruptStatusReg(UART0_BASE_PTR) & UART_S1_TDRE_MASK) == 0) {};
    if (*buffer == '\n') {
      /* Send '\r'(0x0D) before each '\n'(0x0A). */
      /* Save a character into the transmit buffer of the UART0 device */
      UART_PDD_PutChar8(UART0_BASE_PTR, 0x0DU);
      /* Wait until UART is ready for saving a next character into the transmit buffer */
      while ((UART_PDD_ReadInterruptStatusReg(UART0_BASE_PTR) & UART_S1_TDRE_MASK) == 0) {};
    }
    /* Save a character into the transmit buffer of the UART0 device */
    UART_PDD_PutChar8(UART0_BASE_PTR, (unsigned char)*buffer);
    buffer++;                          /* Increase buffer pointer */
    CharCnt++;                         /* Increase char counter */
  }
  *count = CharCnt;
  return(__no_io_error);
}

/*
** ===================================================================
**     Method      :  CsIO1___close_console (component ConsoleIO)
**
**     Description :
**         __close_console
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
int __close_console(__file_handle handle)
{
  (void)handle;                        /* Parameter is not used, suppress unused argument warning */
  return(__no_io_error);
}

#endif /* _EWL_STDINT_H */

#ifndef _EWL_STDINT_H

/*
** ===================================================================
**     Method      :  CsIO1__read (component ConsoleIO)
**
**     Description :
**         _read
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
int _read (int fd, const void *buf, size_t count)
{
  size_t CharCnt = 0x00;

  (void)fd;                            /* Parameter is not used, suppress unused argument warning */
  for (;count > 0x00; --count) {
    if ((UART_PDD_ReadInterruptStatusReg(UART0_BASE_PTR) & UART_S1_RDRF_MASK) == 0x00) { /* Any data in receiver buffer */
      if (CharCnt != 0x00) {           /* No, at least one char received? */
        break;                         /* Yes, return received char(s) */
      } else {                         /* Wait until a char is received */
        while ((UART_PDD_ReadInterruptStatusReg(UART0_BASE_PTR) & UART_S1_RDRF_MASK) == 0x00) {};
      }
    }
    CharCnt++;                         /* Increase char counter */
    /* Save character received by UARTx device into the receive buffer */
    *(uint8_t*)buf = (unsigned char)UART_PDD_GetChar8(UART0_BASE_PTR);
    /* Stop reading if CR (Ox0D) character is received */
    if (*(uint8_t*)buf == 0x0DU) {     /* New line character (CR) received ? */
      *(uint8_t*)buf = '\n';           /* Yes, convert LF to '\n' char. */
      break;                           /* Stop loop and return received char(s) */
    }
    (uint8_t*)buf++;                   /* Increase buffer pointer */
  }
  return CharCnt;
}

/*
** ===================================================================
**     Method      :  CsIO1__write (component ConsoleIO)
**
**     Description :
**         _write
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
int _write (int fd, const void *buf, size_t count)
{
  size_t CharCnt = 0x00;

  (void)fd;                            /* Parameter is not used, suppress unused argument warning */
  for (;count > 0x00; --count) {
    /* Wait until UART is ready for saving a next character into the transmit buffer */
    while ((UART_PDD_ReadInterruptStatusReg(UART0_BASE_PTR) & UART_S1_TDRE_MASK) == 0) {};
    if (*(uint8_t*)buf == '\n') {
      /* Send '\r'(0x0D) before each '\n'(0x0A). */
      /* Save a character into the transmit buffer of the UART0 device */
      UART_PDD_PutChar8(UART0_BASE_PTR, 0x0DU);
      /* Wait until UART is ready for saving a next character into the transmit buffer */
      while ((UART_PDD_ReadInterruptStatusReg(UART0_BASE_PTR) & UART_S1_TDRE_MASK) == 0) {};
    }
    /* Save a character into the transmit buffer of the UART0 device */
    UART_PDD_PutChar8(UART0_BASE_PTR, (unsigned char)*(uint8_t*)buf);
    (uint8_t*)buf++;                   /* Increase buffer pointer */
    CharCnt++;                         /* Increase char counter */
  }
  return CharCnt;
}

#endif /* _EWL_STDINT_H */

/* END CsIO1. */

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
