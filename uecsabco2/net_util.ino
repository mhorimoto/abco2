void net_init(void) {
  extern byte MACAddress[];
  Ethernet.init(53); // MEGA
  Ethernet.begin(MACAddress);
}

void net_util(void) {
  char *buf;
  int i,aval,smenu;
  extern byte MACAddress[];
  extern char lcdtext[];

  aval = analogRead(2);
  smenu = (aval/100)+1;
  Reset_lcdtext();
  switch(smenu) {
  case 1: // IP Address
    buf = "IP Address      ";
    IPv4toChar(Ethernet.localIP(),&lcdtext[0],16);
    break;
  case 2: // Netmask
    buf = "Netmask         ";
    IPv4toChar(Ethernet.subnetMask(),&lcdtext[0],16);
    lcd.print(lcdtext);
    break;
  case 3: // Gateway
    buf = "Gateway         ";
    IPv4toChar(Ethernet.gatewayIP(),&lcdtext[0],16);
    break;
  case 4: // DNS
    buf = "DNS             ";
    IPv4toChar(Ethernet.dnsServerIP(),&lcdtext[0],16);
    break;
  case 5: // MAC Address
    buf = "MAC Address     ";
    sprintf(lcdtext,"%02X%02X.%02X%02X.%02X%02X",
	    MACAddress[0],MACAddress[1],MACAddress[2],
	    MACAddress[3],MACAddress[4],MACAddress[5]);
    break;
  }
  lcd.setCursor(0,0);
  lcd.print(buf);
  lcd.setCursor(0,1);
  lcd.print(lcdtext);
}

uint32_t IPv4toChar(uint32_t ip,char *buf,int n) {
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
  t = strlen(o);
  strncat(o,"        ",n-t);
  return(ip);
}
