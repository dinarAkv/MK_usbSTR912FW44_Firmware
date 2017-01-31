/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V2.1
* Date               : 12/22/2008
* Description        : Short Example, Program using the FMI.
*                      The purpose of this program is to erase one sector 
*                      then write some data to this sector.
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

u8 FMI_Timeout_Status;

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
  
 /*Configure the system clocks*/
  SCU_Configuration();
  
/* -------------------------------------------------------------------------- */
/*             Disable the Write protection for sector 0 of bank 1            */
/* -------------------------------------------------------------------------- */ 

  FMI_WriteProtectionCmd(FMI_B1S0, DISABLE);
  
/* -------------------------------------------------------------------------- */
/*                        Erase the sector 0 of bank 1                        */
/* -------------------------------------------------------------------------- */ 

  FMI_EraseSector(FMI_B1S0);
  FMI_Timeout_Status = FMI_WaitForLastOperation(FMI_BANK_1);

/* -------------------------------------------------------------------------- */
/*                                 Write Data                                 */
/* -------------------------------------------------------------------------- */ 
  FMI_WriteHalfWord(0x80000,0x1111);
  FMI_Timeout_Status = FMI_WaitForLastOperation(FMI_BANK_1);

  FMI_WriteHalfWord(0x80002,0x1111);
  FMI_Timeout_Status = FMI_WaitForLastOperation(FMI_BANK_1);

  FMI_WriteHalfWord(0x80004,0x2222);
  FMI_Timeout_Status = FMI_WaitForLastOperation(FMI_BANK_1);

  FMI_WriteHalfWord(0x80006,0x2222);
  FMI_Timeout_Status = FMI_WaitForLastOperation(FMI_BANK_1);

  FMI_WriteHalfWord(0x80008,0x3333);
  FMI_Timeout_Status = FMI_WaitForLastOperation(FMI_BANK_1);

  FMI_WriteHalfWord(0x8000A,0x3333);
  FMI_Timeout_Status = FMI_WaitForLastOperation(FMI_BANK_1);

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
  
  SCU_AHBPeriphClockConfig(__FMI, ENABLE);
  SCU_AHBPeriphReset(__FMI, DISABLE);
  }
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
