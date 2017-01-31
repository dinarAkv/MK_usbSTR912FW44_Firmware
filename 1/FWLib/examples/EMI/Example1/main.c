/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V2.1
* Date               : 12/22/2008
* Description        : This is an EMI 8bits Non-Mux mode example running @96MHz
*                      It aims to configure the STR9 EMI in 8bits Non-Mux mode.
*                      Hence some bytes are written in external memory mapped
*                      on bank0.
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
GPIO_InitTypeDef  GPIO_InitStructure;
EMI_InitTypeDef   EMI_InitStruct;

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

int main()
{

#ifdef DEBUG
  debug();
#endif

  /* Configure the system clocks */
  SCU_Configuration();

  /* Configure the GPIO ports */
  GPIO_Configuration();

  /* EMI default configuration : Reset configuration*/
  EMI_DeInit();

  /**************************EMI configuration*********************************/

  EMI_StructInit(&EMI_InitStruct);

  /* Number of bus turnaround cycles added between read and write accesses.*/
  EMI_InitStruct.EMI_Bank_IDCY = 0x0F ;

  /* Number of wait states for read accesses*/
  EMI_InitStruct.EMI_Bank_WSTRD = 0x1F ;

  /* Number of wait states for write accesses*/
  EMI_InitStruct.EMI_Bank_WSTWR = 0x1F ;

  /*Output enable assertion delay from chip select assertion*/
  EMI_InitStruct.EMI_Bank_WSTROEN = 0x03;

  /*Write enable assertion delay from chip select assertion*/
  EMI_InitStruct.EMI_Bank_WSTWEN = 0x01;

  /*This member Controls the memory width*/
  EMI_InitStruct.EMI_Bank_MemWidth = EMI_Width_Byte;

  /*Write protection feature */
  EMI_InitStruct.EMI_Bank_WriteProtection =  EMI_Bank_NonWriteProtect;

  /* Normal mode read*/
  EMI_InitStruct.EMI_Burst_and_PageModeRead_Selection =  EMI_NormalMode;

  /*Use Bank0 (CS0)*/
  EMI_Init( EMI_Bank0, &EMI_InitStruct);

  while (1)
  {

#ifdef Buffered
    /*External Memory Bank 0 AHB buffered*/
    *(u8*)0x2C000001 = 0xa  ;
    *(u8*)0x2C000002 = 0xb;
    *(u8*)0x2C000003 = 0xc;
    *(u8*)0x2C000004 = 0xd;
#else
    /*External Memory Bank 0 AHB unbuffered*/
    *(u8*)0x3C000001 = 0xa ;
    *(u8*)0x3C000002 = 0xb;
    *(u8*)0x3C000003 = 0xc;
    *(u8*)0x3C000004 = 0xd;

#endif
  }

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
  
  /* Enable the clock for EMI*/
  SCU_AHBPeriphClockConfig(__EMI | __EMI_MEM_CLK, ENABLE);
  SCU_EMIBCLKDivisorConfig(SCU_EMIBCLK_Div1);

  /*Enable the Non-mux mode*/
  SCU_EMIModeConfig(SCU_EMI_DEMUX);

  /* Enable the GPIO7 Clock */
  SCU_APBPeriphClockConfig(__GPIO7 , ENABLE);

  /* Enable the GPIO5 Clock */
  SCU_APBPeriphClockConfig(__GPIO5 , ENABLE);

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
  /* GPIO8,GPIO9 Configuration*/

  GPIO_EMIConfig(ENABLE);

  /* GPIO7 Configuration */
  GPIO_DeInit(GPIO7);
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |
                              GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull;
  GPIO_InitStructure.GPIO_Alternate = GPIO_OutputAlt2 ;
  GPIO_Init (GPIO7, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;

  /*EMI-A7 mode 8bits*/
  GPIO_InitStructure.GPIO_Alternate = GPIO_OutputAlt3;

  GPIO_Init (GPIO7, &GPIO_InitStructure);

  /* GPIO5 Configuration */
  GPIO_DeInit(GPIO5);
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 ;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull;
  GPIO_InitStructure.GPIO_Alternate = GPIO_OutputAlt3;
  GPIO_Init (GPIO5, &GPIO_InitStructure);
}
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
