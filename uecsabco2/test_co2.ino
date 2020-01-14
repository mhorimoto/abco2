void test_co2(void) {
  char *buf;
  int rc,co2,aval,smenu;
  extern char lcdtext[];
  aval = analogRead(2);
  smenu = (aval/512)+1;
  Reset_lcdtext();
  co2 = 0;
  rc  = 1;
  switch(smenu) {
  case 1: // K33ICB
    buf = "CO2 K33-ICB     ";
    rc = k33icb.readCO2(co2);
    break;
  case 2: // K33LP
    buf = "CO2 K33-LP      ";
    rc = k33lp.readCO2(co2);
    break;
  }
  if (rc==0) { // NORMAL
    sprintf(lcdtext,"%4d ppm        ",co2);
  } else {
    sprintf(lcdtext," ERRCODE=%2d   ",rc);
  }
  lcd.setCursor(0,0);
  lcd.print(buf);
  lcd.setCursor(0,1);
  lcd.print(lcdtext);
}
