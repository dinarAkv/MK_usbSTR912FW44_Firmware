/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : Readme.txt
* Author             : MCD Application Team
* Version            : V2.1
* Date               : 12/22/2008
* Description        : This sub-directory contains all the user-modifiable files 
*                      needed to create a new project linked with the STR91x library
*                      and working with IAR Embedded Workbench for ARM software 
*                      toolchain (version 5.11A)
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
- project .ewd/.eww/.ewp: A pre-configured project files with the provided library 
                          structure that produces a Flash or RAM executable 
                          image with IAR Embedded Workbench.

- 91x_init.s: This module initializes stack pointers,RAM size , flash                               
              configuration (size, addresses),CP15 registers and finally 
              branches to "main".
 
             
- 91x_vect.s: This file contains exception vectors. 
              It is written in assembler and initializes the exception vectors.

- STR91x_FLASH.icf: This file is the IAR specific linking and loading file used 
                    to load in Flash and execute code and variables to target 
                    memories and regions. You can customize this file to your need.

- STR91x_RAM.icf  : This file is the IAR specific linking and loading file used to 
                    load in RAM and execute code and variables to target memories 
                    and regions. You can customize this file to your need.
                  
Note
====
The files listed above should be only used with IAR Embedded Workbench for ARM 
software toolchain (version 5.11A).


COMPATIBILITY
=============
This project is compatible with EWARM 5.11A version and if you have a previous 
version you have to re-configure the project..


How to use it
=============
In order to make the program work, you must do the following:

- double click on Project.eww file.
- load project image: Project -> Debug(ctrl+D)
- Run program : Debug->Go(F5)


******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE******
