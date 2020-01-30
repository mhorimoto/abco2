void disp_select(int menusel) {
  int smenu;
  int pday,psec;
  smenu = (menusel/100)+1;
  //Serial.begin(115200);
  //Serial.println(smenu);
  if ((smenu>=1) and (smenu<=3)) {
    //Serial.println("TOD");
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
  } else {
    //Serial.println("Default");
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
  //Serial.end();

}
