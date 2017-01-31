/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : readme.txt
* Author             : MCD Application Team
* Version            : V2.0
* Date               : 09/29/2008
* Description        :  Description of the RTC Example.
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

This example configures the RTC for generating a 1024Hz periodic interrupt.

This interrupt will be used to toogle a led connected to GPIO P9.2: you should see 
a square waveform on pin P9.2 with a frequency of 512Hz.


Directory contents
==================
 + project
     - EWARMv5: This folder contains a pre-configured project file that 
                produces a binary image of RTC example to be loaded with DFU.

     - RIDE: This folder contains a pre-configured project file that produces a
             binary image of RTC example to be loaded with DFU.

     - RVMDK: This folder contains a pre-configured project file that produces a
              binary image of RTC example to be loaded with DFU.              
     
     - HiTOP: This folder contains a pre-configured project file that produces a
              binary image of RTC example to be loaded with DFU. 
              
91x_conf.h  Library Configuration file
91x_it.c    Interrupt handlers
main.c      Main program
91x_it.h    Interrupt handlers header file
readme.txt  This file

Hardware environment
====================
 - Connect a led to P9.2 pin (LD4 on STR91x-EVAL board).
   
     
How to use it
=============
In order to load the project example with the DFU, you must do the following:
 + EWARMv5:
    - Open the project.eww workspace
   -  Rebuild all files: Project->Rebuild all
   - A binary file project.bin" will be generated under "user application\Exe" 
     folder 
   - Finaly load this image with DFU application

 + RIDE
    - Open the project.rprj project
    - Rebuild all files: Project->build project
    - Run "hextobin.bat"
    - A binary file "project.bin" will be generated under "\obj" folder
    - Finaly load this image with DFU application

 + RVMDK
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

