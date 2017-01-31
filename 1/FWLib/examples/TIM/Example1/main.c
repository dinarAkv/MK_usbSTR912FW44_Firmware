/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V2.1
* Date               : 12/22/2008
* Description        : Program using the TIM peripheral.
*                      Shows the usage of PWM and One Pulse Mode (OPM) 
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "91x_lib.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

TIM_InitTypeDef   TIM_InitStructure;
GPIO_InitTypeDef  GPIO_InitStructure;

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
                     
  /* GPIO Structure Initialization */
  GPIO_StructInit(&GPIO_InitStructure);

  /* GPIO4 configuration */
  GPIO_DeInit(GPIO4);                       
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull;
  GPIO_InitStructure.GPIO_Alternate = GPIO_OutputAlt2;
  GPIO_Init(GPIO4,&GPIO_InitStructure);


  /* GPIO7 configuration */
  GPIO_DeInit(GPIO7); 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_IPInputConnected = GPIO_IPInputConnected_Enable;
  GPIO_InitStructure.GPIO_Alternate = GPIO_InputAlt1;
  GPIO_Init(GPIO7,&GPIO_InitStructure);

  /* TIM0 Structure Initialization */
  TIM_StructInit(&TIM_InitStructure);

  /* TIM0 Configuration in PWM Mode */
  TIM_DeInit(TIM0);  /* TIM0 Deinitialization */
  TIM_InitStructure.TIM_Mode = TIM_PWM;
  TIM_InitStructure.TIM_Clock_Source = TIM_CLK_APB;       
  TIM_InitStructure.TIM_Prescaler = 0xFF;       
  TIM_InitStructure.TIM_Pulse_Level_1 = TIM_HIGH;   
  TIM_InitStructure.TIM_Period_Level = TIM_LOW;    
  TIM_InitStructure.TIM_Pulse_Length_1 = 0xBFF;
  TIM_InitStructure.TIM_Full_Period = 0xFFF;
  TIM_Init (TIM0, &TIM_InitStructure);
  
  /* Start the counter of TIM0 */
  TIM_CounterCmd(TIM0, TIM_START);
 
  /* TIM2 Structure Initialization */
  TIM_StructInit(&TIM_InitStructure);

  /* TIM2 Configuration in OPM Mode */
  TIM_DeInit(TIM2);    /* TIM2 Deinitialization */
  TIM_InitStructure.TIM_Mode = TIM_OPM;    
  TIM_InitStructure.TIM_OPM_INPUT_Edge = TIM_OPM_EDGE_RISING;
  TIM_InitStructure.TIM_Clock_Source = TIM_CLK_APB;       
  TIM_InitStructure.TIM_Prescaler = 0xFF;       
  TIM_InitStructure.TIM_Pulse_Level_1 = TIM_HIGH;  
  TIM_InitStructure.TIM_Period_Level = TIM_LOW; 
  TIM_InitStructure.TIM_Pulse_Length_1 = 0x3FF;
  TIM_Init (TIM2, &TIM_InitStructure);

  /* Start the counter of TIM2 */
  TIM_CounterCmd(TIM2, TIM_START);


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
  
  SCU_APBPeriphClockConfig(__TIM01, ENABLE); /*Enable the clock for TIM0 and TIM1*/
 
  SCU_APBPeriphClockConfig(__TIM23, ENABLE); /*Enable the clock for TIM2 and TIM3*/
  
  SCU_APBPeriphClockConfig(__GPIO4, ENABLE); /*Enable the clock for GPIO4 */
  
  SCU_APBPeriphClockConfig(__GPIO7, ENABLE); /*Enable the clock for GPIO7 */
                         
}     
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
