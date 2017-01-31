/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : Readme.txt
* Author             : MCD Application Team
* Version            : V2.1
* Date               : 12/22/2008
* Description        : Description of the TIM Example 1.
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
This example provides a short description of how to use the TIM peripheral and
gives an example how to configure the TIM0 in PWM and TIM2 in OPM mode.

You have to connect the TIM0 output compare pin (P4.0) to the TIM2 input capture 
pin (P7.2) then you can see the pulse generated in the TIM2 output compare pin 
(P4.4) every rising edge of the PWM signal.


Directory contents
==================
91x_conf.h  Library Configuration file
main.c      Main program
91x_it.c    Interrupt handlers
91x_it.h    Interrupt handlers header file
readme.txt  This file

How to use it
=============
In order to make the program work, you must do the following :
- Create a project and setup all your toolchain's start-up files
- Compile the directory content files and required Library files :
  + 91x_lib.c
  + 91x_tim.c
  + 91x_gpio.c
  + 91x_scu.c
  + 91x_fmi.c
      
- Link all compiled files and load your image into Flash
- Run the example


******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****
