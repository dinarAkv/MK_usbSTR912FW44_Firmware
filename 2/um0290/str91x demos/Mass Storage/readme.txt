/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : readme.txt
* Author             : MCD Application Team
* Version            : V2.0
* Date               : 09/29/2008
* Description        : Description of the USB Mass_Storage Demo.
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
The Mass Storage Demo gives a typical example of how to use the STR91x USB
peripheral to communicate with the PC host using the bulk transfer.
This demo supports the BOT (bulk only transfer) protocol and all needed SCSI
(small computer system interface) commands, and is compatible with both Windows
XP (SP1/SP2) and Windows 2000 (SP4).

More details about this Demo implementation is given in the User manual 
"UM0290 STR7/STR9 USB developer kit", available for download from the ST
microcontrollers website.


Directory contents
==================
 + \include: contains the Demo firmware header files
 + \project: contains pre-configured projects for RVMDK, EWARM5, RIDE and HiTOP 
             toolchains
 + \source: contains the Demo firmware source files


Hardware environment
====================
This example runs on STMicroelectronics STR91x evaluation boards and can be 
easily tailored to any other hardware.

    
How to use it
=============
 + RVMDK
    - Open the MassStorageSimpleBuffer.Uv2 project
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)    

 + EWARM5
    - Open the MassStorageSimpleBuffer.eww workspace.
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)

 + RIDE
    - Open the MassStorageSimpleBuffer.rprj project.
    - Rebuild all files: Project->build project
    - Load project image: Debug->start(ctrl+D)
    - Run program: Debug->Run(ctrl+F9)  

 + HiTOP
    - Open the HiTOP toolchain, a "using projects in HiTOP" window appears.
    - Select open an existing project.
    - Rebuild all files: Project->Rebuild all
    - Click on ok in the "download project" window.
    - Run program: Debug->Go(F5).

******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE******
