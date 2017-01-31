/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : readme.txt
* Author             : MCD Application Team
* Version            : V2.0
* Date               : 09/29/2008
* Description        : Description of the XTI Example.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

Example description
===================
This directory contains a set of sources files that build the application to be
loaded into into Flash memory using Device Firmware Upgrade demo.

This example demonstrates how to configure the XTI to generate IRQ interrupts.
This example highlights one of the features of the XTI which is to generate an
IRQ interrupt service routine and use the P0.15 pins (tied to the wakeup button 
monted on the MB393 board)to execute the interrupt code.
If the program work properly you will see the Led connected to the P0.0 
pin (LD6 on the  MB393 board) change state every time when you press the wakeup
button.

 
Directory contents
==================
 71x_conf.h  Library Configuration file
 71x_it.c    Interrupt handlers
 71x_it.h    Interrupt handlers header file
 main.c      Main program
 

Hardware environment
====================
This example works as standalone on the STR71x-Eval board (mb393) and extra 
hardware implementation is not required to run the example.



How to use it
=============
In order to load the project example with the DFU, you must do the following:
 + EWARMv5:
   - Open the project.eww workspace
   - Rebuild all files: Project->Rebuild all
   - A binary file project.bin" will be generated under "user application\Exe" 
     folder 
   - Finaly load this image with DFU application

 + RIDE
    - Open the project.rprj project
    - Rebuild all files: Project->build project
    - Run "hextobin.bat"
    - A binary file "project.bin" will be generated under "\obj" folder
    - Finaly load this image with DFU application

 + RVMDK (Full version is required)
    - Open the project.Uv2 project
    - Rebuild all files: Project->Rebuild all target files
    - Run "axftobin.bat"
    - A binary file "project.bin" will be generated under "\Output" folder
    - Finaly load this image with DFU application
  
  + HiTOP     
    - Open the HiTOP toolchain, a "using projects in HiTOP" window appears.
    - Select open an existing project.
    - Browse to open the project.htp.
    - Rebuild all files: Project->Rebuild all.
    - Load the project: Click on ok in the "download project" window.
    - Run "hextobin.bat"
    - A binary file "Project.bin" will be generated under "Objects" folder.
    
       
 + The generated .bin file should be converted to the DFU format using the “DFU File
   Manager Tool” included in the “DfuSe” PC software install. For more details on
   how to convert a .bin file to DFU format please refer to the UM0412 user manual
   “Getting started with DfuSe USB device firmware upgrade STMicroelectronics extension”
   available from the STMicroelectronics microcontroller website www.st.com.


******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****
