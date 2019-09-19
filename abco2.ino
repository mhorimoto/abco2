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

const byte U_InitPin = 3;
const byte U_InitPin_Sense=LOW;

////////////////////////////////////
//Node basic infomation
///////////////////////////////////
const char U_name[] PROGMEM= "ABCO2A01";//MAX 20 chars
const char U_vender[] PROGMEM= "HOLLY";//MAX 20 chars
const char U_uecsid[] PROGMEM= "10100C009999";//12 chars fixed
const char U_footnote[] PROGMEM= "";
//const int U_footnoteLetterNumber = 48;//Abolished after Ver 0.6
char U_nodename[20] = "Sample";//MAX 19chars
UECSOriginalAttribute U_orgAttribute;

//////////////////////////////////
// html page1 setting
//////////////////////////////////
#define DECIMAL_DIGIT	1 //小数桁数

const int U_HtmlLine = 4; //Total number of HTML table rows.
const char TempUNIT[] PROGMEM= "℃";

//●熱電対温度センサの値
//UECSSHOWDATA
const char T1TEMP1[] PROGMEM= "T1温度";
const char T1TEMP2[] PROGMEM= "T2温度";
const char T1TEMP3[] PROGMEM= "T3温度";
const char T1TEMP4[] PROGMEM= "T4温度";
const char T1TEMP5[] PROGMEM= "T5温度";
const char T1TEMP6[] PROGMEM= "T6温度";
const char T1TEMP7[] PROGMEM= "T7温度";
const char T1TEMP8[] PROGMEM= "T8温度";
const char T1NOTE1[] PROGMEM= "";
const char T1NOTE2[] PROGMEM= "";
const char T1NOTE3[] PROGMEM= "";
const char T1NOTE4[] PROGMEM= "";
const char T1NOTE5[] PROGMEM= "";
const char T1NOTE6[] PROGMEM= "";
const char T1NOTE7[] PROGMEM= "";
const char T1NOTE8[] PROGMEM= "";

//表示用の値を格納する変数
//小数桁数が1の場合、123が12.3と表示される
signed long showValueTemp1;
signed long showValueTemp2;
signed long showValueTemp3;
signed long showValueTemp4;
signed long showValueTemp5;
signed long showValueTemp6;
signed long showValueTemp7;
signed long showValueTemp8;

//●表示素材の定義(2)選択肢表示
//UECSSELECTDATA
const char VLVNAME1[] PROGMEM= "バルブ1の設定";
const char VLVNAME2[] PROGMEM= "バルブ2の設定";
const char VLVNAME3[] PROGMEM= "バルブ3の設定";
const char VLVNAME4[] PROGMEM= "バルブ4の設定";
const char VLVNAME5[] PROGMEM= "バルブ5の設定";
const char VLVNAME6[] PROGMEM= "バルブ6の設定";
const char VLVNAME7[] PROGMEM= "バルブ7の設定";
const char VLVNAME8[] PROGMEM= "バルブ8の設定";
const char NOTE1[] PROGMEM= "V1の設定";
const char UECSOFF[] PROGMEM= "手動CLOSE";
const char UECSON[] PROGMEM= "手動OPEN";
const char UECSAUTO[] PROGMEM= "自動";
const char *stringSELECT[3]={
  UECSOFF,
  UECSON,
  UECSAUTO,
};

//入力された選択肢の位置を受け取る変数
//UECSOFFが0、UECSONで1、UECSAUTOで2になる
signed long setONOFFAUTO1;
signed long setONOFFAUTO2;
signed long setONOFFAUTO3;
signed long setONOFFAUTO4;
signed long setONOFFAUTO5;
signed long setONOFFAUTO6;
signed long setONOFFAUTO7;
signed long setONOFFAUTO8;

//●表示素材の定義(3)数値入力
//UECSINPUTDATA
const char NAME2[] PROGMEM= "V1作動温度の設定";
const char UNIT2[] PROGMEM= "℃";
const char NOTE2[] PROGMEM= "数値入力モード";

//入力された数値を受け取る変数
//小数桁数が1の場合、例えばWeb上で12.3が入力されると123が代入される
signed long setONTempFromWeb;

//●表示素材の定義(4)文字表示
//UECSSHOWSTRING
const char NAME3[] PROGMEM= "V1の状態";
const char NOTE3[] PROGMEM= "";
const char SHOWSTRING_OFF[] PROGMEM= "CLOSE";
const char SHOWSTRING_ON [] PROGMEM= "OPEN";
const char *stringSHOW[2]={
  SHOWSTRING_OFF,
  SHOWSTRING_ON,
};
signed long showValueStatus;

//●ダミー素材の定義
//dummy value
const char NONES[] PROGMEM= "";
const char** DUMMY = NULL;

//表示素材の登録
struct UECSUserHtml U_html[U_HtmlLine]={
  //{名前,入出力形式	,単位 ,詳細説明,選択肢文字列	,選択肢数,値	,最小値,最大値,小数桁数}
  {NAME0,UECSSHOWDATA	,UNIT0,NOTE0, DUMMY	, 0,  &(showValueTemp)	, 0, 0, DECIMAL_DIGIT},
  {NAME1,UECSSELECTDATA	,NONES,NOTE1, stringSELECT,3, &(setONOFFAUTO)	, 0, 0, 0},
  {NAME2,UECSINPUTDATA	,UNIT2,NOTE2, DUMMY	, 0,  &(setONTempFromWeb), 100, 1000, DECIMAL_DIGIT},
  {NAME3,UECSSHOWSTRING	,NONES,NOTE3, stringSHOW, 2,  &(showValueStatus), 0, 0, 0},
};



//////////////////////////////////
// UserCCM setting
//////////////////////////////////

//define CCMID for identify
//CCMID_dummy must put on last
enum {
  CCMID_TCTemp,
  CCMID_OPETemp,
  CCMID_cnd,
  CCMID_dummy,
};


const int U_MAX_CCM = CCMID_dummy;
UECSCCM U_ccmList[U_MAX_CCM];


const char ccmNameTemp[] PROGMEM= "T1温度";
const char ccmTypeTemp[] PROGMEM= "TCTemp.1";
const char ccmUnitTemp[] PROGMEM= "C";

const char ccmNameOpeTemp[] PROGMEM= "T1動作温度";
const char ccmTypeOpeTemp[] PROGMEM= "OPETemp.1";
const char ccmUnitOpeTemp[] PROGMEM= "C";

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
  UECSsetCCM(true, CCMID_TCTemp, ccmNameTemp, ccmTypeTemp, ccmUnitTemp, 29, 1, A_1S_0);
  UECSsetCCM(true, CCMID_OPETemp,ccmNameOpeTemp, ccmTypeOpeTemp, ccmUnitOpeTemp, 29, 1, A_10S_0);
  UECSsetCCM(true, CCMID_cnd   , ccmNameCnd , ccmTypeCnd , ccmUnitCnd , 29, 0, A_1S_0);
}


void OnWebFormRecieved() {
  U_ccmList[CCMID_OPETemp].value=setONTempFromWeb;
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
  U_ccmList[CCMID_TCTemp].value= a0v * 0.98;
  ChangeThermostat();
}

void setup(){
  UECSsetup();
  pinMode(A0,INPUT);
  pinMode(RLY1,OUTPUT);
  U_ccmList[CCMID_OPETemp].value=setONTempFromWeb;
  U_ccmList[CCMID_TCTemp].validity = true;
}

//---------------------------------------------------------
//サーモスタット動作を変化させる関数
//---------------------------------------------------------
void ChangeThermostat(){
  showValueTemp = U_ccmList[CCMID_TCTemp].value;

  switch(setONOFFAUTO) {
  case 0:
    U_ccmList[CCMID_cnd].value=0;  //Manual OFF
    break;
  case 1:
    U_ccmList[CCMID_cnd].value=1;  //Manual ON
    break;
  case 2:
    if (U_ccmList[CCMID_TCTemp].validity) {
      if (U_ccmList[CCMID_TCTemp].value<U_ccmList[CCMID_OPETemp].value) {
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
  showValueStatus = U_ccmList[CCMID_cnd].value;
  if (showValueStatus==1) {
    digitalWrite(RLY1,LOW);
  } else {
    digitalWrite(RLY1,HIGH);
  }
}

