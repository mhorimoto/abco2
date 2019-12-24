#include <EEPROM.h>

// Set MAC Address at EEPROM 4090-4095 for MEGA
//                    EEPROM 1018-1023 for UNO

#define MEGA_PTR  4090
#define UNO_PTR   1018


void setup() {
  int startptr = UNO_PTR;
  EEPROM.write(startptr,0x02);
  EEPROM.write(startptr+1,0xA2);
  EEPROM.write(startptr+2,0x73);
  EEPROM.write(startptr+3,0xFF);
  EEPROM.write(startptr+4,0x00);
  EEPROM.write(startptr+5,0x03);
}

void loop() {
}
