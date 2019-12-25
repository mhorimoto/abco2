#include "setup1.h"

void test_output() {
  int i,aval,smenu;
  aval = analogRead(2);
  smenu = (aval/42)+1;
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
  case 4: // VLV1_OPEN
    _test_output_vlv(VLV1_OPEN,D_VLV1_OLS,D_VLV1_SLS,"VLV1 OPN");
    break;
  case 5: // VLV1_CLOSE
    _test_output_vlv(VLV1_CLOSE,D_VLV1_OLS,D_VLV1_SLS,"VLV1 CLS");
    break;
  case 6: // VLV1_UNKNOWN
    _test_output_vlv(VLV1_UNKNOWN,D_VLV1_OLS,D_VLV1_SLS,"VLV1 UNK");
    break;
  case 7: // VLV2_OPEN
    _test_output_vlv(VLV2_OPEN,D_VLV2_OLS,D_VLV2_SLS,"VLV2 OPN");
    break;
  case 8: // VLV2_CLOSE
    _test_output_vlv(VLV2_CLOSE,D_VLV2_OLS,D_VLV2_SLS,"VLV2 CLS");
    break;
  case 9: // VLV2_UNKNOWN
    _test_output_vlv(VLV2_UNKNOWN,D_VLV2_OLS,D_VLV2_SLS,"VLV2 UNK");
    break;
  case 10: // VLV3_OPEN
    _test_output_vlv(VLV3_OPEN,D_VLV3_OLS,D_VLV3_SLS,"VLV3 OPN");
    break;
  case 11: // VLV3_CLOSE
    _test_output_vlv(VLV3_CLOSE,D_VLV3_OLS,D_VLV3_SLS,"VLV3 CLS");
    break;
  case 12: // VLV3_UNKNOWN
    _test_output_vlv(VLV3_UNKNOWN,D_VLV3_OLS,D_VLV3_SLS,"VLV3 UNK");
    break;
  case 13: // VLV4_OPEN
    _test_output_vlv(VLV4_OPEN,D_VLV4_OLS,D_VLV4_SLS,"VLV4 OPN");
    break;
  case 14: // VLV4_CLOSE
    _test_output_vlv(VLV4_CLOSE,D_VLV4_OLS,D_VLV4_SLS,"VLV4 CLS");
    break;
  case 15: // VLV4_UNKNOWN
    _test_output_vlv(VLV4_UNKNOWN,D_VLV4_OLS,D_VLV4_SLS,"VLV4 UNK");
    break;
  case 16: // VLV5_OPEN
    _test_output_vlv(VLV5_OPEN,D_VLV5_OLS,D_VLV5_SLS,"VLV5 OPN");
    break;
  case 17: // VLV5_CLOSE
    _test_output_vlv(VLV5_CLOSE,D_VLV5_OLS,D_VLV5_SLS,"VLV5 CLS");
    break;
  case 18: // VLV5_UNKNOWN
    _test_output_vlv(VLV5_UNKNOWN,D_VLV5_OLS,D_VLV5_SLS,"VLV5 UNK");
    break;
  case 19: // VLV6_OPEN
    _test_output_vlv(VLV6_OPEN,D_VLV6_OLS,D_VLV6_SLS,"VLV6 OPN");
    break;
  case 20: // VLV6_CLOSE
    _test_output_vlv(VLV6_CLOSE,D_VLV6_OLS,D_VLV6_SLS,"VLV6 CLS");
    break;
  case 21: // VLV6_UNKNOWN
    _test_output_vlv(VLV6_UNKNOWN,D_VLV6_OLS,D_VLV6_SLS,"VLV6 UNK");
    break;
  case 22: // VLV7_OPEN
    _test_output_vlv(VLV7_OPEN,D_VLV7_OLS,D_VLV7_SLS,"VLV7 OPN");
    break;
  case 23: // VLV7_CLOSE
    _test_output_vlv(VLV7_CLOSE,D_VLV7_OLS,D_VLV7_SLS,"VLV7 CLS");
    break;
  case 24: // VLV7_UNKNOWN
    _test_output_vlv(VLV7_UNKNOWN,D_VLV7_OLS,D_VLV7_SLS,"VLV7 UNK");
    break;
  default:
    break;
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
  int d,olsv,slsv;
  char lcdtext[17];
  extern int output_test_toggle;
  sprintf(lcdtext,"%9s TEST  ",name);
  lcd.setCursor(0,0);
  lcd.print(lcdtext);
  olsv = digitalRead(ols);
  slsv = digitalRead(sls);
  if ((olsv==LOW)&&(slsv==LOW)) {
    sprintf(lcdtext,"%9s IS UNK",name);
  } else if ((olsv==LOW)&&(slsv==HIGH)) {
    sprintf(lcdtext,"%9s IS CLS",name);
  } else if ((olsv==HIGH)&&(slsv==LOW)) {
    sprintf(lcdtext,"%9s IS OPN",name);
  } else {
    sprintf(lcdtext,"%9s IS ERR",name);
  }
  if (output_test_toggle==1) {
    switch(p) {
    case VLV1_OPEN:
      digitalWrite(D_VLV1_NORM,HIGH);
      digitalWrite(D_VLV1_REV,LOW);
      break;
    case VLV1_CLOSE:
      digitalWrite(D_VLV1_NORM,LOW);
      digitalWrite(D_VLV1_REV,HIGH);
      break;
    case VLV1_UNKNOWN:
      digitalWrite(D_VLV1_NORM,LOW);
      digitalWrite(D_VLV1_REV,LOW);
      break;
    case VLV2_OPEN:
      digitalWrite(D_VLV2_NORM,HIGH);
      digitalWrite(D_VLV2_REV,LOW);
      break;
    case VLV2_CLOSE:
      digitalWrite(D_VLV2_NORM,LOW);
      digitalWrite(D_VLV2_REV,HIGH);
      break;
    case VLV2_UNKNOWN:
      digitalWrite(D_VLV2_NORM,LOW);
      digitalWrite(D_VLV2_REV,LOW);
      break;
    case VLV3_OPEN:
      digitalWrite(D_VLV3_NORM,HIGH);
      digitalWrite(D_VLV3_REV,LOW);
      break;
    case VLV3_CLOSE:
      digitalWrite(D_VLV3_NORM,LOW);
      digitalWrite(D_VLV3_REV,HIGH);
      break;
    case VLV3_UNKNOWN:
      digitalWrite(D_VLV3_NORM,LOW);
      digitalWrite(D_VLV3_REV,LOW);
      break;
    case VLV4_OPEN:
      digitalWrite(D_VLV4_NORM,HIGH);
      digitalWrite(D_VLV4_REV,LOW);
      break;
    case VLV4_CLOSE:
      digitalWrite(D_VLV4_NORM,LOW);
      digitalWrite(D_VLV4_REV,HIGH);
      break;
    case VLV4_UNKNOWN:
      digitalWrite(D_VLV4_NORM,LOW);
      digitalWrite(D_VLV4_REV,LOW);
      break;
    case VLV5_OPEN:
      digitalWrite(D_VLV5_NORM,HIGH);
      digitalWrite(D_VLV5_REV,LOW);
      break;
    case VLV5_CLOSE:
      digitalWrite(D_VLV5_NORM,LOW);
      digitalWrite(D_VLV5_REV,HIGH);
      break;
    case VLV5_UNKNOWN:
      digitalWrite(D_VLV5_NORM,LOW);
      digitalWrite(D_VLV5_REV,LOW);
      break;
    case VLV6_OPEN:
      digitalWrite(D_VLV6_NORM,HIGH);
      digitalWrite(D_VLV6_REV,LOW);
      break;
    case VLV6_CLOSE:
      digitalWrite(D_VLV6_NORM,LOW);
      digitalWrite(D_VLV6_REV,HIGH);
      break;
    case VLV6_UNKNOWN:
      digitalWrite(D_VLV6_NORM,LOW);
      digitalWrite(D_VLV6_REV,LOW);
      break;
    case VLV7_OPEN:
      digitalWrite(D_VLV7_NORM,HIGH);
      digitalWrite(D_VLV7_REV,LOW);
      break;
    case VLV7_CLOSE:
      digitalWrite(D_VLV7_NORM,LOW);
      digitalWrite(D_VLV7_REV,HIGH);
      break;
    case VLV7_UNKNOWN:
      digitalWrite(D_VLV7_NORM,LOW);
      digitalWrite(D_VLV7_REV,LOW);
      break;
    }
    output_test_toggle = 0;
  }
  lcd.setCursor(0,1);
  lcd.print(lcdtext);
}

void test_output_all_reset(void) {
  digitalWrite(D_VLV1_NORM,LOW);
  digitalWrite(D_VLV1_REV,LOW);
  digitalWrite(D_VLV2_NORM,LOW);
  digitalWrite(D_VLV2_REV,LOW);
  digitalWrite(D_VLV3_NORM,LOW);
  digitalWrite(D_VLV3_REV,LOW);
  digitalWrite(D_VLV4_NORM,LOW);
  digitalWrite(D_VLV4_REV,LOW);
  digitalWrite(D_VLV5_NORM,LOW);
  digitalWrite(D_VLV5_REV,LOW);
  digitalWrite(D_VLV6_NORM,LOW);
  digitalWrite(D_VLV6_REV,LOW);
  digitalWrite(D_VLV7_NORM,LOW);
  digitalWrite(D_VLV7_REV,LOW);
}
