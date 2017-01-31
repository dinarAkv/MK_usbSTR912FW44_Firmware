 /******************** (C) Private *********************************************
* File Name          : usb_user.c
* Author             : D
* Version            : V1.0.0
* Date               : 20/01/2017
* Description        : Interface function for user to work with usb.
********************************************************************************/









#include "91x_type.h"
#include "usb_user.h"
#include "usb_conf.h"
#include "usb_regs.h"
#include "usb_desc.h"



/* Buffer received from host to device. */
extern u8 buffer_out[VIRTUAL_COM_PORT_DATA_SIZE];
/* Length of received data. */
extern u32 count_out;





/**********************************************************************
*	Function name		: sentDataFromDevToHost. 
* 	This function		: Send data via usb from device to host.
*	Input				: data_buffer - Array of bytes.
*						  Nb_bytes - Number of bytes in data_buffer.
*	Return				: None.					  
***********************************************************************/
void sentDataFromDevToHost(u8* data_buffer, u8 Nb_bytes)
{
  UserToPMABufferCopy(data_buffer, ENDP1_TXADDR, Nb_bytes);
  SetEPTxCount(ENDP1, Nb_bytes);
  SetEPTxValid(ENDP1);
}

/**********************************************************************
*	Function name		: getDataFromHostToDev. 
* 	This function		: Get data array that has received.
*	Input				: None.
*	Return				: Received array of bytes.		
***********************************************************************/
u8* getDataFromHostToDev()
{
	return buffer_out;
}

/**********************************************************************
*	Function name		: getReceivedDataLen. 
* 	This function		: Get length of received array of bytes.
*	Input				: None.
*	Return				: Length of received array of bytes.		
***********************************************************************/
u8 getReceivedDataLen()
{
	return count_out;	
}


/**********************************************************************
*	Function name		: receivedDataHandler. 
* 	This function		: Interrupr service handler when 
*						  data has received. There is able to call user
*						  defined functions from this function.
*	Input				: None.
*	Return				: None.		
***********************************************************************/
void receivedDataHandler()
{
	/*	This is example of code that realize device callback.
	 	When data come from host the same data send back from device
	 	to host .*/
	sentDataFromDevToHost(getDataFromHostToDev(), getReceivedDataLen());	
}