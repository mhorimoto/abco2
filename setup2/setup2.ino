#include <EEPROM.h>

// Set MAC Address at EEPROM 4090-4095

void setup() {
  EEPROM.write(4090,0x02);
  EEPROM.write(4091,0xA2);
  EEPROM.write(4092,0x73);
  EEPROM.write(4093,0xFF);
  EEPROM.write(4094,0x00);
  EEPROM.write(4095,0x01);
  EEPROM.write(4,0xFF);
}

void loop() {
}
