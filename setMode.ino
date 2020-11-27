//-*- mode : C++ -*-
#include <Uardecs_mega.h>
//#include "abco2.h"

// Mode0 全閉
void setMode0(void) {
  modeRUN = 0;
  vlv_ctrl(VLV1_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV2_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV3_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV4_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV5_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV6_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV7_CLOSE,CCMID_cnd);
  stop_blower();
  stop_pump();
  U_ccmList[CCMID_cnd].value &= 0b00111111111111111111111111111111;  // RESET E-STOP BIT
}

// Mode1 CO2貯蔵
void setMode1(void) {
  modeRUN = 1;
  vlv_ctrl(VLV1_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV2_OPEN,CCMID_cnd);
  vlv_ctrl(VLV3_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV4_OPEN,CCMID_cnd);
  vlv_ctrl(VLV5_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV6_OPEN,CCMID_cnd);
  vlv_ctrl(VLV7_CLOSE,CCMID_cnd);
  run_blower();
  run_pump();
  U_ccmList[CCMID_cnd].value &= 0b00111111111111111111111111111111;  // RESET E-STOP BIT
}

// Mode2 放散(1): チャンバーair導入
void setMode2(void) {
  modeRUN = 2;
  vlv_ctrl(VLV1_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV2_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV3_OPEN,CCMID_cnd);
  vlv_ctrl(VLV4_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV5_OPEN,CCMID_cnd);
  vlv_ctrl(VLV6_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV7_OPEN,CCMID_cnd);
  run_blower();
  stop_pump();
  U_ccmList[CCMID_cnd].value &= 0b00111111111111111111111111111111;  // RESET E-STOP BIT
}

// Mode3 放散(2):外部air導入
void setMode3(void) {
  modeRUN = 3;
  vlv_ctrl(VLV1_OPEN,CCMID_cnd);
  vlv_ctrl(VLV2_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV3_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV4_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV5_OPEN,CCMID_cnd);
  vlv_ctrl(VLV6_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV7_OPEN,CCMID_cnd);
  run_blower();
  stop_pump();
  U_ccmList[CCMID_cnd].value &= 0b00111111111111111111111111111111;  // RESET E-STOP BIT
}

// Mode4 外気導入
void setMode4(void) {
  modeRUN = 4;
  vlv_ctrl(VLV1_OPEN,CCMID_cnd);
  vlv_ctrl(VLV2_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV3_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV4_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV5_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV6_OPEN,CCMID_cnd);
  vlv_ctrl(VLV7_OPEN,CCMID_cnd);
  run_blower();
  stop_pump();
  U_ccmList[CCMID_cnd].value &= 0b00111111111111111111111111111111;  // RESET E-STOP BIT
}

// Mode5 冷却
void setMode5(void) {
  modeRUN = 5;
  vlv_ctrl(VLV1_OPEN,CCMID_cnd);
  vlv_ctrl(VLV2_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV3_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV4_OPEN,CCMID_cnd);
  vlv_ctrl(VLV5_OPEN,CCMID_cnd);
  vlv_ctrl(VLV6_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV7_CLOSE,CCMID_cnd);
  run_blower();
  stop_pump();
  U_ccmList[CCMID_cnd].value &= 0b00111111111111111111111111111111;  // RESET E-STOP BIT
}

// Mode6 全開
void setMode6(void) {
  modeRUN = 6;
  vlv_ctrl(VLV1_OPEN,CCMID_cnd);
  vlv_ctrl(VLV2_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV3_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV4_OPEN,CCMID_cnd);
  vlv_ctrl(VLV5_OPEN,CCMID_cnd);
  vlv_ctrl(VLV6_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV7_CLOSE,CCMID_cnd);
  stop_blower();
  stop_pump();
  U_ccmList[CCMID_cnd].value &= 0b00111111111111111111111111111111;  // RESET E-STOP BIT
}

// Mode7 緊急停止
void setMode7(void) {
  modeRUN = 7;
  vlv_ctrl(VLV1_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV2_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV3_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV4_OPEN,CCMID_cnd);
  vlv_ctrl(VLV5_OPEN,CCMID_cnd);
  vlv_ctrl(VLV6_OPEN,CCMID_cnd);
  vlv_ctrl(VLV7_CLOSE,CCMID_cnd);
  U_ccmList[CCMID_cnd].value &= 0b01111111111111110000111000111000;  // MOTOR and VALVE
  U_ccmList[CCMID_cnd].value |= 0b01000000000000000000000000000000;  // EMERGENCY STOP
  stop_blower();
  stop_pump();
}

