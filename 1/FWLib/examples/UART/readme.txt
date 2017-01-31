/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : Readme.txt
* Author             : MCD Application Team
* Version            : V2.1
* Date               : 12/22/2008
* Description        : Description of the UART Example.
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
This example provides a description of how  to use the UART with hardware flow
control and communicate with the Hyperterminal.
First, the UART0 sends the TxBuffer to the hyperterminal, then received character will 
be sent back.

The UART0 is configured as follow:
  - Word Length = 7 Bits
  - Two Stop Bit
  - No parity
  - BaudRate = 115200 baud
  - Hardware flow control enabled (RTS and CTS signals)
  - Receive and transmit enabled
  - Receive and transmit FIFOs are disabled


Directory contents
==================
91x_conf.h  Library Configuration file
main.c      Main program
91x_it.c    Interrupt handlers
91x_it.h    Interrupt handlers header file
readme.txt   This file

Hardware environment
====================
 - All the UART0 signals (Rx, Tx, RTS and CTS) must be connected to a DB9 
   connector using a RS232 transceiver.
   + UART0_Rx (P5.1)
   + UART0_Tx (P3.4)
   + UART0_RTS(P3.3)
   + UART0_CTS(P2.0)

 - Connect a null-modem female/female RS232 cable between the DB9 connector 
   and PC serial port.  

 - Hyperterminal configuration:
    - Word Length = 7 Bits
    - Two Stop Bit
    - No parity
    - BaudRate = 115200 baud
    - flow control: Hardware 
  
  
How to use it
=============
In order to make the program work, you must do the following :
- Create a project and setup all your toolchain's start-up files
- Compile the directory content files and required Library files :
  + 91x_lib.c
  + 91x_uart.c
  + 91x_gpio.c
  + 91x_scu.c
  + 91x_vic.c
  + 91x_fmi.c

- Link all compiled files and load your image into Flash
- Run the example


******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****
