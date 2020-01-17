#include "setup1.h"

void select_mode(void) {
  int i,aval,smenu;
  aval = analogRead(2);
  smenu = (aval/128);
  switch(smenu) {
  case 0: // MODE 0 全閉
    _r_mode(RMODE0,"0-ALL CLOSE     ");
    break;
  case 1: // MODE 1 CO2貯蔵
    _r_mode(RMODE1,"1-SAVE CO2      ");
    break;
  case 2: // MODE 2 放散(1)
    _r_mode(RMODE2,"2-RADIATION(1)  ");
    break;
  case 3: // MODE 3 放散(2)
    _r_mode(RMODE3,"3-RADIATION(2)  ");
    break;
  case 4: // MODE 4 外気導入
    _r_mode(RMODE4,"4-INTAKE OUTAIR ");
    break;
  case 5: // MODE 5 冷却
    _r_mode(RMODE5,"5-COOLING       ");
    break;
  case 6: // MODE 6 全開
    _r_mode(RMODE6,"6-ALL OPEN      ");
    break;
  case 7: // MODE 7 緊急停止
    _r_mode(RMODE7,"7-EMERGENCY STOP");
    break;
  }
}

void _r_mode(int p,char *name) {
  char lcdtext[17];
  extern int select_mode_toggle;
  extern int current_mode;
  lcd.setCursor(0,1);
  lcd.print(name);
  if (select_mode_toggle==1) {
    switch(p) {
    case RMODE0:
      vlv_close(1);
      vlv_close(2);
      vlv_close(3);
      vlv_close(4);
      vlv_close(5);
      vlv_close(6);
      vlv_close(7);
      stop_blower();
      stop_pump();
      break;
    case RMODE1:
      vlv_close(1);
      vlv_open(2);
      vlv_close(3);
      vlv_open(4);
      vlv_close(5);
      vlv_open(6);
      vlv_close(7);
      run_blower();
      run_pump();
      break;
    case RMODE2:
      vlv_close(1);
      vlv_close(2);
      vlv_open(3);
      vlv_close(4);
      vlv_open(5);
      vlv_close(6);
      vlv_open(7);
      run_blower();
      stop_pump();
      break;
    case RMODE3:
      vlv_open(1);
      vlv_close(2);
      vlv_close(3);
      vlv_close(4);
      vlv_open(5);
      vlv_close(6);
      vlv_open(7);
      run_blower();
      stop_pump();
      break;
    case RMODE4:
      vlv_open(1);
      vlv_close(2);
      vlv_close(3);
      vlv_close(4);
      vlv_close(5);
      vlv_open(6);
      vlv_open(7);
      run_blower();
      stop_pump();
      break;
    case RMODE5:
      vlv_open(1);
      vlv_close(2);
      vlv_close(3);
      vlv_open(4);
      vlv_open(5);
      vlv_close(6);
      vlv_close(7);
      run_blower();
      stop_pump();
      break;
    case RMODE6:
      vlv_open(1);
      vlv_open(2);
      vlv_open(3);
      vlv_open(4);
      vlv_open(5);
      vlv_open(6);
      vlv_open(7);
      stop_blower();
      stop_pump();
      break;
    case RMODE7:
      ems_stop();
      break;
    }
    lcd.setCursor(13,1);
    lcd.print("SET");
    current_mode = p;
    select_mode_toggle=0;
    delay(800);
  }
}

void vlv_open(int a) {
  switch(a) {
  case 1:
    digitalWrite(D_VLV1_NORM,HIGH);
    digitalWrite(D_VLV1_REV,LOW);
    break;
  case 2:
    digitalWrite(D_VLV2_NORM,HIGH);
    digitalWrite(D_VLV2_REV,LOW);
    break;
  case 3:
    digitalWrite(D_VLV3_NORM,HIGH);
    digitalWrite(D_VLV3_REV,LOW);
    break;
  case 4:
    digitalWrite(D_VLV4_NORM,HIGH);
    digitalWrite(D_VLV4_REV,LOW);
    break;
  case 5:
    digitalWrite(D_VLV5_NORM,HIGH);
    digitalWrite(D_VLV5_REV,LOW);
    break;
  case 6:
    digitalWrite(D_VLV6_NORM,HIGH);
    digitalWrite(D_VLV6_REV,LOW);
    break;
  case 7:
    digitalWrite(D_VLV7_NORM,HIGH);
    digitalWrite(D_VLV7_REV,LOW);
    break;
  }
}

void vlv_close(int a) {
  switch(a) {
  case 1:
    digitalWrite(D_VLV1_NORM,LOW);
    digitalWrite(D_VLV1_REV,HIGH);
    break;
  case 2:
    digitalWrite(D_VLV2_NORM,LOW);
    digitalWrite(D_VLV2_REV,HIGH);
    break;
  case 3:
    digitalWrite(D_VLV3_NORM,LOW);
    digitalWrite(D_VLV3_REV,HIGH);
    break;
  case 4:
    digitalWrite(D_VLV4_NORM,LOW);
    digitalWrite(D_VLV4_REV,HIGH);
    break;
  case 5:
    digitalWrite(D_VLV5_NORM,LOW);
    digitalWrite(D_VLV5_REV,HIGH);
    break;
  case 6:
    digitalWrite(D_VLV6_NORM,LOW);
    digitalWrite(D_VLV6_REV,HIGH);
    break;
  case 7:
    digitalWrite(D_VLV7_NORM,LOW);
    digitalWrite(D_VLV7_REV,HIGH);
    break;
  }
}

void run_blower(void) {
  digitalWrite(D_BLOWER,HIGH);
}

void stop_blower(void) {
  digitalWrite(D_BLOWER,LOW);
}

void run_pump(void) {
  digitalWrite(D_PUMP,HIGH);
}

void stop_pump(void) {
  digitalWrite(D_PUMP,LOW);
}

void ems_stop(void) {
  vlv_close(1);
  vlv_close(2);
  vlv_close(3);
  vlv_open(4);
  vlv_open(5);
  vlv_open(6);
  vlv_close(7);
  stop_blower();
  stop_pump();
  // バルブの動作が完了したらバルブへの電源を切る動作が必要
}
