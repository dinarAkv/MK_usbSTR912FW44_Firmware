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
#include "75x_lib.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_pwr.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ADC_D3_Base        0xFFFF845C
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ErrorStatus OSC4MStartUpStatus;
u32 ADC_ConvertedValueX = 0;
u32 ADC_ConvertedValueX_1 = 0;
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
  /* MRCC configuration --------------------------------------------------------*/
  MRCC_DeInit();
  
  /* Clear No Clock Detected flag */
  MRCC_ClearFlag(MRCC_FLAG_NCKD);

  /* Enbale No Clock Detected interrupt */
  MRCC_ITConfig(MRCC_IT_NCKD, ENABLE);

  /* Wait for OSC4M start-up */
  OSC4MStartUpStatus = MRCC_WaitForOSC4MStartUp();

  if (OSC4MStartUpStatus == SUCCESS)
  {
    /* Set HCLK to 30 MHz */
    MRCC_HCLKConfig(MRCC_CKSYS_Div2);

    /* Set CKTIM to 30 MHz */
    MRCC_CKTIMConfig(MRCC_HCLK_Div1);

    /* Set PCLK to 30 MHz */
    MRCC_PCLKConfig(MRCC_CKTIM_Div1);

    /* Set CKSYS to 60 MHz */
    MRCC_CKSYSConfig(MRCC_CKSYS_OSC4MPLL, MRCC_PLL_Mul_15);
  }

  /* Enable GPIOs, USB and EXTIT clocks */
  MRCC_PeripheralClockConfig(MRCC_Peripheral_GPIO | MRCC_Peripheral_EXTIT 
                             | MRCC_Peripheral_USB | MRCC_Peripheral_ADC, ENABLE);
  
  /* Configure the used GPIOs*/
  GPIO_Configuration();
  
  /* Configure the EXTI lines for Key and Tamper push buttons*/
  EXTI_Configuration();
  
  /* Configure the ADC*/
  ADC_Configuration();
}

/*******************************************************************************
* Function Name  : Set_USBClock
* Description    : Configures USB Clock input (48MHz)
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_USBClock(void)
{
  /* Set USB kernel clock to 48 MHz */
  MRCC_CKUSBConfig (MRCC_CKUSB_Internal);
  CFG_USBFilterConfig(CFG_USBFilter_Enable);
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
  
  /*    GPIO_WriteBit(GPIO2, GPIO_Pin_19, (BitAction)(1));

      EXTIT_ClearITPendingBit(EXTIT_ITLine14);


      // Enter STOP mode with OSC4M off, FLASH off and MVREG off
       MRCC_EnterSTOPMode(MRCC_STOPParam_MVREGOff);

      //Clear LP_DONE flag
       MRCC_ClearFlag(MRCC_FLAG_LPDONE);*/
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
  
  /* EXTIT_ClearITPendingBit(EXTIT_ITLine14);

   // wake up and enable all clocks in STR75x
   //WaitForOSC4MStartUp();

    // Clear No Clock Detected flag
    MRCC_ClearFlag(MRCC_FLAG_NCKD);
    // Set CKSYS to 60MHz
    MRCC_CKSYSConfig(MRCC_CKSYS_OSC4MPLL, MRCC_PLL_Mul_15);

    Set_USBClock();*/
}

/*******************************************************************************
* Function Name  : USB_Interrupts_Config
* Description    : Configures the USB interrupts
* Input          : None.
* Return         : None.
*******************************************************************************/
void USB_Interrupts_Config(void)
{
  EIC_IRQInitTypeDef  EIC_IRQInitStructure;

  /* Enable and configure the priority of the USB_LP IRQ Channel*/
  EIC_IRQInitStructure.EIC_IRQChannel = USB_LP_IRQChannel;
  EIC_IRQInitStructure.EIC_IRQChannelPriority = 2;
  EIC_IRQInitStructure.EIC_IRQChannelCmd = ENABLE;
  EIC_IRQInit(&EIC_IRQInitStructure);
  
  /* Enable and configure the priority of the EXTIT IRQ Channel*/
  EIC_IRQInitStructure.EIC_IRQChannel = EXTIT_IRQChannel;
  EIC_IRQInitStructure.EIC_IRQChannelPriority = 1;
  EIC_IRQInitStructure.EIC_IRQChannelCmd = ENABLE;
  EIC_IRQInit(&EIC_IRQInitStructure);

  /* Enable and configure the priority of the DMA IRQ Channel*/
  EIC_IRQInitStructure.EIC_IRQChannel = ADC_IRQChannel;
  EIC_IRQInitStructure.EIC_IRQChannelPriority = 1;
  EIC_IRQInitStructure.EIC_IRQChannelCmd = ENABLE;
  EIC_IRQInit(&EIC_IRQInitStructure);
  
  /* Enable the Interrupt controller to manage IRQ channel*/
  EIC_IRQCmd(ENABLE);
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
  {
    GPIO_WriteBit(GPIO0, GPIO_Pin_9, (BitAction)(0));
  }
  else
  {
    GPIO_WriteBit(GPIO0, GPIO_Pin_9, (BitAction)(1));
  }
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
  GPIO_InitTypeDef    GPIO_InitStructure;

  /* Configure P2.18 (USB Suspend/Resume) as Output push-pull */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_19;
  GPIO_Init(GPIO2, &GPIO_InitStructure);

  GPIO_WriteBit(GPIO2, GPIO_Pin_19, (BitAction)(0));

  /* Configure P0.9 (D+ Pull up)  as Output push-pull */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_Init(GPIO0, &GPIO_InitStructure);
  
  /* Configure GPIO1 pin 5 in input mode */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_15;
  GPIO_Init(GPIO1, &GPIO_InitStructure);
  
  /* ADC Channel3 pin configuration */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_17 ;
  GPIO_Init(GPIO0, &GPIO_InitStructure);
  
  /* Configure the LED2, LED3, LED4 & LED5 IOs as Output PP -------------*/
  
  /* Configure P2.18 and P2.19 as output push-pull */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_18 | GPIO_Pin_19;
  GPIO_Init(GPIO2, &GPIO_InitStructure);

  /* Configure P1.01 as output push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;
  GPIO_Init(GPIO1, &GPIO_InitStructure);

  /* Configure P0.16 as output push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_16;
  GPIO_Init(GPIO0, &GPIO_InitStructure);

  /* Mask the unused pins */
  GPIO_PinMaskConfig(GPIO2, ~(GPIO_Pin_18 | GPIO_Pin_19), ENABLE);
  GPIO_PinMaskConfig(GPIO1, ~GPIO_Pin_1, ENABLE);
  GPIO_PinMaskConfig(GPIO0, ~GPIO_Pin_16, ENABLE);
  
  GPIO_Write(GPIO2, 0xFFFFF);
  GPIO_Write(GPIO1, 0xFFFFF);
  GPIO_Write(GPIO0, 0xFFFFFFFF);
  
}
/*******************************************************************************
* Function Name : EXTI_Configuration.
* Description   : Configure the EXTI lines for Key and Tamper push buttons.
* Input         : None.
* Output        : None.
* Return value  : The direction value.
*******************************************************************************/
void EXTI_Configuration(void)
{
  EXTIT_InitTypeDef   EXTIT_InitStructure;
  
  /* Clear EXTIT line 7 pending bit  */
  EXTIT_ClearITPendingBit(EXTIT_ITLine7);

  /* Enable EXTIT line 7 on falling edge */
  EXTIT_InitStructure.EXTIT_ITLine = EXTIT_ITLine7;
  EXTIT_InitStructure.EXTIT_ITTrigger = EXTIT_ITTrigger_Falling;
  EXTIT_InitStructure.EXTIT_ITLineCmd = ENABLE;
  EXTIT_Init(&EXTIT_InitStructure);
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
    
  /* ADC configuration -------------------------------------------------------*/ 
  ADC_InitStructure.ADC_ConversionMode = ADC_ConversionMode_Scan;
  ADC_InitStructure.ADC_ExtTrigger = ADC_ExtTrigger_Disable;
  ADC_InitStructure.ADC_AutoClockOff = ADC_AutoClockOff_Disable;
  ADC_InitStructure.ADC_SamplingPrescaler = 0;
  ADC_InitStructure.ADC_ConversionPrescaler = 0;
  ADC_InitStructure.ADC_FirstChannel = ADC_CHANNEL3;
  ADC_InitStructure.ADC_ChannelNumber = 1;
  ADC_Init(&ADC_InitStructure);

  ADC_ITConfig(ADC_IT_EOC, ENABLE);
    
  /* Enable ADC */
  ADC_Cmd(ENABLE);
  
  /* Start calibration */
  ADC_StartCalibration(ADC_CalibAverage_Enable);
  
}
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
