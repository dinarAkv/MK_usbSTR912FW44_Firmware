/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V2.1
* Date               : 12/22/2008
* Description        : This file contains the firmware implementation for the
*                      MC firmware library verification.
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
* CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS
* A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
* OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
* OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
* CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "91x_lib.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
MC_InitTypeDef MC_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;

/* Private function prototypes -----------------------------------------------*/
void SCU_Configuration(void);

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{

#ifdef DEBUG
    debug();
#endif

  
  /* Configure the system clocks */
   SCU_Configuration(); 
  
/* GPIO Configuration */
  GPIO_DeInit(GPIO6);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2 | GPIO_Pin_3 | 
  GPIO_Pin_4 | GPIO_Pin_5 ;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull;
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Alternate=GPIO_OutputAlt3;
  GPIO_Init(GPIO6, &GPIO_InitStructure);


/* MC deinitialization */  
  MC_DeInit();

/* MC Configuration: The MC is running in Hardware complementary operating mode
   to provide three complementary output signals with respective duty cycle: 
   50%, 25% and 12.5%. */  
    
/* Channel U configuration */
  MC_InitStructure.MC_OperatingMode = MC_HardwareOperating_Mode;
  MC_InitStructure.MC_TachoMode = MC_TachoContinuous_Mode;
  MC_InitStructure.MC_TachoEvent_Mode = MC_TachoEvent_Hardware_Mode;
  MC_InitStructure.MC_TachoPrescaler = 0x00;
  MC_InitStructure.MC_TachoPeriod = 0xFF;
  MC_InitStructure.MC_TachoPolarity = MC_TachoEventEdge_Falling;
  MC_InitStructure.MC_Prescaler = 0x02;
  MC_InitStructure.MC_PWMMode = MC_PWMClassical_Mode;
  MC_InitStructure.MC_Complementary = MC_Complementary_Enable;
  MC_InitStructure.MC_Period = 0x3FF;
  MC_InitStructure.MC_Channel = MC_Channel_U;
  MC_InitStructure.MC_PulseU = 0x1FF;
  MC_InitStructure.MC_PolarityUL = MC_Polarity_NonInverted;
  MC_InitStructure.MC_PolarityUH = MC_Polarity_NonInverted;
  MC_InitStructure.MC_DeadTime = 0xF;
  MC_InitStructure.MC_RepetitionCounter = 0x0;

  MC_Init(&MC_InitStructure);


/* Channel V configuration */
  MC_InitStructure.MC_Channel = MC_Channel_V;
  MC_InitStructure.MC_PulseV = 0xFF;
  MC_InitStructure.MC_PolarityVL = MC_Polarity_NonInverted;
  MC_InitStructure.MC_PolarityVH = MC_Polarity_NonInverted;

  MC_Init(&MC_InitStructure);

/* Channel W configuration */
  MC_InitStructure.MC_Channel = MC_Channel_W;
  MC_InitStructure.MC_PulseW = 0x7F;
  MC_InitStructure.MC_PolarityWL = MC_Polarity_NonInverted;
  MC_InitStructure.MC_PolarityWH = MC_Polarity_NonInverted;

  MC_Init(&MC_InitStructure);
 

/* MC peripheral counters enable */ 
  MC_Cmd(ENABLE);

/* Enables the different outputs */
  MC_CtrlPWMOutputs(ENABLE);

  while(1);
}
 

/*******************************************************************************
* Function Name  : SCU_Configuration
* Description    : Configures the Master clock @96MHz and the Main system 
*                  configuration and Clocks (FMI,PLL,RCLK,HCLK,PCLK ,MCLK )
*                  and enable clocks for peripherals.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SCU_Configuration(void)
{
  SCU_MCLKSourceConfig(SCU_MCLK_OSC);	/* Default configuration */
  
  /*wait state insertion :This function should be executed from SRAM when*/
  /*booting from bank1 to avoid  Read-While-Write from the Same Bank.*/
  FMI_Config(FMI_READ_WAIT_STATE_2, FMI_WRITE_WAIT_STATE_0, FMI_PWD_ENABLE,\
                 FMI_LVD_ENABLE, FMI_FREQ_HIGH);/*Insert 2 Wait States for read*/
                                                
  SCU_PLLFactorsConfig(192, 25, 2); /* PLL factors Configuration based on*/
                                        /* a OSC/Crystal value = 25Mhz*/     
  SCU_PLLCmd(ENABLE);  /* PLL Enable and wait for Locking*/     
  SCU_RCLKDivisorConfig(SCU_RCLK_Div1); /* RCLK @96Mhz */
  SCU_HCLKDivisorConfig(SCU_HCLK_Div1); /* AHB @96Mhz */
  SCU_FMICLKDivisorConfig(SCU_FMICLK_Div1);/* FMI @96Mhz */
  SCU_PCLKDivisorConfig(SCU_PCLK_Div2); /* APB @48Mhz */
  SCU_MCLKSourceConfig(SCU_MCLK_PLL);  /* MCLK @96Mhz */
  
  /* MC Clock enable */
  SCU_APBPeriphClockConfig( __MC, ENABLE);

 /* GPIO6 Clock enable */
  SCU_APBPeriphClockConfig(__GPIO6, ENABLE);

  }
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
