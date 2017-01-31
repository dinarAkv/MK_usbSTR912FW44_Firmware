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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u32 MASS_MEMORY_SIZE;
u32 MASS_BLOCK_SIZE;
u32 MASS_BLOCK_COUNT;

/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power
* Input          : None.
* Output         : None.
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
   
  /* Configure the PLL1 ( * 16 , / 2 ) */
  RCCU_PLL1Config (RCCU_PLL1_Mul_12, RCCU_Div_2) ;

  /* Wait PLL to lock */
  while(RCCU_FlagStatus(RCCU_PLL1_LOCK) == RESET);

  /* Select PLL1_Output as RCLK clock */
  RCCU_RCLKSourceConfig (RCCU_PLL1_Output) ;
  
  /* Enable USB clock on APB1 */
  APB_ClockConfig (APB1, ENABLE, USB_Periph);
  
  /* Enable GPIO2 clock on APB2 */
  APB_ClockConfig (APB2, ENABLE, GPIO2_Periph | GPIO0_Periph);

#ifdef _STR710_EVAL
  /* configure P2.1, P2.4, P2.5, P2.6, P2.7 pins corresponding respectively to  
     CS.1, A20, A21, A22, A23 */
  GPIO_Config(GPIO2, 0xF2, GPIO_AF_PP);
  
   /* EMI configuration ------------------------------------------------------*/
   /* Configure EMI bank 1 as follows: 
                             - 16-bit data length
                             - 15 wait states
   */
   EMI_Config (EMI_BANK1, EMI_SIZE_16, EMI_15_WaitStates);
   
   /* Enable EMI bank 1 */
   EMI_Enable (EMI_BANK1, ENABLE);
#endif
   
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
  /* RCCU_PLL2Config (RCCU_PLL2_Mul_12,RCCU_Div_1)*/
  /* PCU->PLL2CR  = 0x81D0; */
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
  
  /*  RCCU->CCR |= RCCU_CKAF_SEL_Mask;
      RCCU->CFR &= ~0x1;                  //PLL not sys clock
      RCCU->PLL1CR &=~ 0x80;              //Free running off
      GPIO_Config(GPIO0, 0x4010, GPIO_OUT_PP);
      GPIO_Config(GPIO1, 0x8400, GPIO_OUT_PP);
      GPIO_WordWrite(GPIO0,0 x0);

      FLASHR->CR0 |= 0x8000;   //disable the flash during the LPWFI
      PCU->PWRCR  |= 0x8000;   //disable MVR during WFI mode
      PCU->PWRCR  |= 0x0010;

      RCCU->CCR  |= 0x2;    //select the 32KHz as peripherl clock during LPWFI
      RCCU->CCR  |= 0x1;
      
      //Disable External oscillator P0.12
      GPIO_Config (GPIO0, 0x1000, GPIO_HI_AIN_TRI);    

      RCCU->SMR &= 0xFE;                               // enter LPWFI*/
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
  /* GPIO_BitWrite(GPIO0, 12, 0);   //Enable External oscillator
     GPIO_Config(GPIO0, 0x1000, GPIO_OUT_PP);

   Set_System();
   Set_USBClock();*/
}
/*******************************************************************************
* Function Name  : USB_Interrupts_Config
* Description    : Configures the USB interrupts
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_Interrupts_Config(void)
{
  EIC_IRQChannelPriorityConfig(USBHP_IRQChannel, 3);
  EIC_IRQChannelPriorityConfig(USBLP_IRQChannel, 4);
  EIC_IRQChannelConfig(USBLP_IRQChannel, ENABLE);
  EIC_IRQChannelConfig(USBHP_IRQChannel, ENABLE);
  EIC_IRQConfig(ENABLE);
}
/*******************************************************************************
* Function Name  : Get_Medium_Characteristics
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Get_Medium_Characteristics(void)
{

#ifdef _STR710_EVAL
  MASS_MEMORY_SIZE = 0x00400000;   /* 4 Mbytes*/
#else
  MASS_MEMORY_SIZE = 0x00008000;   /* 32 Kbytes*/
#endif


  MASS_BLOCK_SIZE = 0x00000200;

  MASS_BLOCK_COUNT   =   (MASS_MEMORY_SIZE / MASS_BLOCK_SIZE);
}
/*******************************************************************************
* Function Name  : Led_Config
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Led_Config(void)
{
  GPIO_Config (GPIO2, 0x0600, GPIO_OUT_PP);
}
/*******************************************************************************
* Function Name  : Led_Power_ON
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Led_Power_ON(void)
{
  GPIO_BitWrite(GPIO2, 10, 0x01);
}
/*******************************************************************************
* Function Name  : Led_Power_OFF
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Led_Power_OFF(void)
{
  GPIO_BitWrite(GPIO2, 10, 0x00);
}
/*******************************************************************************
* Function Name  : Led_RW_ON
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Led_RW_ON(void)
{
  GPIO_BitWrite(GPIO2, 9, 0x01);
}
/*******************************************************************************
* Function Name  : Led_RW_OFF
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Led_RW_OFF(void)
{
  GPIO_BitWrite(GPIO2, 9, 0x00);
}
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/


