/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : ssp_flash_driver.h
* Author             : MCD Application Team
* Version            : V2.1
* Date               : 12/22/2008
* Description        : Header for spi_flash_driver.c file.
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
* CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS
* A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
* OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
* OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
* CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SSP_FLASH_DRIVER_H
#define __SSP_FLASH_DRIVER_H

/* Includes ------------------------------------------------------------------*/
#include "91x_lib.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void SSP_FLASH_Init(void);
void SSP_FLASH_SectorErase(u32 SectorAddr);
void SSP_FLASH_BulkErase(void);
void SSP_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SSP_FLASH_BlockWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SSP_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SSP_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);


#endif /* __SSP_FLASH_DRIVER_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
