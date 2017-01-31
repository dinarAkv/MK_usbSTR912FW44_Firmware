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
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_pwr.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u32 ADC_ConvertedValueX = 0;
u32 ADC_ConvertedValueX_1 = 0;
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power.
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
   
  /* Configure the PLL1 ( * 16 , / 2 ) */
  RCCU_PLL1Config (RCCU_PLL1_Mul_12, RCCU_Div_2) ;

  /* Wait PLL to lock */
  while(RCCU_FlagStatus(RCCU_PLL1_LOCK) == RESET);

  /* Select PLL1_Output as RCLK clock */
  RCCU_RCLKSourceConfig (RCCU_PLL1_Output) ;
  
  /* Enable USB clock on APB1 */
  APB_ClockConfig (APB1, ENABLE, USB_Periph);
  
  
  /* Configure the used GPIOs*/
  GPIO_Configuration();
  
  /* Configure the XTI lines for NXT and SEL push buttons*/
  XTI_Configuration();
  
  /* Configure the ADC*/
  ADC_Configuration();
}

/*******************************************************************************
* Function Name  : Set_USBClock
* Description    : Configures USB Clock input (48MHz).
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Set_USBClock(void)
{
  /* Configure the PLL2 ( * 12 , / 1 ) assuming HCLK=4MHz */
  /* RCCU_PLL2Config (RCCU_PLL2_Mul_12,RCCU_Div_1)*/
  /* PCU->PLL2CR  = 0x81D0; */
}

/*******************************************************************************
* Function Name  : Enter_LowPowerMode.
* Description    : Power-off system clocks and power while entering suspend mode.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void)
{
  /* Set the device state to suspend */
  bDeviceState = SUSPENDED;
}

/*******************************************************************************
* Function Name  : Leave_LowPowerMode.
* Description    : Restores system clocks and power while exiting suspend mode.
* Input          : None.
* Output         : None.
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
* Description    : Configures the USB interrupts.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_Interrupts_Config(void)
{
  /*  Set the USBLP IRQ Channel priority to 3 */
  EIC_IRQChannelPriorityConfig(USBLP_IRQChannel, 3);
  EIC_IRQChannelConfig(USBLP_IRQChannel, ENABLE);
  
  /*  Set the XTI IRQ Channel priority to 2 */
  EIC_IRQChannelPriorityConfig(XTI_IRQChannel, 2);
  EIC_IRQChannelConfig(XTI_IRQChannel, ENABLE);

  /*  Set the ADC IRQ Channel priority to 1 */
  EIC_IRQChannelPriorityConfig(ADC_IRQChannel, 1);
  EIC_IRQChannelConfig(ADC_IRQChannel, ENABLE);
  
  /* Enable IRQ interrupts on EIC */
  EIC_IRQConfig(ENABLE);
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
  APB_ClockConfig (APB2, ENABLE, GPIO0_Periph | GPIO1_Periph | GPIO2_Periph);
  
  /* LED configuration */
  GPIO_Config (GPIO2, 0x7800, GPIO_OUT_PP);
  
  /* LED initialization */
  GPIO_ByteWrite(GPIO2, GPIO_MSB, 0x00); 

  /* Configure the P0.15 in IPUPD WP mode (Wake push button)*/
  GPIO_Config(GPIO0, 0x0001 << 0x0F, GPIO_IPUPD_WP);
  
  /* Configure Potentiometer IO as analog input -------------------------*/
  GPIO_Config (GPIO1, 0x0001, GPIO_HI_AIN_TRI);  
}
/*******************************************************************************
* Function Name : XTI_Configuration.
* Description   : Configure the XTI lines for NXT and SEL push buttons.
* Input         : None.
* Output        : None.
* Return value  : The direction value.
*******************************************************************************/
void XTI_Configuration(void)
{
  APB_ClockConfig (APB2, ENABLE, XTI_Periph);
  
  /* Initialize the XTI */
  XTI_Init();

  /* Set Line 15 edge */
  XTI_LineModeConfig(XTI_Line15, XTI_RisingEdge);

  /* Enable the External interrupts on line 15 */
  XTI_LineConfig(XTI_Line15, ENABLE);
  
  /* Set the XTI mode */
  XTI_ModeConfig(XTI_Interrupt, ENABLE);
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
  /* Enable the ADC12 clock */
  APB_ClockConfig (APB2, ENABLE, ADC12_Periph);
  
  /*  Initialize the conveter register */
  ADC12_Init();

  /*  Configure the prescaler register with a sampling frequency = 500Hz */
  ADC12_PrescalerConfig(500);

  /*  Select the conversion mode=single channel */
  ADC12_ModeConfig (ADC12_SINGLE);

  /*  Select the channel to be converted */
  ADC12_ChannelSelect(ADC12_CHANNEL0);

  /* Enable the ADC12 intruupts */
  ADC12_ITConfig (ENABLE);
}

/*******************************************************************************
* Function Name  : ADC12_Unsig_Value
* Description    : This is used for ADC12 result calibration
* Input          : - Conv_Res: Conversion result  12 bit two complement.
* Output         : None.
* Return         : Conversion result calibrated 12 bit unsigned value.
*******************************************************************************/
u16 ADC12_UnsigValue(u16 Conv_Res)
{
  if ((Conv_Res & 0x800) == 0x800)
    return (0x800 - ((~ Conv_Res) & 0xFFF) +1);
  else
    return (Conv_Res + 0x800);
}
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
