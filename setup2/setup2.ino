#include <EEPROM.h>
#include <Wire.h>
#include <AT24CX.h>

AT24CX mem(7,32);

// Set MAC Address at EEPROM 4090-4095 for MEGA
//                    EEPROM 1018-1023 for UNO

#define EEPROM_PTR  4090 // MEGA
//#define EEPROM_PTR   1018 // UNO

float Longitude = 130.214750;             // 経度（東経を入力）
float Latitude  = 33.592735;              // 緯度（北緯を入力）
char ntpserver[] = "ntp.nict.jp";

void setup() {
  float lgt,ltt;
  char ntpsrv[32];
  // EEPROM.write(EEPROM_PTR,0x02);
  // EEPROM.write(EEPROM_PTR+1,0xA2);
  // EEPROM.write(EEPROM_PTR+2,0x73);
  // EEPROM.write(EEPROM_PTR+3,0xFF);
  // EEPROM.write(EEPROM_PTR+4,0x00);
  // EEPROM.write(EEPROM_PTR+5,0x03);
  //  mem.writeFloat(0xfd0,Latitude);
  //  mem.writeFloat(0xfd8,Longitude);
  //  mem.writeChars(0xfe0,ntpserver,sizeof(ntpserver));
  ltt = mem.readFloat(0xfd0);
  lgt = mem.readFloat(0xfd8);
  mem.readChars(0xfe0,ntpsrv,sizeof(ntpsrv));
  Serial.begin(115200);
  Serial.print("LATITUDE:");
  Serial.println(ltt,DEC);
  Serial.print("LONGITUDE:");
  Serial.println(lgt,DEC);
  Serial.print("NTPSERVER:");
  Serial.println(ntpsrv);
  Serial.end();
}

void loop() {
}
