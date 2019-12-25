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
  char txt_ip[17],txt_mask[17],txt_gw[17],txt_dns[17],buf[256];
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  Serial.begin(115200);
  for(i=0;i<6;i++) {
    mac[i] = EEPROM.read(MAC_ADDR_PTR+i);
  }
  sprintf(buf,"TRY DHCP with MAC=%02X:%02X:%02X:%02X:%02X:%02X",
	  mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
  Serial.println(buf);
  Serial.end();
  Ethernet.init(53); // MEGA
  Ethernet.begin(mac);
  InfoOut();
}
void loop() {
  delay(10000);
}

void InfoOut(void) {
  char txt_ip[17],txt_mask[17],txt_gw[17],txt_dns[17],buf[256];
  Serial.begin(115200);
  IPv4toChar(Ethernet.localIP(),&txt_ip[0]);
  IPv4toChar(Ethernet.subnetMask(),&txt_mask[0]);
  IPv4toChar(Ethernet.gatewayIP(),&txt_gw[0]);
  IPv4toChar(Ethernet.dnsServerIP(),&txt_dns[0]);
  sprintf(buf,"IP Address:%s/%s\nGateway:%s\nDNS:%s\n",txt_ip,txt_mask,txt_gw,txt_dns);
  Serial.print(buf);
  Serial.end();
}

  
String IPv4toString(uint32_t ip){
  String result = "";
  result += String((ip & 0xFF), 10);
  result += ".";
  result += String((ip & 0xFF00) >> 8, 10);
  result += ".";
  result += String((ip & 0xFF0000) >> 16, 10);
  result += ".";
  result += String((ip & 0xFF000000) >> 24, 10);
  return result;
}

uint32_t IPv4toChar(uint32_t ip,char *buf) {
  int t;
  char *o;
  o = buf;
  t = ip & 0xff;
  sprintf(o,"%d.",t);

  o += strlen(o);
  t = (ip & 0xff00) >> 8;
  sprintf(o,"%d.",t);

  o += strlen(o);
  t = (ip & 0xff0000) >> 16;
  sprintf(o,"%d.",t);

  o += strlen(o);
  t = (ip & 0xff000000) >> 24;
  sprintf(o,"%d",t);
  return(ip);
}

