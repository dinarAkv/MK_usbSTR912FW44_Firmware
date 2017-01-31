/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : hw_config.h
* Author             : MCD Application Team
* Version            : V2.0.0
* Date               : 09/29/2008
* Description        : Hardware Configuration & Setup
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "usb_type.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  vu32 CR0;
  vu32 CR1;
  vu32 DR0;
  vu32 DR1;
  vu32 AR;
  vu32 ER;
} FLASH_TypeDef;

/* Exported constants --------------------------------------------------------*/
#define ApplicationAddress 0x40004000
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Set_System(void);
void Set_USBClock(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void USB_Interrupts_Config(void);

void DFU_Button_Config(void);
u32  DFU_Button_Read(void);

void Reset_Device(void);

#ifdef __GNUC__
void Internal_FLASH_SectorErase(u32 Sectors) __attribute__ ((section (".coderam")));
void Internal_FLASH_WordWrite(u32 Address, u32 Data) __attribute__ ((section (".coderam")));
void Internal_FLASH_WritePrConfig(u32 Xsectors, FunctionalState NewState)__attribute__ ((section (".coderam")));

#else
void Internal_FLASH_SectorErase(u32 Sectors);
void Internal_FLASH_WordWrite(u32 Address, u32 Data) ;
void Internal_FLASH_WritePrConfig(u32 Xsectors, FunctionalState NewState);
#endif

u32  Internal_FLASH_SectorMask(u32 Address);

void External_FLASH_SectorErase(u32 Address);
void External_FLASH_WordWrite(u32 Address, u32 Data);
/* External variables --------------------------------------------------------*/

#endif  /*__HW_CONFIG_H*/

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
