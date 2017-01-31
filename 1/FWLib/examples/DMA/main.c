/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V2.1
* Date               : 12/22/2008
* Description        : main program for DMA example.
*
*                     This example use the DMA Linked list items to perform
*                     a scattering of a "Buffer0"  in the ram to three other
*                     buffers " Buffer1,Buffer2,Buffer3" in another location
*                     in memory.
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
* CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS
* A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
* OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
* OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
* CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/


/* Standard include ----------------------------------------------------------*/
#include "91x_lib.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

DMA_InitTypeDef  DMA_InitStruct;
LLI_InitTypeDef LLI1_InitStructure, LLI2_InitStructure, LLI3_InitStructure;
LLI_CCR_InitTypeDef LLI1_CCR_InitStruct, LLI2_CCR_InitStruct, LLI1_CCR_InitStruct;

/* The first Buffer to be transfered to the other three Buffers ( source Buffer)*/
volatile u32 Buffer0[36] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0, 1, 2, 3,\
               4, 5, 6, 7, 8, 9, 10, 11, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

/* The Three Destination buffers*/
volatile u32 Buffer1[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
volatile u32 Buffer2[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
volatile u32 Buffer3[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

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

  /* LLIx_CCR_InitStructure determines the appropriate control word :
  Please refer to the DMA_CCx Control register on the STR9 UM for more details*/
  /*Transfer size*/

  LLI1_CCR_InitStruct.LLI_TrsfSize = 12;

  /*Source burst size*/
  LLI1_CCR_InitStruct.LLI_SrcBstSize = DMA_SrcBst_4Data;

  /*Destination burst size*/
  LLI1_CCR_InitStruct.LLI_DesBstSize = DMA_DesBst_4Data;

  /*Source width*/
  LLI1_CCR_InitStruct.LLI_SrcWidth = DMA_SrcWidth_Word;

  /*Destination width*/
  LLI1_CCR_InitStruct.LLI_DesWidth = DMA_DesWidth_Word;

  /*Source address is incremented*/
  LLI1_CCR_InitStruct.LLI_SrcIncrement = DMA_SrcIncrement;

  /*Destination address is incremented*/
  LLI1_CCR_InitStruct.LLI_DesIncrement = DMA_DesIncrement;

  /*********************Linked list items for the second Link LLI**************/

  /*Source address */
  LLI1_InitStructure.LLI_SrcAdd = (u32)(&Buffer0[12]);

  /*Destination address*/
  LLI1_InitStructure.LLI_DesAdd = (u32)(&Buffer2[0]);

  /*Pointer*/
  LLI1_InitStructure.LLI_Pointer = (u32)(&(LLI2_InitStructure));

  /*Control word*/
  LLI1_InitStructure.LLI_CCR = DMA_LLI_CCR_Init(&LLI1_CCR_InitStruct);



  /********************Linked list items for the Third Link LLI****************/

  /*Source address */
  LLI2_InitStructure.LLI_SrcAdd = (u32)(&Buffer0[24]);

  /*Destination address*/
  LLI2_InitStructure.LLI_DesAdd = (u32)(&Buffer3[0]);

  /*Pointer*/
  LLI2_InitStructure.LLI_Pointer = 0;

  /*Control word*/
  LLI2_InitStructure.LLI_CCR = DMA_LLI_CCR_Init(&LLI1_CCR_InitStruct);



  /******************************DMA Channel1 Configuration********************/


  /* DMA default configuration : Reset configuration*/
  DMA_DeInit();

  /*Enable the DMA*/
  DMA_Cmd(ENABLE);

  DMA_StructInit(&DMA_InitStruct);


  /*The First LLI is programmed on the DMA chaneel register before starting
  the transfer*/

  /*Set the addresses of next linked list for the first LLI structure*/
  DMA_InitStruct.DMA_Channel_LLstItm = (u32)(&(LLI1_InitStructure.LLI_SrcAdd));

  /* Source address for the first LLI structure */
  DMA_InitStruct.DMA_Channel_SrcAdd = (u32)(&Buffer0[0]);

  /*Destination address for the first LLI structure */
  DMA_InitStruct.DMA_Channel_DesAdd = (u32)(&Buffer1[0]);

  /* The source bus width is a word" 32 bits"*/
  DMA_InitStruct.DMA_Channel_SrcWidth = DMA_SrcWidth_Word;

  /* The Destination bus width is a word */
  DMA_InitStruct.DMA_Channel_DesWidth = DMA_DesWidth_Word;

  /* DMA is The flow controller*/
  DMA_InitStruct.DMA_Channel_FlowCntrl = DMA_FlowCntrlt0_DMA;

  /*Transfer size*/
  DMA_InitStruct.DMA_Channel_TrsfSize = 12;

  /*Configure the DMA channel1 "the chosen channel to perform the transfer"*/
  /*Incrementing source address*/
  DMA_ChannelSRCIncConfig (DMA_Channel1, ENABLE);

  /*Incrementing destination address*/
  DMA_ChannelDESIncConfig (DMA_Channel1, ENABLE);

  /* Update the DMA channel1 registers with the cfirst LLI structure*/
  DMA_Init(DMA_Channel1, &DMA_InitStruct);

  /*Enable the DMA channel*/
  DMA_ChannelCmd (DMA_Channel1, ENABLE);

  /*Wait for the FIFO to be empty*/
  while (DMA_GetChannelActiveStatus(DMA_Channel1));

  while (1);

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


}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
