void disp_select(int menusel) {
  int smenu;
  int pday,psec;
  extern char *StrMODE[];
  //  Serial.println("64");
  smenu = (menusel/100)+1;
  if ((smenu>=1) and (smenu<=3)) {
    RtcDateTime now = Rtc.GetDateTime();
    Reset_lcdtext();
    if (pday!=now.Day()) {
      pday = now.Day();
      sprintf(lcdtext,"D=%4d/%02d/%02d    ",now.Year(),now.Month(),pday);
      lcd.setCursor(0,0);
      lcd.print(lcdtext);
    }
    if (psec!=now.Second()) {
      psec = now.Second();
      sprintf(lcdtext,"T=%02d:%02d:%02d      ",now.Hour(),now.Minute(),psec);
      lcd.setCursor(0,1);
      lcd.print(lcdtext);
    }
  } else if ((smenu>=4) and (smenu<=6)) {
    Reset_lcdtext();
    lcd.setCursor(0,1);
    sprintf(lcdtext,"%16s",StrMODE[3]);
    lcd.print(lcdtext);
  } else {
    lcd.setCursor(0,0);
    sprintf(lcdtitle,"ABCO2 %6s",VERSION);
    lcd.print(lcdtitle);
    lcd.setCursor(0,1);
    lcd.print(Ethernet.localIP());
  // case 5:
  // case 6:
  // case 7:
  // case 8:
  // case 9:
  // case 10:
  }
  //  Serial.println("-64");
}
