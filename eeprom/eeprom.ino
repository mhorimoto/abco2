#include <EEPROM.h>
#include <SPI.h>
#include <Ethernet2.h>
#define  MAC_ADDR_PTR   0xffa   // DEC=4090 for MEGA
//#define  MAC_ADDR_PTR   0x3fa   // DEC=1018 for UNO

byte mac[6];
byte ipa[4];

