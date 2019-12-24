#include "setup1.h"

void test_output() {
  int i,aval,smenu;
  aval = analogRead(2);
  smenu = (aval/100)+1;
  switch(smenu) {
  case 1: // BLOWER TEST
    _test_output(D_BLOWER,"BLOWER");
    break;
  case 2: // PUMP
    _test_output(D_PUMP,"PUMP");
    break;
  case 3: // SPARE
    _test_output(D_SPARE,"SPARE");
    break;
  case 4: // VLV1
  }
}

void _test_output(int p,char *name) {
  int d;
  char lcdtext[17];
  extern int output_test_toggle;
  sprintf(lcdtext,"%9s TEST  ",name);
  lcd.setCursor(0,0);
  lcd.print(lcdtext);
  d = digitalRead(p);
  if (d==HIGH) {
    sprintf(lcdtext,"%9s IS ON ",name);
    if (output_test_toggle==1) {
      digitalWrite(p,LOW);
      output_test_toggle = 0;
    }
  } else {
    sprintf(lcdtext,"%9s IS OFF",name);
    if (output_test_toggle==1) {
      digitalWrite(p,HIGH);
      output_test_toggle = 0;
    }
  }
  lcd.setCursor(0,1);
  lcd.print(lcdtext);
}

void _test_output_vlv(int p,int ols,int sls,char *name) {
  int d;
  char lcdtext[17];
  extern int output_test_toggle;
  sprintf(lcdtext,"%9s TEST  ",name);
  lcd.setCursor(0,0);
  lcd.print(lcdtext);
  d = digitalRead(p);
  if (d==HIGH) {
    sprintf(lcdtext,"%9s IS ON ",name);
    if (output_test_toggle==1) {
      digitalWrite(p,LOW);
      output_test_toggle = 0;
    }
  } else {
    sprintf(lcdtext,"%9s IS OFF",name);
    if (output_test_toggle==1) {
      digitalWrite(p,HIGH);
      output_test_toggle = 0;
    }
  }
  lcd.setCursor(0,1);
  lcd.print(lcdtext);
}
