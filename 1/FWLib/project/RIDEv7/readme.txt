/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : Readme.txt
* Author             : MCD Application Team
* Version            : V2.1
* Date               : 12/22/2008
* Description        : This sub-directory contains all the user-modifiable files 
*                      needed to create a new project linked with the STR91x library
*                      and working with RIDE7 software toolchain (version 1.03.0003)
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
- project .rprj/rapp: A pre-configured project files with the provided library 
                structure that produces a FLASH or RAM image with RIDE7 
                software.

- 91x_init.s: This module initializes the exception vectors, stack pointers,
              RAM size , flash configuration (size, addresses),CP15 registers and 
              finally branches to "main".       

                  
Note
====
The files listed above should be only used with RIDE software 
toolchain(1.03.0003 and later).



How to use it
=============
In order to make the program work, you must do the following:

- double click on Project.rprj file.
- Rebuild all files: Project->build project
- Load project image: Debug->start
- Run program: Debug->Run(ctrl+F9)   


******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE******
