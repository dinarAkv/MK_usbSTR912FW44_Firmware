/******************** (C) COPYRIGHT 2008 STMicroelectronics **********************
* File Name          : Readme.txt
* Author             : MCD Application Team
* Version            : V2.1
* Date               : 12/22/2008
* Description        : Description of the DMA example
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

This example configures the DMA peripheral to perform a memory to memory Data transfer.
The example consists in transferring a block of Data which occupies contiguous areas 
in memory "Buffer0" and scatters the Data in three  memory areas"buffer1, 
Buffer2 & Buffer3".

The example uses a linked lists Item structure in the memory.
After running the example, you can verify with your tool chain’s that the content 
of Buffer0 is transferred to three buffers (Buffer1, Buffer2 & Buffer3).


Directory contents
==================
91x_conf.h Library Configuration file
main.c      Main program
91x_it.c    Interrupt handlers
91x_it.h    Interrupt handlers header file
readme.tx   This file.

How to use it
=============
In order to make the program work, you must do the following :
- Create a project and setup all your tool chain’s start-up files
- Compile the directory content files and required Library files:
  + 91x_lib.c
  + 91x_dma.c
  + 91x_scu.c
  + 91x_fmi.c
  
- Link all compiled files & load program into the Flash.
- Run the example


******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****
