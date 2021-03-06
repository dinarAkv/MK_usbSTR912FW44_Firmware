/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : stm32f10x_it.c
* Author             : MCD Application Team
* Version            : V2.0.0
* Date               : 09/29/2008
* Description        : Main Interrupt Service Routines.
*                      This file can be used to describe all the exceptions
*                      subroutines that may occur within user application.
*                      When an interrupt happens, the software will branch
*                      automatically to the corresponding routine.
*                      The following routines are all empty, user can write code
*                      for exceptions handlers and peripherals IRQ interrupts.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "91x_it.h"
#include "91x_lib.h"
#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_pwr.h"
#include "hw_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u16 Pulse = 0;

/* Extern variables ----------------------------------------------------------*/
extern u8 Stream_Buff[80];
extern u16 In_Data_Offset;
extern u16 Out_Data_Offset;
extern u32 MUTE_DATA;
extern void USB_Istr(void);

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : Undefined_Handler
* Description    : This function Undefined instruction exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Undefined_Handler(void)
{
  while(1)
  {
  	/* infinite loop */
  }
 }
/*******************************************************************************
* Function Name  : SWI_Handler
* Description    : This function handles SW exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SWI_Handler(void)
{
}
/*******************************************************************************
* Function Name  : Prefetch_Handler
* Description    : This function handles preftetch abort exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Prefetch_Handler(void)
{
  while(1)
  {
  	/* infinite loop */
  }
}
/*******************************************************************************
* Function Name  : Abort_Handler
* Description    : This function handles data abort exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Abort_Handler(void)
{
  while(1)
  {
  	/* infinite loop */
  }
}
/*******************************************************************************
* Function Name  : FIQ_Handler
* Description    : This function handles FIQ exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void FIQ_Handler(void)
{
   /*write your handler here*/
   /* ... */
}
/*******************************************************************************
* Function Name  : WDG_IRQHandler
* Description    : This function handles the WDG interrupt request
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void WDG_IRQHandler(void)
{
   /*write your handler here*/
   /* ... */
           
   
   /*write any value to VIC0 VAR*/  
   VIC0->VAR = 0xFF;
}
/*******************************************************************************
* Function Name  : SW_IRQHandler
* Description    : This function handles the SW interrupt request
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SW_IRQHandler(void)
{
   /*write your handler here*/
   /* ... */


   /*write any value to VIC0 VAR*/  
   VIC0->VAR = 0xFF;
}
/*******************************************************************************
* Function Name  : ARMRX_IRQHandler
* Description    : This function handles the ARMRX interrupt request
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ARMRX_IRQHandler(void)
{
   /*write your handler here*/
   /* ... */
           
   
   /*write any value to VIC0 VAR*/  
   VIC0->VAR = 0xFF;
    
}
/*******************************************************************************
* Function Name  : ARMTX_IRQHandler
* Description    : This function handles the ARMTX interrupt request
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ARMTX_IRQHandler(void)
{
   /*write your handler here*/
   /* ... */
           
   
   /*write any value to VIC0 VAR*/  
   VIC0->VAR = 0xFF;
  
}
/*******************************************************************************
* Function Name  : TIM0_IRQHandler
* Description    : This function handles the TIM0 interrupt request
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM0_IRQHandler(void)
{
  Pulse += TIMER_PULSE_LENGTH_22KHZ;

  if ((Out_Data_Offset < In_Data_Offset) && ((u8)(MUTE_DATA) == 0))
  {
    /* Play the the Audio Stream */
    TIM_SetPulse(TIM3, TIM_PWM_OC1_Channel, Stream_Buff[Out_Data_Offset]);
    Out_Data_Offset++;
  }
  /* Set the TIM0 OC1 value */
  TIM_SetPulse(TIM0, TIM_PWM_OC1_Channel, Pulse);
  /* Clear the TIM OC1 flag */
  TIM_ClearFlag(TIM0, TIM_FLAG_OC1);           
   
  /*write any value to VIC0 VAR*/  
  VIC0->VAR = 0xFF;
}
/*******************************************************************************
* Function Name  : TIM1_IRQHandler
* Description    : This function handles the TIM1 interrupt request
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_IRQHandler(void)
{
   /*write your handler here*/
   /* ... */
           
   
   /*write any value to VIC0 VAR*/  
   VIC0->VAR = 0xFF;
}
/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : This function handles the TIM2 interrupt request
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM2_IRQHandler(void)
{
   /*write your handler here*/
   /* ... */
           
   
   /*write any value to VIC0 VAR*/  
   VIC0->VAR = 0xFF;
}
/*******************************************************************************
* Function Name  : TIM3_IRQHandler
* Description    : This function handles the TIM3 interrupt request
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM3_IRQHandler(void)
{
   /*write your handler here*/
   /* ... */
           
   
   /*write any value to VIC0 VAR*/  
   VIC0->VAR = 0xFF;
}
/*******************************************************************************
* Function Name  : USBHP_IRQHandler
* Description    : This function handles the USBHP interrupt request
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USBHP_IRQHandler(void)
{
   /*write your handler here*/
   /* ... */
   CTR_HP();         
   
   /*write any value to VIC0 VAR*/  
   VIC0->VAR = 0xFF;
}
/*******************************************************************************
* Function Name  : USBLP_IRQHandler
* Description    : This function handles the USBLP interrupt request
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USBLP_IRQHandler(void)
{
  USB_Istr();
  
  /*write any value to VIC0 VAR*/  
  VIC0->VAR = 0xFF;
}
/*******************************************************************************
* Function Name  : SCU_IRQHandler
* Description    : This function handles the SCU interrupt request
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SCU_IRQHandler(void)
{
   /*write your handler here*/
   /* ... */
           
   
   /*write any value to VIC0 VAR*/  
   VIC0->VAR = 0xFF;
}
/*******************************************************************************
* Function Name  : ENET_IRQHandler
* Description    : This function handles the DENET interrupt request
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ENET_IRQHandler(void)
{
   /*write your handler here*/
   /* ... */
           
   
   /*write any value to VIC0 VAR*/  
   VIC0->VAR = 0xFF;
}
/*******************************************************************************
* Function Name  : DMA_IRQHandler
* Description    : This function handles the DMA interrupt request
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA_IRQHandler(void)
{
   /*write your handler here*/
   /* ... */
           
   
   /*write any value to VIC0 VAR*/  
   VIC0->VAR = 0xFF;
}
/*******************************************************************************
* Function Name  : CAN_IRQHandler
* Description    : This function handles the CAN interrupt request
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAN_IRQHandler(void)
{
   /*write your handler here*/
   /* ... */
           
   
   /*write any value to VIC0 VAR*/  
   VIC0->VAR = 0xFF;
}
/*******************************************************************************
* Function Name  : MC_IRQHandler
* Description    : This function handles the MC interrupt request
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MC_IRQHandler(void)
{
   /*write your handler here*/
   /* ... */
           
   
   /*write any value to VIC0 VAR*/  
   VIC0->VAR = 0xFF;
}
/*******************************************************************************
* Function Name  : ADC_IRQHandler
* Description    : This function handles the ADC interrupt request
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADC_IRQHandler(void)
{
   /*write your handler here*/
   /* ... */
           
   
   /*write any value to VIC0 VAR*/  
   VIC0->VAR = 0xFF;
}
/*******************************************************************************
* Function Name  : UART0_IRQHandler
* Description    : This function handles the UART0 interrupt request
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART0_IRQHandler(void)
{
   /*write your handler here*/
   /* ... */	
    
}
/*******************************************************************************
* Function Name  : UART1_IRQHandler
* Description    : This function handles the UART1 interrupt request
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART1_IRQHandler(void)
{
   /*write your handler here*/
   /* ... */
    
}
/*******************************************************************************
* Function Name  : UART2_IRQHandler
* Description    : This function handles the UART2 interrupt request
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART2_IRQHandler(void)
{
   /*write your handler here*/
   /* ... */
   
}
/*******************************************************************************
* Function Name  : I2C0_IRQHandler
* Description    : This function handles the I2C0 interrupt request
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C0_IRQHandler(void)
{
   /*write your handler here*/
   /* ... */
    
}
/*******************************************************************************
* Function Name  : I2C1_IRQHandler
* Description    : This function handles the I2C1 interrupt request
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C1_IRQHandler(void)
{
   /*write your handler here*/
   /* ... */
    
}
/*******************************************************************************
* Function Name  : SSP0_IRQHandler
* Description    : This function handles the SSP0 interrupt request
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SSP0_IRQHandler(void)
{
   /*write your handler here*/
   /* ... */
    
}
/*******************************************************************************
* Function Name  : SSP1_IRQHandler
* Description    : This function handles the SSP1 interrupt request
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SSP1_IRQHandler(void)
{
   /*write your handler here*/
   /* ... */
   
}
/*******************************************************************************
* Function Name  : LVD_IRQHandler
* Description    : This function handles the LVD interrupt request
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LVD_IRQHandler(void)
{
   /*write your handler here*/
   /* ... */
    
}
/*******************************************************************************
* Function Name  : RTC_IRQHandler
* Description    : This function handles the RTC interrupt request
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_IRQHandler(void)
{
   /*write your handler here*/
   /* ... */
   
}
/*******************************************************************************
* Function Name  : WIU_IRQHandler
* Description    : This function handles the WIU interrupt request
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void WIU_IRQHandler(void)
{

   /*write your handler here*/
   /* ... */
    
}
/*******************************************************************************
* Function Name  : EXTIT0_IRQHandler
* Description    : This function handles the EXTIT0 interrupt request
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTIT0_IRQHandler(void)
{
   /*write your handler here*/
   /* ... */
    
}
/*******************************************************************************
* Function Name  : EXTIT1_IRQHandler
* Description    : This function handles the EXTIT1 interrupt request
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTIT1_IRQHandler(void)
{
   /*write your handler here*/
   /* ... */
    
}
/*******************************************************************************
* Function Name  : EXTIT2_IRQHandler
* Description    : This function handles the EXTIT2 interrupt request
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTIT2_IRQHandler(void)
{
   /*write your handler here*/
   /* ... */
    
}
/*******************************************************************************
* Function Name  : EXTIT3_IRQHandler
* Description    : This function handles the EXTIT3 interrupt request
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTIT3_IRQHandler(void)
{ 
   /*write your handler here*/
   /* ... */
  
}
/*******************************************************************************
* Function Name  : USBWU_IRQHandler
* Description    : This function handles the USBWU interrupt request
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USBWU_IRQHandler(void)
{ 
   /*write your handler here*/
   /* ... */
    
}
/*******************************************************************************
* Function Name  : PFQBC_IRQHandler
* Description    : This function handles the PFQBC interrupt request
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PFQBC_IRQHandler(void)
{
   /*write your handler here*/
   /* ... */
    
}

/*******************************************************************************
* Function Name  : Dummy_Handler
* Description    : This function is used for handling a case of spurious interrupt
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DefaultVector_Handler(void)
{
    /* Write any value to VICs	*/
    VIC0->VAR = 0xFF;
    VIC1->VAR = 0xFF;	
}
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
