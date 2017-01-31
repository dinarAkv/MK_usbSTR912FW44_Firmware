/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : readme.txt
* Author             : MCD Application Team
* Version            : V2.0
* Date               : 09/29/2008
* Description        : Description of the USB Device_Firmware_Upgrade Demo.
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
This Demo presents the implementation of a device firmware upgrade (DFU) capability
in the STR7 microcontroller. It follows the DFU class specification defined
by the USB Implementers Forum for reprogramming an application through USB. 
The DFU principle is particularly well suited to USB applications that need to be
reprogrammed in the field.

More details about this Demo implementation is given in the User manual 
"UM0290 STR7/STR9 USB developer kit", available for download from the ST
microcontrollers website.


Directory contents
==================
 + \binary     contains a set of sources files that build the application to be
               loaded with DFU 
 + \images     contains DFU images to be loaded in the STR7 microcontroller
 + \include    contains the Demo firmware header files
 + \project    contains pre-configured projects for RVMDK, EWARM5, RIDE
               and HiTOP toolchains
 + \source     contains the Demo firmware source files


Hardware environment
====================
This example runs on STMicroelectronics STR71x evaluation boards and can be 
easily tailored to any other hardware.
    
How to use it
=============
 + RVMDK (Full version is required)
    - Open the DFU.Uv2 project
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)    

 + EWARM5
    - Open the DFU.eww workspace.
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)

 + RIDE
    - Open the DFU.rprj project.
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
