/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_prop.c
* Author             : MCD Application Team
* Version            : V2.0.0
* Date               : 09/29/2008
* Description        : All processings related to DFU demo
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_pwr.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FLASH_BANK0_MODULE     0x000000FF  /* Bank 0 module   */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8  Load_Buffer[wTransferSize];          /* RAM Buffer for Downloaded Data */
u32 wBlockNum = 0, wlength = 0;
u32 Manifest_State = Manifest_complete;
u32 Pointer = ApplicationAddress;  /* Base Address to Erase, Program or Read */
DEVICE Device_Table = 
{
  EP_NUM,
  1
};

DEVICE_PROP Device_Property = 
{
  DFU_init,
  DFU_Reset,
  DFU_Status_In,
  DFU_Status_Out,
  DFU_Data_Setup,
  DFU_NoData_Setup,
  DFU_Get_Interface_Setting,
  DFU_GetDeviceDescriptor,
  DFU_GetConfigDescriptor,
  DFU_GetStringDescriptor,
  0,                    /*DFU_EP0Buffer*/
  bMaxPacketSize0       /*Max Packet size*/
};

USER_STANDARD_REQUESTS User_Standard_Requests = 
{
  DFU_GetConfiguration,
  DFU_SetConfiguration,
  DFU_GetInterface,
  DFU_SetInterface,
  DFU_GetStatus,
  DFU_ClearFeature,
  DFU_SetEndPointFeature,
  DFU_SetDeviceFeature,
  DFU_SetDeviceAddress
};

ONE_DESCRIPTOR Device_Descriptor = 
{
  (u8*)DFU_DeviceDescriptor,
  DFU_SIZ_DEVICE_DESC
};

ONE_DESCRIPTOR Config_Descriptor = 
{
  (u8*)DFU_ConfigDescriptor,
  DFU_SIZ_CONFIG_DESC
};

ONE_DESCRIPTOR DFU_String_Descriptor[6] = {
      { (u8*)DFU_StringLangId,         DFU_SIZ_STRING_LANGID },
      { (u8*)DFU_StringVendor,  DFU_SIZ_STRING_VENDOR },
      { (u8*)DFU_StringProduct,         DFU_SIZ_STRING_PRODUCT },
      { (u8*)DFU_StringSerial,  DFU_SIZ_STRING_SERIAL },
      { (u8*)DFU_StringInterface0, DFU_SIZ_STRING_INTERFACE0 },
      { (u8*)DFU_StringInterface1, DFU_SIZ_STRING_INTERFACE1 }
    };

/* Extern variables ----------------------------------------------------------*/
extern  u8 DeviceState ;
extern  u8 DeviceStatus[6];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : DFU_init
* Description    : DFU init routine.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DFU_init(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  pInfo->Current_Configuration = 0;

  /* Connect the device */
  PowerOn();
  /* USB interrupts initialization */
  _SetISTR(0);               /* clear pending interrupts */
  wInterrupt_Mask = IMR_MSK;
  _SetCNTR(wInterrupt_Mask); /* set interrupts mask */

  /* Enable USB interrupts */
  USB_Interrupts_Config();

  bDeviceState = UNCONNECTED;
}

/*******************************************************************************
* Function Name  : DFU_Reset
* Description    : DFU reset routine
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DFU_Reset(void)
{
  /* Set DFU_DEVICE as not configured */
  pInformation->Current_Configuration = 0;

  /* Current Feature initialization */
  pInformation->Current_Feature = DFU_ConfigDescriptor[7];

  _SetBTABLE(BTABLE_ADDRESS);

  /* Initialize Endpoint 0 */
  _SetEPType(ENDP0, EP_CONTROL);
  _SetEPTxStatus(ENDP0, EP_TX_NAK);
  _SetEPRxAddr(ENDP0, ENDP0_RXADDR);
  SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);
  _SetEPTxAddr(ENDP0, ENDP0_TXADDR);
  SetEPTxCount(ENDP0, Device_Property.MaxPacketSize);
  Clear_Status_Out(ENDP0);
  SetEPRxValid(ENDP0);

  /* Set this device to response on default address */
  SetDeviceAddress(0);
  
  bDeviceState = ATTACHED;
}

/*******************************************************************************
* Function Name  : DFU_SetConfiguration.
* Description    : Udpade the device state to configured.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void DFU_SetConfiguration(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
}

/*******************************************************************************
* Function Name  : DFU_SetConfiguration.
* Description    : Udpade the device state to addressed.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void DFU_SetDeviceAddress (void)
{
  bDeviceState = ADDRESSED;
}

/*******************************************************************************
* Function Name  : DFU_Status_In
* Description    : DFU status IN routine.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DFU_Status_In(void)
{
  return;
}

/*******************************************************************************
* Function Name  : DFU_Status_Out
* Description    : DFU status OUT routine.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DFU_Status_Out (void)
{
  DEVICE_INFO *pInfo = &Device_Info;
  u32 i, Addr, DATA;
  if (pInfo->USBbRequest == DFU_GETSTATUS)
  {

    if (DeviceState == STATE_dfuDNBUSY)
    {
      if (wBlockNum == 0)   /* Decode the Special Command*/
      {

        if ((Load_Buffer[0] == CMD_GETCOMMANDS) && (wlength == 1))
        {
        }
        else if  ((Load_Buffer[0] == CMD_SETADDRESSPOINTER) && (wlength == 5))
        {
          Pointer  = Load_Buffer[1];
          Pointer += Load_Buffer[2] << 8;
          Pointer += Load_Buffer[3] << 16;
          Pointer += Load_Buffer[4] << 24;
        }
        else if ((Load_Buffer[0] ==  CMD_ERASE) && (wlength == 5))
        {
          Pointer  = Load_Buffer[1];
          Pointer += Load_Buffer[2] << 8;
          Pointer += Load_Buffer[3] << 16;
          Pointer += Load_Buffer[4] << 24;

          if  ( Pointer < 0x60000000 )  /* Internal Flash */
          {
            Internal_FLASH_WritePrConfig(Internal_FLASH_SectorMask(Pointer), DISABLE);
            Internal_FLASH_SectorErase(Internal_FLASH_SectorMask(Pointer));
          }
          else
          {
            External_FLASH_SectorErase (Pointer);
          }
        }

      }
      else if (wBlockNum > 1)  // Download Command
      {
        Addr = ((wBlockNum - 2) * wTransferSize) + Pointer;
        if  ( wlength & 0x3) /* Not an aligned data */
        {
          for (i = wlength; i < ((wlength & 0xFFFC) + 4) ; i++)
          {
            Load_Buffer[i] = 0xFF;
          }
        }
        /* Data received are Word multiple */
        for (i = 0; i < wlength; i = i + 4)
        {
          DATA  = Load_Buffer[i];
          DATA += Load_Buffer[i+1] << 8;
          DATA += Load_Buffer[i+2] << 16;
          DATA += Load_Buffer[i+3] << 24;

          if (Addr < 0x60000000)   /* Internal flash programming */
          {
            Internal_FLASH_WritePrConfig(FLASH_BANK0_MODULE , DISABLE);
            Internal_FLASH_WordWrite(Addr, DATA);
          }
          else
          {
            External_FLASH_WordWrite(Addr, DATA);
          }
          Addr += 4;
        }
      }
      wlength = 0;
      wBlockNum = 0;

      DeviceState =  STATE_dfuDNLOAD_SYNC;
      DeviceStatus[4] = DeviceState;
      DeviceStatus[1] = 0;
      DeviceStatus[2] = 0;
      DeviceStatus[3] = 0;
      return;
    }
    else if (DeviceState == STATE_dfuMANIFEST)/* Manifestation in progress*/
    {
      DFU_write_crc();
      return;
    }
  }
  return;
}

/*******************************************************************************
* Function Name  : DFU_Data_Setup
* Description    :
* Input          : RequestNb
* Output         : None
* Return         : USB_SUCCESS or USB_UNSUPPORT
*******************************************************************************/
RESULT DFU_Data_Setup(u8 RequestNo)
{
  u8 *(*CopyRoutine)(u16);
  CopyRoutine = NULL;

  if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
  {
    if (RequestNo == DFU_UPLOAD && (DeviceState == STATE_dfuIDLE 
        || DeviceState == STATE_dfuUPLOAD_IDLE ))
    {
      CopyRoutine = UPLOAD;
    }
    else if (RequestNo == DFU_DNLOAD && (DeviceState == STATE_dfuIDLE 
          || DeviceState == STATE_dfuDNLOAD_IDLE ))
    {
      DeviceState = STATE_dfuDNLOAD_SYNC;
      CopyRoutine = DNLOAD;
    }
    else if (RequestNo == DFU_GETSTATE)
    {
      CopyRoutine = GETSTATE;
    }
    else if (RequestNo == DFU_GETSTATUS)
    {
        CopyRoutine = GETSTATUS;
    }
    else 
    {
      return USB_UNSUPPORT;
    }
  }
  else 
  {
    return USB_UNSUPPORT;
  }
  if (CopyRoutine == NULL)
  {
    return USB_UNSUPPORT;
  } 

  pInformation->Ctrl_Info.CopyData = CopyRoutine;
  pInformation->Ctrl_Info.Usb_wOffset = 0;
  (*CopyRoutine)(0);

  return USB_SUCCESS;

} 

/*******************************************************************************
* Function Name  : DFU_NoData_Setup
* Description    :
* Input          : RequestNb
* Output         : None
* Return         : USB_SUCCESS or USB_UNSUPPORT
*******************************************************************************/
RESULT DFU_NoData_Setup(u8 RequestNo)
{
  if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
  {
    /*DFU_DNLOAD*/
    if (RequestNo == DFU_DNLOAD)
    {
      /* End of DNLOAD operation*/
      if (DeviceState == STATE_dfuDNLOAD_IDLE || DeviceState == STATE_dfuIDLE ) 
      {
        Manifest_State = Manifest_In_Progress;
        DeviceState = STATE_dfuMANIFEST_SYNC;
        DeviceStatus[1] = 0;
        DeviceStatus[2] = 0;
        DeviceStatus[3] = 0;
        DeviceStatus[4] = DeviceState;
        return USB_SUCCESS;
      }
    }
    /*DFU_UPLOAD*/
    else if (RequestNo == DFU_UPLOAD)
    {
      DeviceState = STATE_dfuIDLE;
      DeviceStatus[1] = 0;
      DeviceStatus[2] = 0;
      DeviceStatus[3] = 0;
      DeviceStatus[4] = DeviceState;
      return USB_SUCCESS;
    }
    /*DFU_CLRSTATUS*/
    else if (RequestNo == DFU_CLRSTATUS)
    {
      if (DeviceState == STATE_dfuERROR)
      {
        DeviceState = STATE_dfuIDLE;
        DeviceStatus[0] = STATUS_OK;/*bStatus*/
        DeviceStatus[1] = 0;
        DeviceStatus[2] = 0;
        DeviceStatus[3] = 0; /*bwPollTimeout=0ms*/
        DeviceStatus[4] = DeviceState;/*bState*/
        DeviceStatus[5] = 0;/*iString*/
      }
      else
      {   /*State Error*/
        DeviceState = STATE_dfuERROR;
        DeviceStatus[0] = STATUS_ERRUNKNOWN;/*bStatus*/
        DeviceStatus[1] = 0;
        DeviceStatus[2] = 0;
        DeviceStatus[3] = 0; /*bwPollTimeout=0ms*/
        DeviceStatus[4] = DeviceState;/*bState*/
        DeviceStatus[5] = 0;/*iString*/
      }
      return USB_SUCCESS;
    }
    /*DFU_ABORT*/
    else if (RequestNo == DFU_ABORT)
    {
      if (DeviceState == STATE_dfuIDLE || DeviceState == STATE_dfuDNLOAD_SYNC
          || DeviceState == STATE_dfuDNLOAD_IDLE 
          || DeviceState == STATE_dfuMANIFEST_SYNC
          || DeviceState == STATE_dfuUPLOAD_IDLE )
      {
        DeviceState = STATE_dfuIDLE;
        DeviceStatus[0] = STATUS_OK;
        DeviceStatus[1] = 0;
        DeviceStatus[2] = 0;
        DeviceStatus[3] = 0; /*bwPollTimeout=0ms*/
        DeviceStatus[4] = DeviceState;
        DeviceStatus[5] = 0;/*iString*/
        wBlockNum = 0;
        wlength = 0;
      }
      return USB_SUCCESS;
    }
  }
  return USB_UNSUPPORT;
}

/*******************************************************************************
* Function Name  : DFU_GetDeviceDescriptor.
* Description    : Gets the device descriptor.
* Input          : Length.
* Output         : None.
* Return         : The address of the device descriptor.
*******************************************************************************/
u8 *DFU_GetDeviceDescriptor(u16 Length)
{
  return Standard_GetDescriptorData(Length, &Device_Descriptor);
}
 
 /*******************************************************************************
* Function Name  : DFU_GetConfigDescriptor.
* Description    : Gets the configuration descriptor.
* Input          : Length.
* Output         : None.
* Return         : The address of the configuration discriptor.
*******************************************************************************/
u8 *DFU_GetConfigDescriptor(u16 Length)
{
  return Standard_GetDescriptorData (Length, &Config_Descriptor);
}

/*******************************************************************************
* Function Name  : DFU_GetStringDescriptor.
* Description    : Gets the string descriptors according to the needed index.
* Input          : Length.
* Output         : None.
* Return         : The address of the string descriptors.
*******************************************************************************/
u8 *DFU_GetStringDescriptor(u16 Length)
{
  u8 wValue0 = pInformation->USBwValue0;

  if (wValue0 > 6)
  {
    return NULL;
  }
  else
  {
    return Standard_GetDescriptorData(Length, &DFU_String_Descriptor[wValue0]);
  }
}

/*******************************************************************************
* Function Name  : DFU_Get_Interface_Setting
* Description    :
* Input          : InterfaceNb / AlternateNb
* Output         : None
* Return         : USB_SUCCESS or USB_UNSUPPORT
*******************************************************************************/
RESULT DFU_Get_Interface_Setting(u8 Interface, u8 AlternateSetting)
{
  if (AlternateSetting > 2)
  {
    return USB_UNSUPPORT;   
    /* In this application we don't have more tha 3 AlternateSetting */

  }
  else if (Interface > 0) 
  {
    return USB_UNSUPPORT; /* In this application we have only 1 interfaces */
  }

  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : UPLOAD
* Description    : Upload routine.
* Input          : Length
* Output         : None
* Return         : Pointer to data
*******************************************************************************/
u8 *UPLOAD(u16 Length)
{
  DEVICE_INFO  *pInfo = &Device_Info;
  u8         B1, B0;
  u16       offset, returned;
  u8 *Phy_Addr;
  u32 Addr;

  B0 = pInfo->USBwValues.bw.bb0;
  B1 = pInfo->USBwValues.bw.bb1;
  wBlockNum = (u16)B1;
  wBlockNum = wBlockNum * 0x100;
  wBlockNum += (u16)B0;          /* wBlockNum value updated*/

  B0 = pInfo->USBwLengths.bw.bb0;
  B1 =  pInfo->USBwLengths.bw.bb1;
  wlength = (u16)B0;
  wlength = wlength * 0x100;
  wlength += (u16)B1;           /* wlength value updated*/

  offset = pInformation->Ctrl_Info.Usb_wOffset;

  if (wBlockNum == 0)  /* Get Command */
  {
    if (wlength > 3)
    {
      DeviceState = STATE_dfuIDLE ;
    }
    else 
    {
      DeviceState = STATE_dfuUPLOAD_IDLE;
    }

    DeviceStatus[4] = DeviceState;
    DeviceStatus[1] = 0;
    DeviceStatus[2] = 0;
    DeviceStatus[3] = 0;

    Load_Buffer[0] = CMD_GETCOMMANDS;
    Load_Buffer[1] = CMD_SETADDRESSPOINTER;
    Load_Buffer[2] = CMD_ERASE;

    if (Length == 0)
    {
      pInformation->Ctrl_Info.Usb_wLength = 3 ;
      return NULL;
    }

    return(&Load_Buffer[0]);


  }
  else if (wBlockNum > 1)
  {
    DeviceState = STATE_dfuUPLOAD_IDLE ;
    DeviceStatus[4] = DeviceState;
    DeviceStatus[1] = 0;
    DeviceStatus[2] = 0;
    DeviceStatus[3] = 0;

    Addr = ((wBlockNum - 2) * wTransferSize) + Pointer; 
    Phy_Addr = (u8*)(Addr);

    returned = wlength - offset;

    if (Length == 0)
    {
      pInformation->Ctrl_Info.Usb_wLength = returned ;
      return 0;
    }
    return(Phy_Addr + offset);
  }
  else  /* unsupported wBlockNum */
  {
    DeviceState = STATUS_ERRSTALLEDPKT;
    DeviceStatus[4] = DeviceState;
    DeviceStatus[1] = 0;
    DeviceStatus[2] = 0;
    DeviceStatus[3] = 0;
    return NULL;
  }
}

/*******************************************************************************
* Function Name  : DNLOAD
* Description    : Download routine.
* Input          : Length.
* Output         : None.
* Return         : Pointer to data.
*******************************************************************************/
u8 *DNLOAD (u16 Length)
{
  DEVICE_INFO  *pInfo = &Device_Info;
  u8         B1, B0;
  u16       offset, returned;

  B0 = pInfo->USBwValues.bw.bb0;
  B1 = pInfo->USBwValues.bw.bb1;
  wBlockNum = (u16)B1;
  wBlockNum = wBlockNum * 0x100;
  wBlockNum += (u16)B0;
  B0 = pInfo->USBwLengths.bw.bb0;
  B1 =  pInfo->USBwLengths.bw.bb1;
  wlength = (u16)B0;
  wlength = wlength * 0x100;
  wlength += (u16)B1;

  offset = pInfo->Ctrl_Info.Usb_wOffset;

  DeviceState = STATE_dfuDNLOAD_SYNC;
  DeviceStatus[4] = DeviceState;

  returned = wlength - offset;

  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = returned ;
    return NULL;
  }
  return((u8*)Load_Buffer + offset);
}

/*******************************************************************************
* Function Name  : GETSTATE.
* Description    :	Get State request routine.
* Input          : Length.
* Output         : None.
* Return         : Pointer to data.
*******************************************************************************/
u8 *GETSTATE(u16 Length)
{
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = 1 ;
    return NULL;
  }
  else
  {
    return( &DeviceState);
  }
}

/*******************************************************************************
* Function Name  : GETSTATUS.
* Description    : Get Status request routine.
* Input          : Length.
* Output         : None.
* Return         : Pointer to data.
*******************************************************************************/
u8 *GETSTATUS(u16 Length)
{
  switch ( DeviceState )
  {
    case   STATE_dfuDNLOAD_SYNC  :
      if (wlength != 0)
      {
        DeviceState = STATE_dfuDNBUSY;
        DeviceStatus[4] = DeviceState;
        if ((wBlockNum == 0) && (Load_Buffer[0] == CMD_ERASE))
        {
          if ( ((Pointer >= 0x40000000) && (Pointer <= 0x40006000)) ||
               ((Pointer >= 0x400C0000) && (Pointer <= 0x400C2000)))  
          { /* 8K sectors */
            DeviceStatus[1] = 0xE8;   /* 1 second */
            DeviceStatus[2] = 0x03;
            DeviceStatus[3] = 0x00;
          }
          else if ( Pointer == 0x40008000 ) /* 32K sector */
          {
            DeviceStatus[1] = 0xD0;  /* 2 seconds */
            DeviceStatus[2] = 0x07;
            DeviceStatus[3] = 0x00;
          }
          else if  ((Pointer >= 0x40010000) && (Pointer <= 0x40030000)) 
          { /* 64K sectors */
            DeviceStatus[1] = 0xA0;  /* 4 seconds */
            DeviceStatus[2] = 0x0F;
            DeviceStatus[3] = 0x00;
          }
        }
        else
        {
          DeviceStatus[1] = wlength >> 4 ; /* 62 µs x  Nb of words */
          DeviceStatus[2] = 0;
          DeviceStatus[3] = 0;
        }
        break;
      }
      else  /* (wlength==0)*/
      {
        DeviceState = STATE_dfuDNLOAD_IDLE;
        DeviceStatus[4] = DeviceState;
        DeviceStatus[1] = 0;
        DeviceStatus[2] = 0;
        DeviceStatus[3] = 0;
        break;
      }
    case   STATE_dfuMANIFEST_SYNC :
      if (Manifest_State == Manifest_In_Progress)
      {
        DeviceState = STATE_dfuMANIFEST;
        DeviceStatus[4] = DeviceState;
        DeviceStatus[1] = 1;             /*bwPollTimeout = 1ms*/
        DeviceStatus[2] = 0;
        DeviceStatus[3] = 0;
        break;
      }

      else if (Manifest_State == Manifest_complete 
               && DFU_ConfigDescriptor[20] & 0x04)
      {
        DeviceState = STATE_dfuIDLE;
        DeviceStatus[4] = DeviceState;
        DeviceStatus[1] = 0;
        DeviceStatus[2] = 0;
        DeviceStatus[3] = 0;
        break;
      }
    default :
      break;
  }
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = 6 ;
    return 0;
  }
  else
  {
    return(&(DeviceStatus[0]));
  }
}

/*******************************************************************************
* Function Name  : DFU_write_crc.
* Description    : DFU Write CRC routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void DFU_write_crc(void)
{
  /*Internal_FLASH_WordWrite(0x400C3FFC,0x6D6D6D6D);*/
  Manifest_State = Manifest_complete;
  if (DFU_ConfigDescriptor[20]&0x04)
  {
    DeviceState = STATE_dfuMANIFEST_SYNC;
    DeviceStatus[4] = DeviceState;
    DeviceStatus[1] = 0;
    DeviceStatus[2] = 0;
    DeviceStatus[3] = 0;
    return;
  }
  else
  {
    DeviceState = STATE_dfuMANIFEST_WAIT_RESET;
    DeviceStatus[4] = DeviceState;
    DeviceStatus[1] = 0;
    DeviceStatus[2] = 0;
    DeviceStatus[3] = 0;

    Reset_Device();
    return;
  }
} 

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/


