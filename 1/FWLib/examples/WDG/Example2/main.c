/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V2.1
* Date               : 12/22/2008
* Description        : Main program for wdg Example2
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/


/* Standard include ----------------------------------------------------------*/
#include "91x_lib.h"

/* Include of other module interface headers ---------------------------------*/
/* Local includes ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
  WDG_InitTypeDef WDG_InitStructure;
  GPIO_InitTypeDef  GPIO9_InitStruct;
  void Delay(u32 nCount);
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
                                                
  /* Configure the GPIO9  */
  GPIO_DeInit(GPIO9);
  GPIO9_InitStruct.GPIO_Pin= GPIO_Pin_All;  /* Choose the all pins*/
  GPIO9_InitStruct.GPIO_Direction=GPIO_PinOutput; /* Choose  port Direction" output".*/
  GPIO_Init(GPIO9,&GPIO9_InitStruct); /*GPIO9 initialization with the previous chosen parameters.*/
 
  /* GPIO7 initialization*/
  GPIO_DeInit(GPIO7);
  
  /* Configure the WDG to generate a system reset signal after 5 s */
  WDG_InitStructure.WDG_Preload = 0x27B;
  WDG_InitStructure.WDG_Prescaler = 0xFF;
  WDG_InitStructure.WDG_ClockSource=WDG_ClockSource_Rtc; /* using the 32khz RTC clock as clock source*/
  WDG_Init(&WDG_InitStructure);  /* WDG initialization with the previous chosen parameters.*/
 
  /*Led sequence to indicate application restart*/
  GPIO_WriteBit(GPIO9, GPIO_Pin_0, Bit_RESET);
  Delay(0xFFFFF);
  GPIO_WriteBit(GPIO9, GPIO_Pin_1, Bit_RESET);
  Delay(0xFFFFF);
  GPIO_WriteBit(GPIO9, GPIO_Pin_2, Bit_RESET);
  Delay(0xFFFFF);
  GPIO_WriteBit(GPIO9, GPIO_Pin_3, Bit_RESET);
  

  /*if pin P7.4 goes low, then the watchdog will be started*/
  while((GPIO_Read(GPIO7)&0x10));    
  
  /*start watchdog counter*/
  WDG_StartWatchdogMode();
 
  while(1);
}

/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length.
* Output         : None
* Return         : None
*******************************************************************************/
void Delay(u32 nCount)
{
 vu32 Delay_tmp = 0x00;

 for(Delay_tmp = 0; Delay_tmp< nCount; Delay_tmp++);
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
  
  SCU_APBPeriphClockConfig(__GPIO7, ENABLE);/* Enable the clock for GPIO7*/
  SCU_APBPeriphClockConfig(__GPIO9, ENABLE); /* Enable the clock for GPIO9*/
  SCU_APBPeriphClockConfig(__WDG, ENABLE);  /* Enable the clock for WDG*/
  SCU_AHBPeriphClockConfig(__VIC,ENABLE);   /* Enable the clock to VIC*/
}  
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/







