/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : memory.h
* Author             : MCD Application Team
* Version            : V2.0.0
* Date               : 09/29/2008
* Description        : Memory management layer
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __memory_H
#define __memory_H
/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Read_Memory(void);
void Write_Memory(void);
bool Address_Management_Test(u8 Cmd);
#endif /* __memory_H */
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
