/////////////////////////////////////////
//UARDECS Sample Program "DummyThermocouple"
//UECS ccm "TCTemp" sending test Ver1.0
//By Masafumi Horimoto 2019/09/12
//////////////////////////////////////////
// -*- mode : C++ -*-
//[概要]
// ABCO2の基幹的プログラム


#include <SPI.h>
#include <Ethernet2.h>
#include <avr/pgmspace.h>
#include <EEPROM.h>
#include <Uardecs_mega.h>
#include "abco2.h"

/////////////////////////////////////
//IP reset jupmer pin setting
//IPアドレスリセット用ジャンパーピン設定
/////////////////////////////////////

const byte U_DhcpPin = 2;
const byte U_DhcpPin_Sense=HIGH;
const byte U_InitPin = 3;
const byte U_InitPin_Sense=LOW;

////////////////////////////////////
//Node basic infomation
///////////////////////////////////
const char U_name[] PROGMEM= "ABCO2A04";//MAX 20 chars
const char U_vender[] PROGMEM= "HOLLY";//MAX 20 chars
const char U_uecsid[] PROGMEM= "10100C009999";//12 chars fixed
const char U_footnote[] PROGMEM= "";
//const int U_footnoteLetterNumber = 48;//Abolished after Ver 0.6
char U_nodename[20] = "ABCO2A04";//MAX 19chars
UECSOriginalAttribute U_orgAttribute;

//////////////////////////////////
// html page1 setting
//////////////////////////////////
#define DECIMAL_DIGIT	1 //小数桁数

const int U_HtmlLine = 34; //Total number of HTML table rows.

// 各種センサの値
// センサの値は設定しないので UECSSHOWDATAで表示する。
// 熱電対温度センサの値
const char T1TEMP0[] PROGMEM= "T1温度";
const char T1TEMP1[] PROGMEM= "T2温度";
const char T1TEMP2[] PROGMEM= "T3温度";
const char T1TEMP3[] PROGMEM= "T4温度";
const char T1TEMP4[] PROGMEM= "T5温度";
const char T1TEMP5[] PROGMEM= "T6温度";
const char T1TEMP6[] PROGMEM= "T7温度";
const char T1TEMP7[] PROGMEM= "T8温度";
const char T1NOTE0[] PROGMEM= "";
const char T1NOTE1[] PROGMEM= "";
const char T1NOTE2[] PROGMEM= "";
const char T1NOTE3[] PROGMEM= "";
const char T1NOTE4[] PROGMEM= "";
const char T1NOTE5[] PROGMEM= "";
const char T1NOTE6[] PROGMEM= "";
const char T1NOTE7[] PROGMEM= "";
const char TempUNIT[] PROGMEM= "℃";

//熱電対温度センサの値
//小数桁数が1の場合、123が12.3と表示される
// 配列は0から始まることに注意。
#define T1T_DECIMAL_DIGIT 1 // 小数桁数
signed long t1tValue[8];


// 液面レベル
// H/Lの二者択一。UECSSHOWSTRING で表示する。
const char WATER_LVL[] PROGMEM = "液面レベル";
const char WLVL_HIGH[] PROGMEM = "HIGH";
const char WLVL_LOW[] PROGMEM  = "LOW";
const char *StrWATER_LVL[2] = {
  WLVL_HIGH,
  WLVL_LOW
};
boolean waterLevel;          // I/Oから信号を受電するため
signed long ShowWaterLevel;  // 表示のインデックスのため

// 圧力センサ
// H/Lの二者択一。UECSSHOWSTRING で表示する。
const char PRESS_LVL[] PROGMEM  = "圧力";
const char PRESS_HIGH[] PROGMEM = "HIGH";
const char PRESS_LOW[] PROGMEM  = "LOW";
const char *StrPRESS_LVL[2] = {
  PRESS_HIGH,
  PRESS_LOW
};
boolean pressLevel;          // I/Oから信号を受電するため
signed long ShowPressLevel;  // 表示のインデックスのため


// CO2センサ
// センサの値は設定しないので UECSSHOWDATAで表示する。
// K33LP
#define CO2_DIGIT 0 // 小数桁数
const char CO2NAME[] PROGMEM = "CO2濃度";
const char CO2NOTE1[] PROGMEM = "K33LP";
const char CO2NOTE2[] PROGMEM = "K33ICB";
const char CO2NOTE3[] PROGMEM = "合わせて";
const char CO2UNIT[] PROGMEM= "ppm";
signed long co2detail;
signed long co2bigger;
signed long co2val;

#define CO2TEMP_DIGIT  1
#define CO2HUMI_DIGIT  1
const char CO2TEMP_NAME[] PROGMEM = "CO2温度";
const char CO2HUMI_NAME[] PROGMEM = "CO2湿度";
const char CO2HUMI_UNIT[] PROGMEM = "%RH";
signed long co2temp;
signed long co2humi;


//バルブの状態表示
//UECSSHOWSTRING
const char VLVSTSNAME0[] PROGMEM= "V1の状態";
const char VLVSTSNAME1[] PROGMEM= "V2の状態";
const char VLVSTSNAME2[] PROGMEM= "V3の状態";
const char VLVSTSNAME3[] PROGMEM= "V4の状態";
const char VLVSTSNAME4[] PROGMEM= "V5の状態";
const char VLVSTSNAME5[] PROGMEM= "V6の状態";
const char VLVSTSNAME6[] PROGMEM= "V7の状態";
const char VLVSTSNAME7[] PROGMEM= "V8の状態";
const char VLVSTSNOTE0[] PROGMEM= "";
const char VLVSTSNOTE1[] PROGMEM= "";
const char VLVSTSNOTE2[] PROGMEM= "";
const char VLVSTSNOTE3[] PROGMEM= "";
const char VLVSTSNOTE4[] PROGMEM= "";
const char VLVSTSNOTE5[] PROGMEM= "";
const char VLVSTSNOTE6[] PROGMEM= "";
const char VLVSTSNOTE7[] PROGMEM= "";
const char VLVSTS_CLOSE[] PROGMEM= "CLOSE";
const char VLVSTS_OPEN[] PROGMEM= "OPEN";
const char VLVSTS_UNKN[] PROGMEM= "UNKNOWN";
//
// 0: CLOSE
// 1: OPEN
// 2: UNKNOWN
//
const char *StrVLVSTS[3]={
  VLVSTS_CLOSE,
  VLVSTS_OPEN,
  VLVSTS_UNKN
};
signed long VLVStatus[8];
//signed long showValueStatus2;


//バルブの設定内容(強制OPEN/CLOSE,自動)
//UECSSELECTDATA
const char VLVNAME0[] PROGMEM= "バルブ1の設定";
const char VLVNAME1[] PROGMEM= "バルブ2の設定";
const char VLVNAME2[] PROGMEM= "バルブ3の設定";
const char VLVNAME3[] PROGMEM= "バルブ4の設定";
const char VLVNAME4[] PROGMEM= "バルブ5の設定";
const char VLVNAME5[] PROGMEM= "バルブ6の設定";
const char VLVNAME6[] PROGMEM= "バルブ7の設定";
const char VLVNAME7[] PROGMEM= "バルブ8の設定";
const char VLVNOTE0[] PROGMEM= "";
const char VLVNOTE1[] PROGMEM= "";
const char VLVNOTE2[] PROGMEM= "";
const char VLVNOTE3[] PROGMEM= "";
const char VLVNOTE4[] PROGMEM= "";
const char VLVNOTE5[] PROGMEM= "";
const char VLVNOTE6[] PROGMEM= "";
const char VLVNOTE7[] PROGMEM= "";
const char VLV_CLOSE[] PROGMEM= "手動CLOSE";
const char VLV_OPEN[] PROGMEM= "手動OPEN";
const char VLV_AUTO[] PROGMEM= "自動";
const char *StrVLV_SELECT[3]={
  VLV_AUTO,
  VLV_CLOSE,
  VLV_OPEN,
};

//入力された選択肢の位置を受け取る変数
// VLV_AUTOが0、VLV_CLOSEで1、VLV_OPENで2になる
signed long set_VLV_SELECT[8];

//●バルブ動作条件の入力
//UECSINPUTDATA
const char VCTNAME0[] PROGMEM= "V1作動温度";
const char VCTNAME1[] PROGMEM= "V2作動温度";
const char VCTNAME2[] PROGMEM= "V3作動温度";
const char VCTNAME3[] PROGMEM= "V4作動温度";
const char VCTNAME4[] PROGMEM= "V5作動温度";
const char VCTNAME5[] PROGMEM= "V6作動温度";
const char VCTNAME6[] PROGMEM= "V7作動温度";
const char VCTNAME7[] PROGMEM= "V8作動温度";
const char VCTNOTE0[] PROGMEM= "";
const char VCTNOTE1[] PROGMEM= "";
const char VCTNOTE2[] PROGMEM= "";
const char VCTNOTE3[] PROGMEM= "";
const char VCTNOTE4[] PROGMEM= "";
const char VCTNOTE5[] PROGMEM= "";
const char VCTNOTE6[] PROGMEM= "";
const char VCTNOTE7[] PROGMEM= "";

//バルブ動作温度を収める変数
//小数桁数が1の場合、例えばWeb上で12.3が入力されると123が代入される
signed long setVCTval[8];
//signed long setONTempFromWeb2;

// モーターポンプ、ブロア関係
//
const char MOTONAME0[] PROGMEM= "ブロアー";
const char MOTONAME1[] PROGMEM= "ポンプ";
const char MOTO_RUN[] PROGMEM  ="RUN";
const char MOTO_STOP[] PROGMEM = "STOP";
const char MOTO_AUTO[] PROGMEM = "AUTO";
const char *StrMOTOSW[3] = {
  MOTO_AUTO,
  MOTO_RUN,
  MOTO_STOP
};

signed long setMOTO_ON_OFF_AUTO[2];
signed long statusMOTO_ON_OFF_AUTO[2];



//●ダミー素材の定義
//dummy value
const char NONES[] PROGMEM= "";
const char** DUMMY = NULL;

//表示素材の登録
struct UECSUserHtml U_html[U_HtmlLine]={
  //{名前,入出力形式	,単位 ,詳細説明,選択肢文字列	,選択肢数,値	,最小値,最大値,小数桁数}
  {MOTONAME0, UECSSHOWSTRING, NONES, VLVSTSNOTE0,StrMOTOSW,3,&(statusMOTO_ON_OFF_AUTO[0]),0,0,0},
  {MOTONAME1, UECSSHOWSTRING, NONES, VLVSTSNOTE1,StrMOTOSW,3,&(statusMOTO_ON_OFF_AUTO[1]),0,0,0},
  {T1TEMP0, UECSSHOWDATA, TempUNIT, T1NOTE0, DUMMY, 0,&(t1tValue[0])	, 0, 0, T1T_DECIMAL_DIGIT},
  {T1TEMP1, UECSSHOWDATA, TempUNIT, T1NOTE1, DUMMY, 0,&(t1tValue[1])	, 0, 0, T1T_DECIMAL_DIGIT},
  {T1TEMP2, UECSSHOWDATA, TempUNIT, T1NOTE2, DUMMY, 0,&(t1tValue[2])	, 0, 0, T1T_DECIMAL_DIGIT},
  {T1TEMP3, UECSSHOWDATA, TempUNIT, T1NOTE3, DUMMY, 0,&(t1tValue[3])	, 0, 0, T1T_DECIMAL_DIGIT},
  {T1TEMP4, UECSSHOWDATA, TempUNIT, T1NOTE4, DUMMY, 0,&(t1tValue[4])	, 0, 0, T1T_DECIMAL_DIGIT},
  {T1TEMP5, UECSSHOWDATA, TempUNIT, T1NOTE5, DUMMY, 0,&(t1tValue[5])	, 0, 0, T1T_DECIMAL_DIGIT},
  {T1TEMP6, UECSSHOWDATA, TempUNIT, T1NOTE6, DUMMY, 0,&(t1tValue[6])	, 0, 0, T1T_DECIMAL_DIGIT},
  {T1TEMP7, UECSSHOWDATA, TempUNIT, T1NOTE7, DUMMY, 0,&(t1tValue[7])	, 0, 0, T1T_DECIMAL_DIGIT},
  {VLVSTSNAME0, UECSSHOWSTRING, NONES, VLVSTSNOTE0,StrVLVSTS,3,&(VLVStatus[0]),0,0,0},
  {VLVSTSNAME1, UECSSHOWSTRING, NONES, VLVSTSNOTE1,StrVLVSTS,3,&(VLVStatus[1]),0,0,0},
  {VLVSTSNAME2, UECSSHOWSTRING, NONES, VLVSTSNOTE2,StrVLVSTS,3,&(VLVStatus[2]),0,0,0},
  {VLVSTSNAME3, UECSSHOWSTRING, NONES, VLVSTSNOTE3,StrVLVSTS,3,&(VLVStatus[3]),0,0,0},
  {VLVSTSNAME4, UECSSHOWSTRING, NONES, VLVSTSNOTE4,StrVLVSTS,3,&(VLVStatus[4]),0,0,0},
  {VLVSTSNAME5, UECSSHOWSTRING, NONES, VLVSTSNOTE5,StrVLVSTS,3,&(VLVStatus[5]),0,0,0},
  {VLVSTSNAME6, UECSSHOWSTRING, NONES, VLVSTSNOTE6,StrVLVSTS,3,&(VLVStatus[6]),0,0,0},
  {VLVSTSNAME7, UECSSHOWSTRING, NONES, VLVSTSNOTE7,StrVLVSTS,3,&(VLVStatus[7]),0,0,0},
  
  {VLVNAME0,UECSSELECTDATA,NONES,VLVNOTE0, StrVLV_SELECT,3, &(set_VLV_SELECT[0]), 0, 0, 0},
  {VCTNAME0,UECSINPUTDATA, TempUNIT,VCTNOTE0,DUMMY, 0,&(setVCTval[0]), 100, 1000, T1T_DECIMAL_DIGIT},
  {VLVNAME1,UECSSELECTDATA,NONES,VLVNOTE1, StrVLV_SELECT,3, &(set_VLV_SELECT[1]), 0, 0, 0},
  {VCTNAME1,UECSINPUTDATA, TempUNIT,VCTNOTE1,DUMMY, 0,&(setVCTval[1]), 100, 1000, T1T_DECIMAL_DIGIT},
  {VLVNAME2,UECSSELECTDATA,NONES,VLVNOTE2, StrVLV_SELECT,3, &(set_VLV_SELECT[2]), 0, 0, 0},
  {VCTNAME2,UECSINPUTDATA, TempUNIT,VCTNOTE2,DUMMY, 0,&(setVCTval[2]), 100, 1000, T1T_DECIMAL_DIGIT},
  {VLVNAME3,UECSSELECTDATA,NONES,VLVNOTE3, StrVLV_SELECT,3, &(set_VLV_SELECT[3]), 0, 0, 0},
  {VCTNAME3,UECSINPUTDATA, TempUNIT,VCTNOTE3,DUMMY, 0,&(setVCTval[3]), 100, 1000, T1T_DECIMAL_DIGIT},
  {VLVNAME4,UECSSELECTDATA,NONES,VLVNOTE4, StrVLV_SELECT,3, &(set_VLV_SELECT[4]), 0, 0, 0},
  {VCTNAME4,UECSINPUTDATA, TempUNIT,VCTNOTE4,DUMMY, 0,&(setVCTval[4]), 100, 1000, T1T_DECIMAL_DIGIT},
  {VLVNAME5,UECSSELECTDATA,NONES,VLVNOTE5, StrVLV_SELECT,3, &(set_VLV_SELECT[5]), 0, 0, 0},
  {VCTNAME5,UECSINPUTDATA, TempUNIT,VCTNOTE5,DUMMY, 0,&(setVCTval[5]), 100, 1000, T1T_DECIMAL_DIGIT},
  {VLVNAME6,UECSSELECTDATA,NONES,VLVNOTE6, StrVLV_SELECT,3, &(set_VLV_SELECT[6]), 0, 0, 0},
  {VCTNAME6,UECSINPUTDATA, TempUNIT,VCTNOTE6,DUMMY, 0,&(setVCTval[6]), 100, 1000, T1T_DECIMAL_DIGIT},
  {VLVNAME7,UECSSELECTDATA,NONES,VLVNOTE7, StrVLV_SELECT,3, &(set_VLV_SELECT[7]), 0, 0, 0},
  {VCTNAME7,UECSINPUTDATA, TempUNIT,VCTNOTE7,DUMMY, 0,&(setVCTval[7]), 100, 1000, T1T_DECIMAL_DIGIT},
  //  {VLVNAME1, UECSSELECTDATA,NONES, &(showValueStatus1), stringSELECT,3,
  //  {VLVNAME1,UECSSELECTDATA,&(setONTempFromWeb1),&(showValueStatus1),
  //  {NAME0,UECSSHOWDATA	,UNIT0,NOTE0, DUMMY	, 0,  &(showValueTemp)	, 0, 0, DECIMAL_DIGIT},
  //  {NAME1,UECSSELECTDATA	,NONES,NOTE1, stringSELECT,3, &(setONOFFAUTO)	, 0, 0, 0},
  //  {NAME2,UECSINPUTDATA	,UNIT2,NOTE2, DUMMY	, 0,  &(setONTempFromWeb), 100, 1000, DECIMAL_DIGIT},
  //  {NAME3,UECSSHOWSTRING	,NONES,NOTE3, stringSHOW, 2,  &(showValueStatus), 0, 0, 0},
};


//////////////////////////////////
// UserCCM setting
//////////////////////////////////
//define CCMID for identify
//CCMID_dummy must put on last
enum {
  CCMID_TCTemp1,CCMID_OPETemp1,
  CCMID_TCTemp2,CCMID_OPETemp2,
  CCMID_TCTemp3,CCMID_OPETemp3,
  CCMID_TCTemp4,CCMID_OPETemp4,
  CCMID_TCTemp5,CCMID_OPETemp5,
  CCMID_TCTemp6,CCMID_OPETemp6,
  CCMID_TCTemp7,CCMID_OPETemp7,
  CCMID_TCTemp8,CCMID_OPETemp8,
  CCMID_cnd,
  CCMID_dummy,
};


const int U_MAX_CCM = CCMID_dummy;
UECSCCM U_ccmList[U_MAX_CCM];


const char ccmUnitTemp[] PROGMEM= "C";
const char ccmNameTemp1[] PROGMEM= "T1温度";
const char ccmTypeTemp1[] PROGMEM= "TCTemp.1";
const char ccmNameTemp2[] PROGMEM= "T2温度";
const char ccmTypeTemp2[] PROGMEM= "TCTemp.2";
const char ccmNameTemp3[] PROGMEM= "T3温度";
const char ccmTypeTemp3[] PROGMEM= "TCTemp.3";
const char ccmNameTemp4[] PROGMEM= "T4温度";
const char ccmTypeTemp4[] PROGMEM= "TCTemp.4";
const char ccmNameTemp5[] PROGMEM= "T5温度";
const char ccmTypeTemp5[] PROGMEM= "TCTemp.5";
const char ccmNameTemp6[] PROGMEM= "T6温度";
const char ccmTypeTemp6[] PROGMEM= "TCTemp.6";
const char ccmNameTemp7[] PROGMEM= "T7温度";
const char ccmTypeTemp7[] PROGMEM= "TCTemp.7";
const char ccmNameTemp8[] PROGMEM= "T8温度";
const char ccmTypeTemp8[] PROGMEM= "TCTemp.8";

const char ccmUnitOpeTemp[] PROGMEM= "C";
const char ccmNameOpeTemp1[] PROGMEM= "T1動作温度";
const char ccmTypeOpeTemp1[] PROGMEM= "OPETemp.1";
const char ccmNameOpeTemp2[] PROGMEM= "T2動作温度";
const char ccmTypeOpeTemp2[] PROGMEM= "OPETemp.2";
const char ccmNameOpeTemp3[] PROGMEM= "T3動作温度";
const char ccmTypeOpeTemp3[] PROGMEM= "OPETemp.3";
const char ccmNameOpeTemp4[] PROGMEM= "T4動作温度";
const char ccmTypeOpeTemp4[] PROGMEM= "OPETemp.4";
const char ccmNameOpeTemp5[] PROGMEM= "T5動作温度";
const char ccmTypeOpeTemp5[] PROGMEM= "OPETemp.5";
const char ccmNameOpeTemp6[] PROGMEM= "T6動作温度";
const char ccmTypeOpeTemp6[] PROGMEM= "OPETemp.6";
const char ccmNameOpeTemp7[] PROGMEM= "T7動作温度";
const char ccmTypeOpeTemp7[] PROGMEM= "OPETemp.7";
const char ccmNameOpeTemp8[] PROGMEM= "T8動作温度";
const char ccmTypeOpeTemp8[] PROGMEM= "OPETemp8.";

const char ccmNameCnd[] PROGMEM= "NodeCondition";
const char ccmTypeCnd[] PROGMEM= "cnd.mIC";
const char ccmUnitCnd[] PROGMEM= "";


void UserInit(){
  //MAC address is printed on sticker of Ethernet Shield.
  //You must assign unique MAC address to each nodes.
  //MACアドレス設定、必ずEthernet Shieldに書かれた値を入力して下さい。
  //全てのノードに異なるMACアドレスを設定する必要があります。
  U_orgAttribute.mac[0] = 0x02;
  U_orgAttribute.mac[1] = 0xa2;
  U_orgAttribute.mac[2] = 0x73;
  U_orgAttribute.mac[3] = 0x8f;
  U_orgAttribute.mac[4] = 0x00;
  U_orgAttribute.mac[5] = 0x01;
  
  //Set ccm list
  UECSsetCCM(true, CCMID_TCTemp1, ccmNameTemp1, ccmTypeTemp1, ccmUnitTemp, 29, 1, A_10S_0);
  UECSsetCCM(true, CCMID_OPETemp1,ccmNameOpeTemp1, ccmTypeOpeTemp1, ccmUnitOpeTemp, 29, 1, A_10S_0);
  UECSsetCCM(true, CCMID_TCTemp2, ccmNameTemp2, ccmTypeTemp2, ccmUnitTemp, 29, 1, A_10S_0);
  UECSsetCCM(true, CCMID_OPETemp2,ccmNameOpeTemp2, ccmTypeOpeTemp2, ccmUnitOpeTemp, 29, 1, A_10S_0);
  UECSsetCCM(true, CCMID_TCTemp3, ccmNameTemp3, ccmTypeTemp3, ccmUnitTemp, 29, 1, A_10S_0);
  UECSsetCCM(true, CCMID_OPETemp3,ccmNameOpeTemp3, ccmTypeOpeTemp3, ccmUnitOpeTemp, 29, 1, A_10S_0);
  UECSsetCCM(true, CCMID_TCTemp4, ccmNameTemp4, ccmTypeTemp4, ccmUnitTemp, 29, 1, A_10S_0);
  UECSsetCCM(true, CCMID_OPETemp4,ccmNameOpeTemp4, ccmTypeOpeTemp4, ccmUnitOpeTemp, 29, 1, A_10S_0);
  UECSsetCCM(true, CCMID_TCTemp5, ccmNameTemp5, ccmTypeTemp5, ccmUnitTemp, 29, 1, A_10S_0);
  UECSsetCCM(true, CCMID_OPETemp5,ccmNameOpeTemp5, ccmTypeOpeTemp5, ccmUnitOpeTemp, 29, 1, A_10S_0);
  UECSsetCCM(true, CCMID_TCTemp6, ccmNameTemp6, ccmTypeTemp6, ccmUnitTemp, 29, 1, A_10S_0);
  UECSsetCCM(true, CCMID_OPETemp6,ccmNameOpeTemp6, ccmTypeOpeTemp6, ccmUnitOpeTemp, 29, 1, A_10S_0);
  UECSsetCCM(true, CCMID_TCTemp7, ccmNameTemp7, ccmTypeTemp7, ccmUnitTemp, 29, 1, A_10S_0);
  UECSsetCCM(true, CCMID_OPETemp7,ccmNameOpeTemp7, ccmTypeOpeTemp7, ccmUnitOpeTemp, 29, 1, A_10S_0);
  UECSsetCCM(true, CCMID_TCTemp8, ccmNameTemp8, ccmTypeTemp8, ccmUnitTemp, 29, 1, A_10S_0);
  UECSsetCCM(true, CCMID_OPETemp8,ccmNameOpeTemp8, ccmTypeOpeTemp8, ccmUnitOpeTemp, 29, 1, A_10S_0);
  UECSsetCCM(true, CCMID_cnd   , ccmNameCnd , ccmTypeCnd , ccmUnitCnd , 29, 0, A_1S_0);
}


void OnWebFormRecieved() {
  U_ccmList[CCMID_OPETemp1].value=setVCTval[0];
  U_ccmList[CCMID_OPETemp2].value=setVCTval[1];
  U_ccmList[CCMID_OPETemp3].value=setVCTval[2];
  U_ccmList[CCMID_OPETemp4].value=setVCTval[3];
  U_ccmList[CCMID_OPETemp5].value=setVCTval[4];
  U_ccmList[CCMID_OPETemp6].value=setVCTval[5];
  U_ccmList[CCMID_OPETemp7].value=setVCTval[6];
  U_ccmList[CCMID_OPETemp8].value=setVCTval[7];
  ChangeThermostat();
 
}

void UserEverySecond() {
}
void UserEveryMinute() {
}
void UserEveryLoop() {
}

void loop(){
  int a0v;
  UECSloop();
  a0v = analogRead(A0);
  U_ccmList[CCMID_TCTemp1].value= a0v * 0.98;
  ChangeThermostat();
}

void setup(){
  extern byte megaEtherSS;
  int i;
  megaEtherSS = 53; // SS is pin 53
  UECSsetup();
  pinMode(A0,INPUT);
  pinMode(BLOWER,OUTPUT);
  digitalWrite(BLOWER,HIGH);
  U_ccmList[CCMID_OPETemp1].value=setVCTval[0];
  U_ccmList[CCMID_TCTemp1].validity = true;
  for (i=0;i<8;i++) {
    VLVStatus[i] = 1;
  }
  for (i=0;i<2;i++) {
    statusMOTO_ON_OFF_AUTO[i] = 0;
  }
}

//---------------------------------------------------------
//サーモスタット動作を変化させる関数
//---------------------------------------------------------
void ChangeThermostat(){
  t1tValue[0] = U_ccmList[CCMID_TCTemp1].value;
  t1tValue[1] = U_ccmList[CCMID_TCTemp2].value;
  t1tValue[2] = U_ccmList[CCMID_TCTemp3].value;
  t1tValue[3] = U_ccmList[CCMID_TCTemp4].value;
  t1tValue[4] = U_ccmList[CCMID_TCTemp5].value;
  t1tValue[5] = U_ccmList[CCMID_TCTemp6].value;
  t1tValue[6] = U_ccmList[CCMID_TCTemp7].value;
  t1tValue[7] = U_ccmList[CCMID_TCTemp8].value;

  switch(set_VLV_SELECT[0]) {
  case 0:
    U_ccmList[CCMID_cnd].value=0;  //Manual OFF
    break;
  case 1:
    U_ccmList[CCMID_cnd].value=1;  //Manual ON
    break;
  case 2:
    if (U_ccmList[CCMID_TCTemp1].validity) {
      if (U_ccmList[CCMID_TCTemp1].value<U_ccmList[CCMID_OPETemp1].value) {
	U_ccmList[CCMID_cnd].value=1;  //Auto ON
	break;
      }
    }
    U_ccmList[CCMID_cnd].value=0;  //Auto OFF
    break;
  }
  switch(set_VLV_SELECT[1]) {
  case 0:
    U_ccmList[CCMID_cnd].value=0;  //Manual OFF
    break;
  case 1:
    U_ccmList[CCMID_cnd].value=1;  //Manual ON
    break;
  case 2:
    if (U_ccmList[CCMID_TCTemp2].validity) {
      if (U_ccmList[CCMID_TCTemp2].value<U_ccmList[CCMID_OPETemp2].value) {
	U_ccmList[CCMID_cnd].value=1;  //Auto ON
	break;
      }
    }
    U_ccmList[CCMID_cnd].value=0;  //Auto OFF
    break;
  }
  switch(set_VLV_SELECT[2]) {
  case 0:
    U_ccmList[CCMID_cnd].value=0;  //Manual OFF
    break;
  case 1:
    U_ccmList[CCMID_cnd].value=1;  //Manual ON
    break;
  case 2:
    if (U_ccmList[CCMID_TCTemp3].validity) {
      if (U_ccmList[CCMID_TCTemp3].value<U_ccmList[CCMID_OPETemp3].value) {
	U_ccmList[CCMID_cnd].value=1;  //Auto ON
	break;
      }
    }
    U_ccmList[CCMID_cnd].value=0;  //Auto OFF
    break;
  }
  switch(set_VLV_SELECT[3]) {
  case 0:
    U_ccmList[CCMID_cnd].value=0;  //Manual OFF
    break;
  case 1:
    U_ccmList[CCMID_cnd].value=1;  //Manual ON
    break;
  case 2:
    if (U_ccmList[CCMID_TCTemp4].validity) {
      if (U_ccmList[CCMID_TCTemp4].value<U_ccmList[CCMID_OPETemp4].value) {
	U_ccmList[CCMID_cnd].value=1;  //Auto ON
	break;
      }
    }
    U_ccmList[CCMID_cnd].value=0;  //Auto OFF
    break;
  }
  switch(set_VLV_SELECT[4]) {
  case 0:
    U_ccmList[CCMID_cnd].value=0;  //Manual OFF
    break;
  case 1:
    U_ccmList[CCMID_cnd].value=1;  //Manual ON
    break;
  case 2:
    if (U_ccmList[CCMID_TCTemp5].validity) {
      if (U_ccmList[CCMID_TCTemp5].value<U_ccmList[CCMID_OPETemp5].value) {
	U_ccmList[CCMID_cnd].value=1;  //Auto ON
	break;
      }
    }
    U_ccmList[CCMID_cnd].value=0;  //Auto OFF
    break;
  }
  switch(set_VLV_SELECT[5]) {
  case 0:
    U_ccmList[CCMID_cnd].value=0;  //Manual OFF
    break;
  case 1:
    U_ccmList[CCMID_cnd].value=1;  //Manual ON
    break;
  case 2:
    if (U_ccmList[CCMID_TCTemp6].validity) {
      if (U_ccmList[CCMID_TCTemp6].value<U_ccmList[CCMID_OPETemp6].value) {
	U_ccmList[CCMID_cnd].value=1;  //Auto ON
	break;
      }
    }
    U_ccmList[CCMID_cnd].value=0;  //Auto OFF
    break;
  }
  switch(set_VLV_SELECT[6]) {
  case 0:
    U_ccmList[CCMID_cnd].value=0;  //Manual OFF
    break;
  case 1:
    U_ccmList[CCMID_cnd].value=1;  //Manual ON
    break;
  case 2:
    if (U_ccmList[CCMID_TCTemp7].validity) {
      if (U_ccmList[CCMID_TCTemp7].value<U_ccmList[CCMID_OPETemp7].value) {
	U_ccmList[CCMID_cnd].value=1;  //Auto ON
	break;
      }
    }
    U_ccmList[CCMID_cnd].value=0;  //Auto OFF
    break;
  }
  switch(set_VLV_SELECT[7]) {
  case 0:
    U_ccmList[CCMID_cnd].value=0;  //Manual OFF
    break;
  case 1:
    U_ccmList[CCMID_cnd].value=1;  //Manual ON
    break;
  case 2:
    if (U_ccmList[CCMID_TCTemp8].validity) {
      if (U_ccmList[CCMID_TCTemp8].value<U_ccmList[CCMID_OPETemp8].value) {
	U_ccmList[CCMID_cnd].value=1;  //Auto ON
	break;
      }
    }
    U_ccmList[CCMID_cnd].value=0;  //Auto OFF
    break;
  }
  //サーモスタット動作 
  // if(setONOFFAUTO==0) {
  //   U_ccmList[CCMID_cnd].value=0;  //Manual OFF
  // } else if(setONOFFAUTO==1) {
  //   U_ccmList[CCMID_cnd].value=1;  //Manual ON
  // } else if(setONOFFAUTO==2 && U_ccmList[CCMID_TCTemp].validity
  // 	    && U_ccmList[CCMID_TCTemp].value<U_ccmList[CCMID_OPETemp].value) {
  //   U_ccmList[CCMID_cnd].value=1;  //Auto ON
  // } else {
  //   U_ccmList[CCMID_cnd].value=0;  //OFF
  // }
  VLVStatus[0] = U_ccmList[CCMID_cnd].value;
  if (VLVStatus[0]==1) {
    digitalWrite(BLOWER,LOW);
  } else {
    digitalWrite(BLOWER,HIGH);
  }
}
