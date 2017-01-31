# MK_usbSTR912FW44_Firmware
This repository contain example of firmware for usb peripheral as virtual com port.
Firmware has tasted on microcontroller STR912FW44. 

Firmware programmed on the base of example rendered by STMicroelectronics.

How to work firmware. When user from host through terminal send message (sequence of bytes,
for example: 0xFF 0xFE 0xFD) to device (STR912FW44). After device received message it callback this message
to the host. So user get the same message.
