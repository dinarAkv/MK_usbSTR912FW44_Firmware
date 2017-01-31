/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : Readme.txt
* Author             : MCD Application Team
* Version            : V2.1
* Date               : 12/22/2008
* Description        : Description of the SSP Example2.
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
This example provides a description of how to communicate with the SSP Flash memory
(ST M25P64 Flash memory)available on STR91xF-EVAL board (MB460).It aims to write 
a Tx_Buffer into sector0 of SSP Flash and then make a comparison with 
the read data to check if the operation is done successfully.The result of this 
comparison is stored in the "TransferStatus"variable.   


Directory contents
==================
91x_conf.h              Library Configuration file
91x_it.c                Interrupt handlers
main.c                  Main program
ssp_flash_driver.h      SSP Flash driver header file
ssp_flash_driver.c      SSP Flash driver
91x_it.h                Interrupt handlers header file
readme.txt              This file


How to use it
=============
In order to make the program work, you must do the following :
- Create a project and setup all your toolchain's start-up files
- Compile the directory content files and required Library files :
  + 91x_lib.c
  + 91x_ssp.c
  + 91x_gpio.c
  + 91x_scu.c
  + 91x_fmi.c
  
- Link all compiled files and load your image into Flash
- Run the example


******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****
