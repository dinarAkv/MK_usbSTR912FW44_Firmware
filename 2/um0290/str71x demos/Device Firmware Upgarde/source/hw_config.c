/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : hw_config.c
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

/* Includes ------------------------------------------------------------------*/
#include "71x_lib.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "usb_desc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FLASH_SPR_MASK     0x01000000
#define FLASH_WPG_MASK     0x20000000
#define FLASH_WMS_MASK     0x80000000
#define FLASH_SER_MASK     0x08000000


#define FLASH_BASE_ADDRESS      0x40100000
#define FLASH_NVWPAR_ADDRESS   (FLASH_BASE_ADDRESS + 0xDFB0)

#define FLASH           ((FLASH_TypeDef *)FLASH_BASE_ADDRESS)
/* STR71x Flash lock and busy bits */
#define FLASH_FLAG_LOCKBSY     0x16

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_System(void)
{
  /* Configure PCLK1 = RCLK / 2 */
  RCCU_PCLK1Config (RCCU_RCLK_2);

  /* Configure PCLK2 = RCLK / 2 */
  RCCU_PCLK2Config (RCCU_RCLK_2);

  /* Configure MCLK clock for the CPU, RCCU_DEFAULT = RCLK /1 */
  RCCU_MCLKConfig (RCCU_DEFAULT);
   
  /* Configure the PLL1 ( * 12 , / 2 ) */
  RCCU_PLL1Config (RCCU_PLL1_Mul_12, RCCU_Div_2) ;

  /* Wait PLL to lock */
  while(RCCU_FlagStatus(RCCU_PLL1_LOCK) == RESET);

  /* Select PLL1_Output as RCLK clock */
  RCCU_RCLKSourceConfig (RCCU_PLL1_Output) ;
  
  /* Enable USB clock on APB1 */
  APB_ClockConfig (APB1, ENABLE, USB_Periph); 
  
  /* Enable GPIO2 clock on APB2 */
   APB_ClockConfig (APB2, ENABLE, GPIO2_Periph | GPIO1_Periph | GPIO0_Periph); 
}

/*******************************************************************************
* Function Name  : Set_USBClock
* Description    : Configures USB Clock input (48MHz)
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_USBClock(void)
{
  /* Configure the PLL2 ( * 12 , / 1 ) assuming HCLK=4MHz */

  /* RCCU->PER |= ~0x0010;
   PCU->PLL2CR  = 0x01D0;
   while(!(PCU->PLL2CR & 0x8000));  //Waiting PLL2 Locking */

}

/*******************************************************************************
* Function Name  : Enter_LowPowerMode
* Description    : Power-off system clocks and power while entering suspend mode
* Input          : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void)
{
  /* Set the device state to suspend */
  bDeviceState = SUSPENDED;
  
  /*  RCCU->CCR  |= RCCU_CKAF_SEL_Mask;
    RCCU->CFR    &=~0x1;                  //PLL not sys clock
    RCCU->PLL1CR &=~0x80;                 //Free running off
    GPIO_Config(GPIO0,0x4010,GPIO_OUT_PP);
    GPIO_Config(GPIO1,0x8400,GPIO_OUT_PP);
    GPIO_WordWrite(GPIO0,0x0);

    FLASHR->CR0|=0x8000;    //disable the flash during the LPWFI
    PCU->PWRCR  |=0x8000;   //disable MVR during WFI mode
    PCU->PWRCR  |=0x0010;

    RCCU->CCR  |=0x2;       //select the 32KHz as peripherl clock during LPWFI
    RCCU->CCR  |=0x1;

    //Disable External oscillator P0.12
    GPIO_Config (GPIO0, 0x1000,GPIO_HI_AIN_TRI);    
    RCCU->SMR &=0xFE;       // enter LPWFI*/
}

/*******************************************************************************
* Function Name  : Leave_LowPowerMode
* Description    : Restores system clocks and power while exiting suspend mode
* Input          : None.
* Return         : None.
*******************************************************************************/
void Leave_LowPowerMode(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  /* Set the device state to the correct state */
  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
  else
  {
    bDeviceState = ATTACHED;
  }
  /* GPIO_BitWrite(GPIO0, 12, 0);   */       /*Enable External oscillator*/
  /* GPIO_Config(GPIO0, 0x1000, GPIO_OUT_PP);

   Set_System();
   Set_USBClock();*/
}

/*******************************************************************************
* Function Name  : DFU_Button_Config
* Description    : Configures the DFU selector Button to enter DFU Mode
* Input          : None.
* Return         : None.
*******************************************************************************/
void DFU_Button_Config(void)
{
  /* GPIO_Config (GPIO1, 0x0100, GPIO_IN_TRI_CMOS) */
  /* P1.4 is configured as input to Enter DFU Mode */
  /* P1.4 pin = "SEL" button on the STR710-EVAL    */
   APB_ClockConfig (APB2, ENABLE, GPIO1_Periph);
  GPIO1->PC0 &= ~0x0100;
  GPIO1->PC1 |= 0x0100;
  GPIO1->PC2 &= ~0x0100;

  GPIO_Config(GPIO0, 0xFFFF, GPIO_OUT_PP);
  GPIO_WordWrite(GPIO0, 0x0);

}

/*******************************************************************************
* Function Name  : DFU_Button_Read
* Description    : Reads the DFU selector Button to enter DFU Mode
* Input          : None.
* Return         : Status
*******************************************************************************/
u32 DFU_Button_Read (void)
{
  /* Return the status of the P1.4 pin = "SEL" button on the STR710-EVAL*/
  return GPIO_BitRead(GPIO1, 8) ;

}

/*******************************************************************************
* Function Name  : USB_Interrupts_Config
* Description    : Configures the USB interrupts
* Input          : None.
* Return         : None.
*******************************************************************************/
void USB_Interrupts_Config(void)
{
  /* Configuration of the USB Resume Event using the XTI */
  /*XTI_Init();
  XTI_LineModeConfig(XTI_Line1,  XTI_RisingEdge);
  XTI_LineConfig(XTI_Line1, ENABLE);
  XTI_ModeConfig(XTI_WakeUpInterrupt, ENABLE);
  EIC_IRQChannelPriorityConfig(XTI_IRQChannel,4);
  EIC_IRQChannelConfig(XTI_IRQChannel,ENABLE);*/
  /* Configuration of the USB Interrupts  */
  EIC_IRQChannelPriorityConfig(USBLP_IRQChannel, 2);
  EIC_IRQChannelConfig(USBLP_IRQChannel, ENABLE);
  EIC_IRQConfig(ENABLE);
}

/*******************************************************************************
* Function Name  : Reset_Device
* Description    : Reset the device using the watchdog Reset
* Input          : None.
* Return         : None.
*******************************************************************************/
void Reset_Device(void)
{
  WDG_PrescalerConfig (0x80);
  WDG_CntReloadUpdate (0x07FF); /* Start WatchDog Timer counter */
  WDG_Enable ();
}

/*******************************************************************************
* Function Name  : External_FLASH_SectorErase
* Description    : Erases a sector in the external Flash (M28W320FCB)
* Input 1        : Address of the Destination.
* Return         : None.
*******************************************************************************/
void External_FLASH_SectorErase(u32 Address)
{
  * (vu16 *) (Address) = 0x60;
  * (vu16 *) (Address) = 0xD0;
  * (vu16 *) (Address) = 0x20;
  * (vu16 *) (Address) = 0xD0;
  while (((*(vu16 *)Address) & 0x80) != 0x80);
  * (vu16 *) (Address ) = 0x50;
  * (vu16 *) (Address) = 0xFF;
}

/*******************************************************************************
* Function Name  : External_FLASH_WordWrite
* Description    : Writes a Word in the external Flash (M28W320FCB)
* Input 1        : Address of the Destination.
* Input 2        : Word to program at Address.
* Return         : None.
*******************************************************************************/
void External_FLASH_WordWrite(u32 Address, u32 Data)
{
  * (vu16 *) (Address ) = 0x50;
  * (vu16 *) (Address ) = 0x30 ;
  * (vu32 *) (Address  ) = Data;
  while (((*(vu16 *)Address) & 0x80) != 0x80)
  {
  }
  /* Clear status register*/
  * (vu16 *) (Address ) = 0x50;
}

/*******************************************************************************
* Function Name  : Internal_FLASH_WordWrite
* Description    : Writes a Word in the Flash (To be executed from RAM)
* Input 1        : Address of the Destination.
* Input 2        : Word to program at Address.
* Return         : None.
*******************************************************************************/
#ifdef __TASKING__          /* Ram function remaping for TASKING compiler*/
#pragma section_code_init on
#endif

RAM_exe void Internal_FLASH_WordWrite(u32 FLASH_Address, u32 FLASH_Data)
{
  /* Set the word programming bit */
  FLASH->CR0 |= FLASH_WPG_MASK;
  /* Load the destination address */
  FLASH->AR   = FLASH_Address;
  /* Load DATA to be programmed */
  FLASH->DR0  = FLASH_Data;
  /* Start the operation */
  FLASH->CR0 |= FLASH_WMS_MASK;
  
  /* Wait until the Flash controller acknowledges the end of the last
  operation resetting the BSYAs and LOCK bits in the CR0 register */
  while((FLASH->CR0 & FLASH_FLAG_LOCKBSY) != RESET);
}
#ifdef __TASKING__
#pragma section_code_init restore
#endif
/*******************************************************************************
* Function Name  : Internal_FLASH_SectorErase
* Description    : Erases a Flash sector (To be executed from RAM)
* Input 1        : Sectors to be Erased.
* Return         : None.
*******************************************************************************/
#ifdef __TASKING__          /* Ram function remaping for TASKING compiler*/
#pragma section_code_init on
#endif

RAM_exe  void Internal_FLASH_SectorErase(u32 FLASH_Sectors)
{
  /* Set the sector erase bit */
  FLASH->CR0 |= FLASH_SER_MASK;
  /* Select the sectors to be erased */
  FLASH->CR1 |= FLASH_Sectors;
  /* Start the operation */
  FLASH->CR0 |= FLASH_WMS_MASK;
  
  /* Wait until the Flash controller acknowledges the end of the last
  operation resetting the BSYAs and LOCK bits in the CR0 register */
  while((FLASH->CR0 & FLASH_FLAG_LOCKBSY) != RESET);
}
#ifdef __TASKING__
#pragma section_code_init restore
#endif
/*******************************************************************************
* Function Name  : FLASH_WritePrConfig
* Description    : Enable Write protection or Disable temporarily Write
*                  protection of a flash sector. (To be executed from RAM)
* Input 1        : Flash Sector.
* Input 2        : Enable or disable Flash sector Write Protection.
* Return         : None.
*******************************************************************************/
#ifdef __TASKING__          /* Ram function remaping for TASKING compiler*/
#pragma section_code_init on
#endif
RAM_exe void Internal_FLASH_WritePrConfig(u32 FLASH_Sectors, FunctionalState FLASH_NewState)
{
  if (FLASH_NewState == ENABLE)
  {
    /* Set the set protection bit */
    FLASH->CR0 |= FLASH_SPR_MASK;
    /* Set the write protection register address */
    FLASH->AR  = FLASH_NVWPAR_ADDRESS;
    /* Data to be programmed to the protection register */
    FLASH->DR0  = (*(u32*)FLASH_NVWPAR_ADDRESS) & (~FLASH_Sectors);
    /* Start the sequence */
    FLASH->CR0 |= FLASH_WMS_MASK;
  }
  else  /* DISABLE */
  {
    /* Set the set protection bit */
    FLASH->CR0 |= FLASH_SPR_MASK;
    /* Set the write protection register address */
    FLASH->AR  = FLASH_NVWPAR_ADDRESS;
    /* Data to be programmed to the protection register */
    FLASH->DR0  = (*(u32*)FLASH_NVWPAR_ADDRESS) | FLASH_Sectors;
    /* Start the sequence */
    FLASH->CR0 |= FLASH_WMS_MASK;
  }
  
  /* Wait until the Flash controller acknowledges the end of the last
  operation resetting the BSYAs and LOCK bits in the CR0 register */
  while((FLASH->CR0 & FLASH_FLAG_LOCKBSY) != RESET);
}
#ifdef __TASKING__
#pragma section_code_init restore
#endif
/*******************************************************************************
* Function Name  : Internal_FLASH_SectorMask
* Description    : Selects the internal Flash sector mask
* Input          : Address
* Return         : Sector mask
*******************************************************************************/
u32 Internal_FLASH_SectorMask(u32 Address)
{
  u32 ReturnValue = 0;
  switch (Address)
  {
    case 0x40000000:
      ReturnValue = 0x01;
      break;
    case 0x40002000:
      ReturnValue = 0x02;
      break;
    case 0x40004000:
      ReturnValue = 0x04;
      break;
    case 0x40006000:
      ReturnValue = 0x08;
      break;
    case 0x40008000:
      ReturnValue = 0x10;
      break;
    case 0x40010000:
      ReturnValue = 0x20;
      break;
    case 0x40020000:
      ReturnValue = 0x40;
      break;
    case 0x40030000:
      ReturnValue = 0x80;
      break;
    case 0x400C0000:
      ReturnValue = 0x10000;
      break;
    case 0x400C2000:
      ReturnValue = 0x20000;
      break;
    default :
      ReturnValue = 0;
      break;
  }
  return (ReturnValue);
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
