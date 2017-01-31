/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V2.1
* Date               : 12/22/2008
* Description        : Program using the ADC peripheral.triggered through the 
*                      external Pin to start the conversion.When 16 samples are 
*                      transferred to internal SRAM (using the DMA capability) 
*                      the ADC is disabled.  
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

TIM_InitTypeDef   TIM_InitStructure;
ADC_InitTypeDef   ADC_InitStructure;
GPIO_InitTypeDef  GPIO_InitStructure;
DMA_InitTypeDef   DMA_InitStruct;
u16 Dest_Buffer[16] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

/* Private function prototypes -----------------------------------------------*/
void SCU_Configuration(void);
void GPIO_Configuration(void);

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

  /* Configure the GPIO ports */
  GPIO_Configuration();
  
  VIC_DeInit(); /* VIC Deinitialization */
  VIC_InitDefaultVectors(); /* initialize VICs default vector registers*/
  
  /*DMA interrupt config*/
  VIC_Config(DMA_ITLine, VIC_IRQ, 0);
  VIC_ITCmd(DMA_ITLine, ENABLE);

  /* TIM01 Structure Initialization */
  TIM_StructInit(&TIM_InitStructure);

  /* TIM0 Configuration in PWM Mode */
  TIM_DeInit(TIM0); /* TIM0 Deinitialization*/
  TIM_InitStructure.TIM_Mode = TIM_PWM;    
  TIM_InitStructure.TIM_Clock_Source = TIM_CLK_APB;       
  TIM_InitStructure.TIM_Prescaler = 0x0;       
  TIM_InitStructure.TIM_Pulse_Level_1 = TIM_HIGH;   
  TIM_InitStructure.TIM_Period_Level = TIM_LOW;    
  TIM_InitStructure.TIM_Pulse_Length_1 = 0x80;
  TIM_InitStructure.TIM_Full_Period = 0x100;
  TIM_Init (TIM0, &TIM_InitStructure);
 

  /* TIM1 Configuration in PWM Mode */
  TIM_DeInit(TIM1); /* TIM1 Deinitialization*/
  TIM_InitStructure.TIM_Mode = TIM_PWM;    
  TIM_InitStructure.TIM_Clock_Source = TIM_CLK_APB;       
  TIM_InitStructure.TIM_Prescaler = 0x0;       
  TIM_InitStructure.TIM_Pulse_Level_1 = TIM_HIGH;   
  TIM_InitStructure.TIM_Period_Level = TIM_LOW;    
  TIM_InitStructure.TIM_Pulse_Length_1 = 0x100;
  TIM_InitStructure.TIM_Full_Period = 0x200;
  TIM_Init (TIM1, &TIM_InitStructure);

  /* Start the counter of TIM0 */
  TIM_CounterCmd(TIM0, TIM_START);
  
  /* Start the counter of TIM1 */
  TIM_CounterCmd(TIM1, TIM_START);
  
  /* ADC Structure Initialization */
  ADC_StructInit(&ADC_InitStructure);

  /* Configure the ADC structure in Single mode conversion */
  ADC_DeInit(); /* ADC Deinitialization */
  ADC_InitStructure.ADC_Channel_6_Mode = ADC_NoThreshold_Conversion;
  ADC_InitStructure.ADC_Select_Channel = ADC_Channel_6;
  ADC_InitStructure.ADC_Scan_Mode = DISABLE;
  ADC_InitStructure.ADC_Conversion_Mode =ADC_Single_Mode;
  
  /* Enable the ADC */
  ADC_Cmd(ENABLE);

  /* Prescaler config */
  ADC_PrescalerConfig(0x2);

  /* Configure the ADC */
  ADC_Init(&ADC_InitStructure);

  
  /* Start the ADC conversion */
  
  ADC_DMACmd(ENABLE);                                 /*Enable ADC-DMA Request*/
  ADC_ExternalTrigConfig(ADC_PIN_Trig  , Rising_ETE); /*Trigg Source&Edge*/
  ADC_ExternalTrigCmd(ENABLE);                       /*External trigger enable*/
  
  
 /*******************************DMA configuration*****************************/
 
 
  /* DMA default configuration : Reset configuration*/
  DMA_DeInit(); 
    
  /*Enable the DMA*/
  DMA_Cmd(ENABLE);  
    
  DMA_StructInit(&DMA_InitStruct);

  /*************************** Channel1 configuration *************************/
  DMA_InitStruct.DMA_Channel_LLstItm=0;
  
  /* Source address */
  DMA_InitStruct.DMA_Channel_SrcAdd= (u32)(&ADC->DDR); 
  
  /*Destination address  */
  DMA_InitStruct.DMA_Channel_DesAdd=((u32)&Dest_Buffer[0]);  
  
  /* The source bus width is a HalfWord" 16 bits"*/
  DMA_InitStruct.DMA_Channel_SrcWidth= DMA_SrcWidth_HalfWord;
  
  /* The Destination bus width is a HalfWord" 16 bits*/
  DMA_InitStruct.DMA_Channel_DesWidth= DMA_DesWidth_HalfWord; 
   
  /* DMA is The flow controller*/
  DMA_InitStruct.DMA_Channel_FlowCntrl=  DMA_FlowCntrl2_DMA;
  
  DMA_InitStruct.DMA_Channel_Des = DMA_SRC_External_Req1;
  
  /*Transfer size*/
  DMA_InitStruct.DMA_Channel_TrsfSize =16;  
  
  /* Increment the destination*/ 
  
  DMA_ChannelDESIncConfig (DMA_Channel1, ENABLE);
  
  DMA_ITConfig(DMA_Channel1, ENABLE);
  DMA_ITMaskConfig(DMA_Channel1, DMA_ITMask_ITC, ENABLE);
  
  /* Update the DMA channel1 registers */
  DMA_Init(DMA_Channel1,&DMA_InitStruct);
  
  /*Enable the DMA channel*/
  DMA_ChannelCmd (DMA_Channel1,ENABLE);
 
  
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
  
  /*Enable DMA clock*/
  SCU_AHBPeriphClockConfig(__DMA, ENABLE);

   /*Enable VIC clock*/
  SCU_AHBPeriphClockConfig(__VIC,ENABLE);
  
  /* Enable the clock for the ADC */
  SCU_APBPeriphClockConfig(__ADC, ENABLE); 
  
  /* Enable the clock for TIM0 and TIM1 */
  SCU_APBPeriphClockConfig(__TIM01, ENABLE); 
  
  /* Enable the GPIO4 Clock */
  SCU_APBPeriphClockConfig(__GPIO4, ENABLE);

  /* Enable the GPIO6 Clock */
  SCU_APBPeriphClockConfig(__GPIO6, ENABLE);

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
  
  /* Configure the GPIO4 pin 6 as analog input */
  GPIO_DeInit(GPIO4);
  GPIO_ANAPinConfig(GPIO_ANAChannel6, ENABLE);
 
  /* GPIO4 configuration (PWM on P4.2) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull;
  GPIO_InitStructure.GPIO_Alternate = GPIO_OutputAlt2;
  GPIO_Init(GPIO4,&GPIO_InitStructure);

  GPIO_DeInit(GPIO6);
  /* GPIO6 configuration (PWM on P6.0, pin 29) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull;
  GPIO_InitStructure.GPIO_Alternate = GPIO_OutputAlt2;
  GPIO_Init(GPIO6, &GPIO_InitStructure);
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
