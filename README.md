# MK_usbSTR912FW44_Firmware
This repository contain example of firmware for usb peripheral as virtual com port.
Firmware has tasted on microcontroller STR912FW44. 

Firmware programmed on the base of example rendered by STMicroelectronics.

How to work firmware. 
When user from host through terminal send message (sequence of bytes,
for example: 0xFF 0xFE 0xFD) to device (STR912FW44). After device received message it callback this message
to the host. So user get the same message.



Firmware code wrote with help of Keil IDE. 
Project for keil locates in:        '\2\um0290\str91x demos\Virtual Com Port\project\RVMDK'
Library for usb locates in:         '\2\um0290\USBLib'
Common library for STR9 locates in: '\2\um0290\FWLib\str91x'
My own built files locate in:       '\2\um0290\str91x demos\Virtual Com Port\project\RVMDK\myLib'
