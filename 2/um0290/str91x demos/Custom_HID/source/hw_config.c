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
#include "91x_lib.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

u32 ADC_ConvertedValueX = 0;
u32 ADC_ConvertedValueX_1 = 0;

/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : Set_System.
* Description    : Configures Main system clocks & power
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_System(void)
{
  SCU_MCLKSourceConfig(SCU_MCLK_OSC);
  FMI_Config(FMI_READ_WAIT_STATE_2, FMI_WRITE_WAIT_STATE_0, FMI_PWD_ENABLE, \
             FMI_LVD_ENABLE, FMI_FREQ_HIGH);
  SCU_PLLFactorsConfig(192, 25, 2);
  SCU_PLLCmd(ENABLE);
  SCU_MCLKSourceConfig(SCU_MCLK_PLL);

  SCU_AHBPeriphClockConfig(__VIC, ENABLE);
  SCU_AHBPeriphReset(__VIC, DISABLE);
  /*USB clock = MCLK/2= 48MHz*/
  SCU_USBCLKConfig(SCU_USBCLK_MCLK2);
  /*Enable USB clock*/
  SCU_AHBPeriphClockConfig(__USB, ENABLE);
  SCU_AHBPeriphReset(__USB, DISABLE);

  SCU_AHBPeriphClockConfig(__USB48M, ENABLE);

  /* Enable and configure the priority of the USB_LP IRQ Channel*/
  VIC_DeInit();
  
  /* Configure the used GPIOs*/
  GPIO_Configuration();
  
  /* Configure the EXTIT lines for Key push buttons*/
  EXTIT_Configuration();
  
  /* Configure the ADC*/
  ADC_Configuration();
}
/*******************************************************************************
* Function Name  : USB_Cable_Config
* Description    : Software Connection/Disconnection of USB Cable
* Input          : None.
* Return         : Status
*******************************************************************************/
void USB_Cable_Config (FunctionalState NewState)
{

  if (NewState == ENABLE)
    GPIO_WriteBit(GPIO0, GPIO_Pin_1, Bit_RESET);
  else
    GPIO_WriteBit(GPIO0, GPIO_Pin_1, Bit_SET);

}
/*******************************************************************************
* Function Name  : Enter_LowPowerMode.
* Description    :
* Input          : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void)
{
  /* Set the device state to suspend */
  bDeviceState = SUSPENDED;
}
/*******************************************************************************
* Function Name  : Leave_LowPowerMode.
* Description    :
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
}
/*******************************************************************************
* Function Name  : USB_Interrupts_Config.
* Description    : configure and enable the USB interrupt Lines.
* Input          : None.
* Return         : None.
*******************************************************************************/
void USB_Interrupts_Config(void)
{
  /* Initialize VICs Default vector address registers */
  VIC_InitDefaultVectors();
  
  /* Configure the External interrupt group 3 priority as FIQ interrupt */
  VIC_Config(EXTIT3_ITLine, VIC_FIQ, 0);
  
  /* Enable the External interrupt group 3 */
  VIC_ITCmd(EXTIT3_ITLine, ENABLE);
  
  /* VIC interrupt configuration for ADC */
  VIC_Config(ADC_ITLine, VIC_IRQ, 0);
  VIC_ITCmd(ADC_ITLine, ENABLE);
  
  VIC_Config(USBLP_ITLine, VIC_IRQ, 1);
  VIC_ITCmd(USBLP_ITLine, ENABLE);
}
/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{  
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /*Configure GPIO0 (D+ Pull-Up on P0.1)*/
  SCU_APBPeriphClockConfig(__GPIO0 , ENABLE);
  
  /* GPIO 7 clock source enable */
  SCU_APBPeriphClockConfig(__GPIO7, ENABLE);
  
  /* GPIO 9 clock source enable */
  SCU_APBPeriphClockConfig(__GPIO9 ,ENABLE);
  
  /* GPIO 4 clock source enable */
  SCU_APBPeriphClockConfig(__GPIO6 ,ENABLE);


  GPIO_DeInit(GPIO0);
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull ;
  GPIO_InitStructure.GPIO_Alternate = GPIO_OutputAlt1;
  GPIO_Init (GPIO0, &GPIO_InitStructure);
  
  /* Configure the GPIO4 pin 6 as analog input */
  GPIO_DeInit(GPIO4);
  GPIO_ANAPinConfig(GPIO_ANAChannel6, ENABLE);
  
  /* GPIO pin P7.4 configuration (on the MB460 board, this pin goes low when 
  pressing joystick or Key button)*/

  GPIO_DeInit(GPIO7);
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Direction = GPIO_PinInput;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init (GPIO7, &GPIO_InitStructure);
  
  GPIO_DeInit(GPIO9);
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull ;
  GPIO_Init (GPIO9, &GPIO_InitStructure);
}
/*******************************************************************************
* Function Name : EXTI_Configuration.
* Description   : Configure the EXTI lines for Key and Tamper push buttons.
* Input         : None.
* Output        : None.
* Return value  : The direction value.
*******************************************************************************/
void EXTIT_Configuration(void)
{  
  WIU_InitTypeDef WIU_InitStructure;
    
  /* Enable WIU clock */
  SCU_APBPeriphClockConfig(__WIU, ENABLE);
  
  /* Enable the WIU & Clear the WIU line 28 pending bit */
  WIU_Cmd(ENABLE );
  WIU_ClearITPendingBit(WIU_Line28);

  /* Configure external interrupt line */
  WIU_DeInit();
  WIU_StructInit(&WIU_InitStructure);
  WIU_InitStructure.WIU_Line = WIU_Line28 ;
  WIU_InitStructure.WIU_TriggerEdge = WIU_FallingEdge ;
  WIU_Init(&WIU_InitStructure);

  /* Select WIU line 28 as EXTIT3_ITLine interrupt source */
  SCU_WakeUpLineConfig(28);
}

/*******************************************************************************
* Function Name : ADC_Configuration.
* Description   : Configure the ADC and DMA.
* Input         : None.
* Output        : None.
* Return value  : The direction value.
*******************************************************************************/
void ADC_Configuration(void)
{
  ADC_InitTypeDef   ADC_InitStructure;
  
  /* Enable the clock for the ADC */
  SCU_APBPeriphClockConfig(__ADC, ENABLE); 
  
  /* ADC Structure Initialization */
  ADC_StructInit(&ADC_InitStructure);

  /* Configure the ADC  structure in continuous mode conversion */
  ADC_DeInit();             /* ADC Deinitialization */
  ADC_InitStructure.ADC_Channel_6_Mode = ADC_NoThreshold_Conversion;
  ADC_InitStructure.ADC_Select_Channel = ADC_Channel_6;
  ADC_InitStructure.ADC_Scan_Mode = DISABLE;
  ADC_InitStructure.ADC_Conversion_Mode = ADC_Continuous_Mode;

  /* Enable the ADC */
  ADC_Cmd(ENABLE);

  /* Prescaler config */
  ADC_PrescalerConfig(0x30);

  /* Configure the ADC */
  ADC_Init(&ADC_InitStructure);
    
  /* ADC interrupt config */
  ADC_ITConfig(ADC_IT_ECV, ENABLE);
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
