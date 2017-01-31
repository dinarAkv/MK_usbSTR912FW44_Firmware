/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : 91x_it.c
* Author             : MCD Application Team
* Version            : V2.0.0
* Date               : 09/29/2008
* Description        : Main Interrupt Service Routines
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
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 Send_Buffer[2];
WIU_InitTypeDef WIU_InitStructure;

/* Extern variables ----------------------------------------------------------*/
extern u32 ADC_ConvertedValueX;
extern u32 ADC_ConvertedValueX_1;
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
  Send_Buffer[0] = 0x05;
    
  WIU_InitStructure.WIU_Line = WIU_Line28 ;

  WIU_Init(&WIU_InitStructure);
  
  if (GPIO_ReadBit(GPIO7, GPIO_Pin_4) == Bit_RESET)
  {
    WIU_InitStructure.WIU_TriggerEdge = WIU_RisingEdge;
    WIU_Init(&WIU_InitStructure);
    Send_Buffer[1] = 0x01;
  }
  else 
  {
    WIU_InitStructure.WIU_TriggerEdge = WIU_FallingEdge;
    WIU_Init(&WIU_InitStructure);
    Send_Buffer[1] = 0x00;
  }
    
  UserToPMABufferCopy(Send_Buffer, ENDP1_TXADDR, 2);
  SetEPTxCount(ENDP1, 2);
  SetEPTxValid(ENDP1);
    
  /* Clear the WIU line 28 pending bit */
  WIU_ClearITPendingBit(WIU_Line28);
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
   /*write your handler here*/
   /* ... */
           
   
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
  /*Get the converted Value*/
  ADC_ConvertedValueX = ADC_GetConversionValue(ADC_Channel_6);
  
  Send_Buffer[0] = 0x07;
  
  if((ADC_ConvertedValueX >> 2) - (ADC_ConvertedValueX_1 >> 2) > 4)
  {
    Send_Buffer[1] = (u8)(ADC_ConvertedValueX >> 2);
    UserToPMABufferCopy(Send_Buffer, ENDP1_TXADDR, 2);
    SetEPTxCount(ENDP1, 2);
    SetEPTxValid(ENDP1);
    ADC_ConvertedValueX_1 = ADC_ConvertedValueX;
  }
 
  /* Clear the end of conversion flag */
  ADC_ClearFlag(ADC_FLAG_ECV);
        
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
