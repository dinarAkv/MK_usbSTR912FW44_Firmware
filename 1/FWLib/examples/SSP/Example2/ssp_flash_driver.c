/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : ssp_flash_driver.h
* Author             : MCD Application Team
* Version            : V2.1
* Date               : 12/22/2008
* Description        : Driver containing c functions to access to M25P64 ST 
*                      Serial Flash Memory available on MB460 eval board
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
* CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS
* A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
* OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
* OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
* CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "ssp_flash_driver.h"

/* Private typedef -----------------------------------------------------------*/
#define SSP_FLASH_PageSize 256

#define WRITE      0x02  /* Write to Memory instruction */
#define WRSR       0x01  /* Write Status Register instruction */
#define WREN       0x06  /* Write enable instruction */

#define READ       0x03  /* Read from Memory instruction */
#define RDSR       0x05  /* Read Status Register instruction  */
#define SE         0xD8  /* Sector Erase instruction */
#define BE         0xC7  /* Bulk Erase instruction */

#define Low        0x00  /* ChipSelect line low */
#define High       0x01  /* ChipSelect line high */

#define WIP_Flag   0x01  /* Write In Progress (WIP) flag */

#define Dummy_Byte 0xA5

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SSP_FLASH_ChipSelect(u8 State);
static u8 SSP_FLASH_SendByte(u8 byte);
static void SSP_FLASH_WriteEnable(void);
static void SSP_FLASH_WaitForWriteEnd(void);

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : SSP_FLASH_Init
* Description    : Initializes the peripherals used by the SPI FLASH driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SSP_FLASH_Init(void)
{
  SSP_InitTypeDef  SSP_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_DeInit(GPIO5);
  /* Gonfigure SSP0_CLK, SSP0_MOSI */
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull;
  GPIO_InitStructure.GPIO_Alternate = GPIO_OutputAlt2;
  GPIO_Init(GPIO5, &GPIO_InitStructure);

  /* Gonfigure SSP0_NSS pins */
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_OpenCollector;
  GPIO_InitStructure.GPIO_Alternate = GPIO_OutputAlt1;
  GPIO_Init(GPIO5, &GPIO_InitStructure);

  /* Gonfigure SSP0_MISO pin GPIO5.6 */
  GPIO_InitStructure.GPIO_Direction = GPIO_PinInput;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull;
  GPIO_InitStructure.GPIO_IPInputConnected = GPIO_IPInputConnected_Enable;
  GPIO_InitStructure.GPIO_Alternate = GPIO_InputAlt1 ;
  GPIO_Init(GPIO5, &GPIO_InitStructure);

  /* SSP0 configuration */
  SSP_DeInit(SSP0);
  SSP_InitStructure.SSP_FrameFormat = SSP_FrameFormat_Motorola;
  SSP_InitStructure.SSP_Mode = SSP_Mode_Master;
  SSP_InitStructure.SSP_CPOL = SSP_CPOL_High;
  SSP_InitStructure.SSP_CPHA = SSP_CPHA_2Edge;
  SSP_InitStructure.SSP_DataSize = SSP_DataSize_8b;
  SSP_InitStructure.SSP_ClockRate = 0;
  SSP_InitStructure.SSP_ClockPrescaler = 2;
  SSP_Init(SSP0, &SSP_InitStructure);

  /* SSP0 enable */
  SSP_Cmd(SSP0, ENABLE);
}

/*******************************************************************************
* Function Name  : SSP_FLASH_SectorErase
* Description    : Erases the specified FLASH sector.
* Input          : SectorAddr: address of the sector to erase.
* Output         : None
* Return         : None
*******************************************************************************/
void SSP_FLASH_SectorErase(u32 SectorAddr)
{
  /* Send write enable instruction */
  SSP_FLASH_WriteEnable();
			
  /* Sector Erase */
  /* Select the FLASH: Chip Select low */
  SSP_FLASH_ChipSelect(Low);		
  /* Send Sector Erase instruction  */
  SSP_FLASH_SendByte(SE);
  /* Send SectorAddr high nibble address byte */
  SSP_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  /* Send SectorAddr medium nibble address byte */
  SSP_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  /* Send SectorAddr low nibble address byte */
  SSP_FLASH_SendByte(SectorAddr & 0xFF);
  /* Deselect the FLASH: Chip Select high */
  SSP_FLASH_ChipSelect(High);	

  /* Wait the end of Flash writing */
  SSP_FLASH_WaitForWriteEnd();	
}

/*******************************************************************************
* Function Name  : SSP_FLASH_BulkErase
* Description    : Erases the entire FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SSP_FLASH_BulkErase(void)
{ 	
  /* Send write enable instruction */
  SSP_FLASH_WriteEnable();	
	
  /* Bulk Erase */
  /* Select the FLASH: Chip Select low */
  SSP_FLASH_ChipSelect(Low);		
  /* Send Bulk Erase instruction  */
  SSP_FLASH_SendByte(BE);
  /* Deselect the FLASH: Chip Select high */
  SSP_FLASH_ChipSelect(High);	
		
  /* Wait the end of Flash writing */
  SSP_FLASH_WaitForWriteEnd();		
}

/*******************************************************************************
* Function Name  : SSP_FLASH_PageWrite
* Description    : Writes more than one byte to the FLASH with a single WRITE
*                  cycle(Page WRITE sequence). The number of byte can't exceed
*                  the FLASH page size.
* Input          : - pBuffer : pointer to the buffer  containing the data to be
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH,
*                    must be equal or less than "SSP_FLASH_PageSize" value.
* Output         : None
* Return         : None
*******************************************************************************/
void SSP_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  /* Enable the write access to the FLASH */
  SSP_FLASH_WriteEnable();		

  /* Select the FLASH: Chip Select low */
  SSP_FLASH_ChipSelect(Low);	
  /* Send "Write to Memory " instruction */
  SSP_FLASH_SendByte(WRITE);	
  /* Send WriteAddr high nibble address byte to write to */
  SSP_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  /* Send WriteAddr medium nibble address byte to write to */
  SSP_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  /* Send WriteAddr low nibble address byte to write to */
  SSP_FLASH_SendByte(WriteAddr & 0xFF);

  /* while there is data to be written on the FLASH */
  while(NumByteToWrite--)
  {
    /* Send the current byte */			
    SSP_FLASH_SendByte(*pBuffer); 	

    /* Point on the next byte to be written */
    pBuffer++;
  }		

  /* Deselect the FLASH: Chip Select high */
  SSP_FLASH_ChipSelect(High);

  /* Wait the end of Flash writing */
  SSP_FLASH_WaitForWriteEnd();	
}

/*******************************************************************************
* Function Name  : SSP_FLASH_BlockWrite
* Description    : Writes block of data to the FLASH. In this function, each
*                  byte is written using Byte WRITE sequence.
* Input          : - pBuffer : pointer to the buffer  containing the data to be
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
void SSP_FLASH_BlockWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  /* while there is data to be written */
  while(NumByteToWrite--)
  {
    /* Enable the write access to the FLASH */
    SSP_FLASH_WriteEnable();		

    /* Select the FLASH: Chip Select low */
    SSP_FLASH_ChipSelect(Low);	

    /* Send "Write to Memory " instruction */
    SSP_FLASH_SendByte(WRITE);	
    /* Send WriteAddr high nibble address byte to write to */
    SSP_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
    /* Send WriteAddr medium nibble address byte to write to */
    SSP_FLASH_SendByte((WriteAddr& 0xFF00) >> 8);
    /* Send WriteAddr low nibble address byte to write to */
    SSP_FLASH_SendByte(WriteAddr & 0xFF);

    /* Send the current byte  */			
    SSP_FLASH_SendByte(*pBuffer); 	

    /* Deselect the FLASH: Chip Select high */
    SSP_FLASH_ChipSelect(High);	

    /* Point to the next byte to be written */
    pBuffer++;

    /* Increment the FLASH's internal address to write to */
    WriteAddr++;
    	
    /* Wait the end of Flash writing */
    SSP_FLASH_WaitForWriteEnd();	
  }		
}

/*******************************************************************************
* Function Name  : SSP_FLASH_BufferWrite
* Description    : Writes block of data to the FLASH. In this function, the
*                  number of WRITE cycles are reduced, using Page WRITE sequence.
* Input          : - pBuffer : pointer to the buffer  containing the data to be
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
void SSP_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  Addr = WriteAddr % SSP_FLASH_PageSize;
  count = SSP_FLASH_PageSize - Addr;
  NumOfPage =  NumByteToWrite / SSP_FLASH_PageSize;
  NumOfSingle = NumByteToWrite % SSP_FLASH_PageSize;

  if(Addr == 0) /* WriteAddr is SSP_FLASH_PageSize aligned  */
  {
    if(NumOfPage == 0) /* NumByteToWrite < SSP_FLASH_PageSize */
    {
      SSP_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SSP_FLASH_PageSize */
    {
      while(NumOfPage--)
      {
        SSP_FLASH_PageWrite(pBuffer, WriteAddr, SSP_FLASH_PageSize);
        WriteAddr +=  SSP_FLASH_PageSize;
        pBuffer += SSP_FLASH_PageSize;
      }

      SSP_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
   }
  }
  else /* WriteAddr is not SSP_FLASH_PageSize aligned  */
  {
    if(NumOfPage== 0) /* NumByteToWrite < SSP_FLASH_PageSize */
    {
      if(NumOfSingle > count) /* (NumByteToWrite + WriteAddr) > SSP_FLASH_PageSize */
      {
      	temp = NumOfSingle - count;
      	
      	SSP_FLASH_PageWrite(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;

        SSP_FLASH_PageWrite(pBuffer, WriteAddr, temp);       	
      }
      else
      {
      	SSP_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SSP_FLASH_PageSize */
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SSP_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SSP_FLASH_PageSize;	

      SSP_FLASH_PageWrite(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;

      while(NumOfPage--)
      {
        SSP_FLASH_PageWrite(pBuffer, WriteAddr, SSP_FLASH_PageSize);
        WriteAddr +=  SSP_FLASH_PageSize;
        pBuffer += SSP_FLASH_PageSize;
      }

      if(NumOfSingle != 0)
      {
        SSP_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }      	
}

/*******************************************************************************
* Function Name  : SSP_FLASH_BufferRead
* Description    : Reads a block of data from the FLASH.
* Input          : - pBuffer : pointer to the buffer that receives the data read
*                    from the FLASH.
*                  - ReadAddr : FLASH's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
void SSP_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
  /* Select the FLASH: Chip Select low */
  SSP_FLASH_ChipSelect(Low);	

  /* Send "Read from Memory " instruction */
  SSP_FLASH_SendByte(READ);	

  /* Send ReadAddr high nibble address byte to read from */
  SSP_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte to read from */
  SSP_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte to read from */
  SSP_FLASH_SendByte(ReadAddr & 0xFF);
    	
  while(NumByteToRead--) /* while there is data to be read */
  {
    /* Read a byte from the FLASH */			
    *pBuffer = SSP_FLASH_SendByte(Dummy_Byte);

    /* Point to the next location where the byte read will be saved */
    pBuffer++;
  }	

  /* Deselect the FLASH: Chip Select high */
  SSP_FLASH_ChipSelect(High);		
}

/*******************************************************************************
* Function Name  : SSP_FLASH_ChipSelect
* Description    : Selects or deselects the FLASH.
* Input          : State : level to be applied on the FLASH's ChipSelect pin.
* Output         : None
* Return         : None
*******************************************************************************/
static void SSP_FLASH_ChipSelect(u8 State)
{
  /* Set High or low the chip select line on P57 pin */
  GPIO_WriteBit(GPIO5, GPIO_Pin_7, (BitAction)State);
}

/*******************************************************************************
* Function Name  : SSP_FLASH_SendByte
* Description    : Sends a byte through the SSP interface and return the  byte
*                  received from the SSP bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
static u8 SSP_FLASH_SendByte(u8 byte)
{
  /* Send byte through the SSP0 peripheral */	
  SSP0->DR = byte;	

  /* Loop while Transmit FIFO is full */
  while(SSP_GetFlagStatus(SSP0, SSP_FLAG_TxFifoEmpty) == RESET);

  /* Loop while Receive FIFO is empty */
  while(SSP_GetFlagStatus(SSP0, SSP_FLAG_RxFifoNotEmpty) == RESET);	

  /* Return the byte read from the SSP bus */
  return(SSP0->DR); 	
}

/*******************************************************************************
* Function Name  : SSP_FLASH_WriteEnable
* Description    : Enables the write access to the FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void SSP_FLASH_WriteEnable(void)
{
  /* Select the FLASH: Chip Select low */
  SSP_FLASH_ChipSelect(Low);	

  /* Send "Write Enable" instruction */
  SSP_FLASH_SendByte(WREN);

  /* Deselect the FLASH: Chip Select high */
  SSP_FLASH_ChipSelect(High);	
}

/*******************************************************************************
* Function Name  : SSP_FLASH_WaitForWriteEnd
* Description    : Polls the status of the Write In Progress (WIP) flag in the
*                  FLASH's status  register  and  loop  until write  opertaion
*                  has completed.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void SSP_FLASH_WaitForWriteEnd(void)
{
  u8 FLASH_Status = 0;

  /* Select the FLASH: Chip Select low */
  SSP_FLASH_ChipSelect(Low);
  	
  /* Send "Read Status Register" instruction */
  SSP_FLASH_SendByte(RDSR);

  /* Loop as long as the memory is busy with a write cycle */ 		
  do
  { 	
	
    /* Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    FLASH_Status = SSP_FLASH_SendByte(Dummy_Byte);
																	
  } while((FLASH_Status & WIP_Flag) == SET); /* Write in progress */

  /* Deselect the FLASH: Chip Select high */
  SSP_FLASH_ChipSelect(High);	 	
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
