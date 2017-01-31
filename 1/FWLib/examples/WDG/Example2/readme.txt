/******************** (C) COPYRIGHT 2008 STMicroelectronics **********************
* File Name          : Readme.txt
* Author             : MCD Application Team
* Version            : V2.1
* Date               : 12/22/2008
* Description        : Description of the WDG example2
**********************************************************************************
 THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
 CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
 OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**********************************************************************************

Example description
===================
This example configures the watchdog pripheral on watchdog mode and starts the watchdog.

When pin P7.4 goes low (PB3 Button pressed on the STR910 eval board:MB460), the watchdog counter
will be started and a reset is generated after 5 seconds delay.

* A led sequence on the STR910 eval board will indicate restart of application

* Note that the wdg use the RTC 32Khz clock as clock source.

Directory contents
==================
91x_conf.h  Library Configuration file
main.c      Main program
91x_it.c    Interrupt handlers
91x_it.h    Interrupt handlers header file
readme.txt   This file

How to use it
=============
In order to make the program work, you must do the following :
- Create a project and setup all your tool chain’s start-up files
- Compile the directory content files and required Library files:
  + 91x_lib.c
  + 91x_gpio.c
  + 91x_wdg.c
  + 91x_scu.c
  + 91x_fmi.c
  
- Link all compiled files & load program into the Flash.
- Run the example


******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****
