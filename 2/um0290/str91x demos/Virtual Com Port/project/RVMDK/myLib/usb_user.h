 /******************** (C) Private *********************************************
* File Name          : usb_user.h
* Author             : D
* Version            : V1.0.0
* Date               : 20/01/2017
* Description        : Contain prototypes used in usb interface 
*					   functions for user.
********************************************************************************



 /* Select appropriate bit. */
#define BIT(shift) (1<<shift)


/*	Prototype declaration. */
void sentDataFromDevToHost(u8* , u8 );
u8* getDataFromHostToDev();
u8 getReceivedDataLen();
void receivedDataHandler();