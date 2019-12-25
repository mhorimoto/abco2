void rtc_util(void) {
  int  sec;
  char buf[16];
  int i,aval,smenu;
  float ltt,lgt;
  char ntpsrv[32];
  extern char lcdtext[];

  aval = analogRead(2);
  smenu = (aval/100)+1;
  Reset_lcdtext();
  switch(smenu) {
  case 1:
  case 2:
  case 3:
  case 4:
    _rtc_util_1(aval);
    break;
  case 5:
  case 6:
    lcd.setCursor(0,0);
    lcd.print("LATITUDE        ");
    ltt = atmem.readFloat(ATMEM_LTT);
    Reset_lcdtext();
    ConvertFloatChar(ltt,&buf[0]);
    sprintf(lcdtext,"        %8s",buf);
    break;
  case 7:
  case 8:
    lcd.setCursor(0,0);
    lcd.print("LONGITUDE       ");
    lgt = atmem.readFloat(ATMEM_LGT);
    Reset_lcdtext();
    ConvertFloatChar(lgt,&buf[0]);
    sprintf(lcdtext,"        %8s",buf);
    break;
  case 9:
  case 10:
    atmem.readChars(ATMEM_NTPSRV,ntpsrv,sizeof(ntpsrv));
    lcd.setCursor(0,0);
    lcd.print("NTPSRV          ");
    sprintf(lcdtext,"%16s",ntpsrv);
    break;
  }
  lcd.setCursor(0,1);
  lcd.print(lcdtext);
}

void _rtc_util_1(int aval) {
  int sec;
  lcd.setCursor(0,0);
  lcd.print("RTC UTIL        ");
  RtcDateTime now = Rtc.GetDateTime();
  Reset_lcdtext();
  sec = now.Second();
  switch(sec) {
  case 0:
  case 1:
  case 2:
  case 15:
  case 16:
  case 17:
  case 30:
  case 31:
  case 32:
  case 45:
  case 46:
  case 47:
    sprintf(lcdtext,"%4d/%02d/%02d      ",now.Year(),now.Month(),now.Day());
    break;
  default:
    sprintf(lcdtext,"%02d:%02d:%02d        ",now.Hour(),now.Minute(),now.Second());
    break;
  }
}
