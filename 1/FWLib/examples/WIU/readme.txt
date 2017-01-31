/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : Readme.txt
* Author             : MCD Application Team
* Version            : V2.1
* Date               : 12/22/2008
* Description        : Description of the WIU Example.
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
This example provides a description of how to configure an wakeup interrupt 
line. In this example, the wakeup line 28 (External group 3)is configured to 
generate a FIQ interrupt on each falling edge. In the interrupt routine a led 
connected to P9.0 will be toggled.
This led will be toggled due to the interrupt generated on wakeup Line 28 
at each falling edge.

Directory contents
==================

91x_conf.h  Library Configuration file
main.c      Main program
91x_it.c    Interrupt handlers
91x_it.h    Interrupt handlers header file
readme.txt   This file

Hardware environment
====================
Connect a push-button to P7.4 pin with a resistor and a capacity to generate
edges on WIU line 28 (Key push-button and joystick push buttons in STR91x-EVAL 
board). Connect a LED to P9.0 pin (LD2 in STR91x-EVAL board)

How to use it
=============
In order to make the program work, you must do the following :
- Create a project and setup all your toolchain's start-up files
- Compile the directory content files and required Library files :
  + 91x_lib.c
  + 91x_scu.c
  + 91x_vic.c
  + 91x_wiu.c
  + 91x_gpio.c
  + 91x_fmi.c
  
- Link all compiled files and load your image into Flash
- Run the example


******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE******
