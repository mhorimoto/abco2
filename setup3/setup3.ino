// Ethernet initialize
//  MAC Address store in EEPROM 4090-4095

#include <EEPROM.h>
#include <SPI.h>
#include <Ethernet2.h>
#define  MAC_ADDR_PTR   0xffa   // DEC=4090 for MEGA
//#define  MAC_ADDR_PTR   0x3fa   // DEC=1018 for UNO

byte mac[6];
byte ipa[4];

//IPAddress  ip(192,168,31,159);
EthernetClient client;

void setup() {
  int i;
  char buf[256];
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  Serial.begin(115200);
  for(i=0;i<6;i++) {
    mac[i] = EEPROM.read(MAC_ADDR_PTR+i);
  }
  sprintf(buf,"TRY DHCP with MAC=%02X:%02X:%02X:%02X:%02X:%02X",
	  mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
  Serial.println(buf);
  Ethernet.init(53); // MEGA
  Ethernet.begin(mac);
  //  if (Ethernet.begin(mac)==0) {
  //    digitalWrite(13,HIGH);
  //    for(;;) {;}
  //  } else {
    Serial.print("IP Address is ");
    Serial.println(Ethernet.localIP());
    Serial.end();
    //  }
}
void loop() {
}
  
  
