/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : Readme.txt
* Author             : MCD Application Team
* Version            : V2.1
* Date               : 12/22/2008
* Description        : This sub-directory contains all the user-modifiable files 
*                      needed to create a new project linked with the STR91x library
*                      and working with HiTOP53-ARM toolchain (version 5.30)
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

Directory contents
===================
- project .htp: A pre-configured project files with the provided library 
                structure that produces a Flash executable image with HiTOP.

- Obj : This directory will contain the executable images.
 
             
- Setting : This directory contains the linker and script files.

             * STR912.lsl: This file is the HiTOP specific linking and loading 
                           file used to load in FLASH and execute code and variables 
                           to target memories and regions. 
                           You can customize this file to your need.

             * StartupScript.scr: this script resets the target before loading 
                                  the executable image.
             * Reset_go_main.scr : this script runs the program till main and should be
                                   executed after loading the application into flash. 

- Src : This directory includes the following file:          
             
              * cstart_thumb2.asm: This file initializes stack pointers,RAM size , flash                               
                                   configuration (size, addresses),exception vectors, CP15 
                                   registers and finally branches to "main".
                                   It is written in assembler and initializes the exception 
                                   vectors.
                           
Note
====
The files listed above should be only used with HiTOP53-ARM toolchain (version 5.30).


COMPATIBILITY
=============
This project is compatible with HiTOP53-ARM 5.30 version and if you have a previous 
version you have to re-configure the project.


How to use it
=============
In order to make the program work, you must do the following:


    - Open the HiTOP toolchain, a "using projects in HiTOP" window appears.
    - Select open an existing project.
    - Open the project.htp.
    - Rebuild all files: Project->Rebuild all
    - Click on ok in the "download project" window.
    - Execute the "reset_go_main.scr" script from the toolbar menu.
    - Run program: Debug->Go(F5)..


******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE******
