#include "setup1.h"

void test_input(void) {
  int di ;
  char *title,*msg;
  int i,aval,smenu;
  extern char lcdtext[];

  aval = analogRead(2);
  smenu = (aval/170)+1;
  Reset_lcdtext();
  switch(smenu) {
  case 1:
    title = "WATER LEVEL-1   ";
    di = digitalRead(WLVL1);
    break;
  case 2:
    title = "WATER LEVEL-2   ";
    di = digitalRead(WLVL2);
    break;
  case 3:
    title = "AIR PRESSURE-1  ";
    di = digitalRead(PRSLVL1);
    break;
  case 4:
    title = "AIR PRESSURE-2  ";
    di = digitalRead(PRSLVL2);
    break;
  case 5:
    title = "BURNER ON/OFF   ";
    di = digitalRead(BURNER);
    break;
  case 6:
    title = "ALARM MISSFIRE  ";
    di = digitalRead(ALRM_MISSFIRE);
    break;
  default:
    break;
  }
  if (di==LOW) {
    msg = "LOW             ";
  } else {
    msg = "HIGH            ";
  }
  lcd.setCursor(0,0);
  lcd.print(title);
  lcd.setCursor(0,1);
  lcd.print(msg);
}
