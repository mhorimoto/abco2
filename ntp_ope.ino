#define LEAP_YEAR(Y)     ( ((1970+(Y))>0) && !((1970+(Y))%4) && ( ((1970+(Y))%100) || !((1970+(Y))%400) ) )

int ntp_ope() {
  RtcDateTime ntpset;
  unsigned long highWord,lowWord,secsSince1900,epoch,localepoch,setepoch;
  unsigned int xyear,xmonth,xdays,xday,xwday,xhour,xminute,xsecond;
  unsigned int monthLength;
  // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
  const unsigned long seventyYears = 2208988800UL;
  static  const uint8_t monthDays[]={31,28,31,30,31,30,31,31,30,31,30,31}; // API starts months from 1, this array starts from 0

  atmem.readChars(NTPSERVER_NAME,&ntpServer[0],NTPSERVER_NAME_LENGTH);
  ntp.begin(NTPLOCAL_PORT);
  lcd.setCursor(0,1);
  lcd.print(F("ENTER NTP       "));
  wdt_reset();
  sendNTPpacket(ntpServer); // send an NTP packet to a time server
  wdt_reset();
  // wait to see if a reply is available
  if ( ntp.parsePacket() ) {
    wdt_reset();
    // We've received a packet, read the data from it
    ntp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
    wdt_reset();
    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:
    highWord = word(packetBuffer[40], packetBuffer[41]);
    lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    secsSince1900 = highWord << 16 | lowWord;
    // now convert NTP time into everyday time:
    // subtract seventy years:
    epoch = secsSince1900 - seventyYears;
    // print Unix time:
    localepoch = epoch + (9*60*60); // For JST +9:00
    setepoch = localepoch;
    xsecond = localepoch % 60;
    localepoch /= 60; // now it is minutes
    xminute = localepoch % 60;
    localepoch /= 60; // now it is hours
    xhour = localepoch % 24;
    localepoch /= 24; // now it is days
    xwday = ((localepoch + 4) % 7) + 1;  // Sunday is day 1 

    xyear = 0;  
    xdays = 0;
    while((unsigned)(xdays += (LEAP_YEAR(xyear) ? 366 : 365)) <= localepoch) {
      xyear++;
    }
    xdays -= LEAP_YEAR(xyear) ? 366 : 365;
    localepoch  -= xdays; // now it is days in this year, starting at 0
    xdays=0;
    xmonth=0;
    monthLength=0;
    for (xmonth=0; xmonth<12; xmonth++) {
      if (xmonth==1) { // february
	if (LEAP_YEAR(xyear)) {
	  monthLength=29;
	} else {
	  monthLength=28;
	}
      } else {
	monthLength = monthDays[xmonth];
      }
      if (localepoch >= monthLength) {
	localepoch -= monthLength;
      } else {
        break;
      }
    }
    xmonth++;  // jan is month 1  
    xday = localepoch + 1;     // day of month
    xyear += 1970;
    ntpset = RtcDateTime(xyear,xmonth,xday,xhour,xminute,xsecond);
    Rtc.SetDateTime(ntpset);
    if (!Rtc.GetIsRunning()) {
      lcd.setCursor(0,1);
      lcd.print(F("RTC RESTART     "));
      Rtc.SetIsRunning(true);
    }
    lcd.setCursor(0,1);
    lcd.print(F("NTP SET OK      "));
    delay(800);
    return(0);   // NORMAL
  } else {
    lcd.setCursor(0,1);
    lcd.print(F("NTP ERROR       "));
    delay(800);
    return(-1);  // ERROR
  }
}

// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(char* address) {
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  ntp.beginPacket(address, 123); //NTP requests are to port 123
  ntp.write(packetBuffer, NTP_PACKET_SIZE);
  ntp.endPacket();
  lcd.setCursor(0,1);
  lcd.print(F("SEND-NTP END    "));
  delay(800);
}
