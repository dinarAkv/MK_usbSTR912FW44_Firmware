/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : Readme.txt
* Author             : MCD Application Team
* Version            : V2.1
* Date               : 12/22/2008
* Description        : Description of the ADC Example 1.
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

Example description
===================
This example provides a short description of how to use the ADC peripheral and
gives an example how to configure the ADC in single mode and using the 
interrupt feature.

The example permits to convert the analog signal issued from a trimmer (RV2 on
the MB460 Eval board) and connected to the channel 6 (P4.6 pin 125) of the ADC.


When the conversion is finished an interrupt is generated. 
The c library "printf" function is retargeted to the UART0 so this implementation 
output the converted result on the Hyperterminal using UART0. 



Directory contents
==================
91x_conf.h  Library Configuration file
main.c      Main program
91x_it.c    Interrupt handlers
91x_it.h    Interrupt handlers header file
readme.txt   This file

Hardware environment
====================

/* UART0 configuration -----------------------------------------------------*/
  /* UART0 configured as follow:
        - Word Length = 7 Bits
        - Two Stop Bit
        - No parity
        - BaudRate = 115200 baud
        - Hardware flow control enabled (RTS and CTS signals)
        - Receive and transmit enabled
        - Receive and transmit FIFOs are enabled
        - Transmit and Receive FIFOs levels have 8 bytes depth*/

How to use it
=============
In order to make the program work, you must do the following :
- Create a project and setup all your toolchain's start-up files
- Compile the directory content files and required Library files :
  + 91x_adc.c
  + 91x_lib.c
  + 91x_tim.c
  + 91x_gpio.c
  + 91x_scu.c 
  + 91x_vic.c
  + 91x_uart.c
  + 91x_fmi.c
      
- Link all compiled files and load your image into Flash
- Run the example


******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****
