/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : Readme.txt
* Author             : MCD Application Team
* Version            : V2.1
* Date               : 12/22/2008
* Description        : Description of the MC Example.
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
This example show how to configure the MC peripheral to generate Three complementary
PWM signals, to insert a defined dead time value and to use the Tacho interface in 
hardware Mode.
The PCLK frequency is set to 48 MHz, the MC is running at 23.43 KHz, the 
PulseU, PulseV and PulseW duty cycles are fixed respectively to 50%, 25% and 12.5%.
The dead time is equal to 625 nanoseconds. 
The PWM waveform can be display in a scope. 


Directory contents
==================
91x_conf.h  Library Configuration file
main.c      Main program
91x_it.c    Interrupt handlers
91x_it.h    Interrupt handlers header file
readme.tx   This file.

Hardware environment
====================
Connect a scope to:
- Pin29 (P6.00/MC_UH).  
- Pin31 (P6.01/MC_UL).
- Pin19 (P6.02/MC_VH).
- Pin20 (P6.03/MC_VL).
- Pin83 (P6.04/MC_WH).
- Pin84 (P6.05/MC_WL).


How to use it
=============
In order to make the program work, you must do the following :
- Create a project and setup all your toolchain's start-up files
- Compile the directory content files and required Library files :
  + 91x_lib.c
  + 91x_mc.c
  + 91x_gpio.c
  + 91x_scu.c
  + 91x_fmi.c

      
- Link all compiled files and load your image into Flash
- Run the example


******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****
