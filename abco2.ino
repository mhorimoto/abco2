/////////////////////////////////////////
// -*- mode : C++ -*-
//[概要]
// ABCO2の基幹的プログラム
//   T-6が50℃以上になったらMODE-5,6を繰り返す予定。(D0057)
//   バルブシーケンスを20201117に合致させる。K33ICBの単位表記を改める。 (D0056B)
//   setMode()を別ファイルにした (D0056A)
//   モード切替時刻設定を任意に出来るように (D0056)
//   運転モード表記を改める[tech-abco2:0000132] (D0055D)
//   定義をabco2.hに移行した。(D0055C)
//   運転モードの自動切り替えを1分毎から1秒毎確認に変更(D0055)
//   BURNERのport間違いを修正 (D0054)
//   NTPで時刻合わせを行うように変更 (D0053)
//   起動直後は、Mode6(全開)から開始するように変更 (D0052)
//   E-STOP以外の時にE-STOP BITが消えるようにした。(D0050)
//   手動の時にモード切替が出来るようにした。モードがLCD表示される。(D0048)
//   運転種別が自動のときだけ (D0047)
//   昼間はMode2,夜間はMode0かMode1になるように。 (D0046)
//   mcp_ope.ino,k33_ope.inoのシリアル出力を抑制 (D0045)
//   UECSsetup()の後にcnd.mCDに起動メッセージを入れる。
//        起動メッセージは0x68010+VERSION
//        そうすると 10進数表記だと 4260xx(Version)になる。
//        cndフォーマット的には、
//           0110: スタンドアーロンモード
//           1000: 拡張用予約領域を使う
//           0x010: 10進数で000にするための補正 (D0043,D0044)
//   Debug用のSerial出力を止める。(D0042)
//   UserEveryMinute()に観測関係ルーチンを入れる (D0040)
//////////////////////////////////////////


#include <SPI.h>
#include <Ethernet2.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <EEPROM.h>
#include <Uardecs_mega.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <K30_I2C.h>
#include <RtcDS3231.h>
#include <AT24CX.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Adafruit_MCP9600.h>

#include "abco2.h"

uint8_t mcusr_mirror __attribute__ ((section (".noinit")));
void get_mcusr(void)	 \
  __attribute__((naked)) \
  __attribute__((section(".init3")));
void get_mcusr(void) {
  mcusr_mirror = MCUSR;
  MCUSR = 0;
  wdt_disable();
}


const char *VERSION = "D0057G";
const signed long ccmver = 0x68010 + 57;

/////////////////////////////////////
// Hardware Define
/////////////////////////////////////
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
char lcdtext[17];
char lcdtitle[17];

RtcDS3231<TwoWire> Rtc(Wire);
K30_I2C          k33lp  = K30_I2C(0x6c);
K30_I2C          k33icb = K30_I2C(0x6a);
AT24CX           atmem(7,32);
Adafruit_MCP9600 mcp[8];
int mcp96_addr[]={0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67};
boolean mcp96_present[8];  // true:present, false:absent

/////////
// NTP
/////////
#define NTPSERVER_NAME_LENGTH 32
#define NTPSERVER_NAME  0xfe0
#define NTPLOCAL_PORT   8123
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
char ntpServer[NTPSERVER_NAME_LENGTH];
//char dod[11],tod[9];      // Date of Date,Time of Date
int  ntprc;
EthernetUDP ntp;


/////////////////////////////////////
//IP reset jupmer pin setting
//IPアドレスリセット用ジャンパーピン設定
/////////////////////////////////////

const byte U_DhcpPin = 25;
const byte U_DhcpPin_Sense=HIGH;
const byte U_InitPin = 26;
const byte U_InitPin_Sense=LOW;

////////////////////////////////////
//Node basic infomation
///////////////////////////////////
const char U_name[] PROGMEM= "ABCO2A08";//MAX 20 chars
const char U_vender[] PROGMEM= "HOLLY";//MAX 20 chars
const char U_uecsid[] PROGMEM= "10100C009999";//12 chars fixed
const char U_footnote[] PROGMEM= "";
//const int U_footnoteLetterNumber = 48;//Abolished after Ver 0.6
char U_nodename[20] = "ABCO2A08";//MAX 19chars
UECSOriginalAttribute U_orgAttribute;

//////////////////////////////////
// html page1 setting
//////////////////////////////////
#define DECIMAL_DIGIT	1 //小数桁数




//表示素材の登録
const int U_HtmlLine = 35; //Total number of HTML table rows.
struct UECSUserHtml U_html[U_HtmlLine]={
  //{名前,    入出力形式,     単位,     詳細説明, 選択肢文字列,  選択肢数,値,	      最小値,最大値,小数桁数}
  {StrRUNMODE,UECSSELECTDATA, NONES,    VLVNOTE0, StrRUN,        3,       &(runMODE), 0,     0,     0},
  {StrMODESEL,UECSSELECTDATA, NONES,    VLVNOTE0, StrMODE,       9,       &(modeRUN), 0,     0,     0},
  {MOTONAME0, UECSSELECTDATA, NONES,    VLVNOTE0, StrMOTOSW,     3,       &(statusMOTO_ON_OFF_AUTO[0]),0,0,0},
  {MOTONAME1, UECSSELECTDATA, NONES,    VLVNOTE0, StrMOTOSW,     3,       &(statusMOTO_ON_OFF_AUTO[1]),0,0,0},
  {VLVNAME0,  UECSSELECTDATA, NONES,    VLVNOTE0, StrVLV_SELECT, 3, &(set_VLV_SELECT[0]), 0, 0, 0},
  {VLVNAME1,  UECSSELECTDATA, NONES,    VLVNOTE1, StrVLV_SELECT, 3, &(set_VLV_SELECT[1]), 0, 0, 0},
  {VLVNAME2,  UECSSELECTDATA, NONES,    VLVNOTE2, StrVLV_SELECT, 3, &(set_VLV_SELECT[2]), 0, 0, 0},
  {VLVNAME3,  UECSSELECTDATA, NONES,    VLVNOTE3, StrVLV_SELECT, 3, &(set_VLV_SELECT[3]), 0, 0, 0},
  {VLVNAME4,  UECSSELECTDATA, NONES,    VLVNOTE4, StrVLV_SELECT, 3, &(set_VLV_SELECT[4]), 0, 0, 0},
  {VLVNAME5,  UECSSELECTDATA, NONES,    VLVNOTE5, StrVLV_SELECT, 3, &(set_VLV_SELECT[5]), 0, 0, 0},
  {VLVNAME6,  UECSSELECTDATA, NONES,    VLVNOTE6, StrVLV_SELECT, 3, &(set_VLV_SELECT[6]), 0, 0, 0}, // #10
  {FUNCSEL,   UECSSHOWDATA,   NONES,    NONES,    DUMMY,    0,&(a2in),     0, 0, 0},
  {T1TEMP0,   UECSSHOWDATA,   TempUNIT, T1NOTE0,  DUMMY, 0,&(t1tValue[0])	, 0, 0, T1T_DECIMAL_DIGIT},
  {T1TEMP1,   UECSSHOWDATA,   TempUNIT, T1NOTE1,  DUMMY, 0,&(t1tValue[1])	, 0, 0, T1T_DECIMAL_DIGIT},
  {T1TEMP2,   UECSSHOWDATA,   TempUNIT, T1NOTE2,  DUMMY, 0,&(t1tValue[2])	, 0, 0, T1T_DECIMAL_DIGIT},
  {T1TEMP3,   UECSSHOWDATA,   TempUNIT, T1NOTE3,  DUMMY, 0,&(t1tValue[3])	, 0, 0, T1T_DECIMAL_DIGIT},
  {T1TEMP4,   UECSSHOWDATA,   TempUNIT, T1NOTE4,  DUMMY, 0,&(t1tValue[4])	, 0, 0, T1T_DECIMAL_DIGIT},
  {T1TEMP5,   UECSSHOWDATA,   TempUNIT, T1NOTE5,  DUMMY, 0,&(t1tValue[5])	, 0, 0, T1T_DECIMAL_DIGIT},
  {T1TEMP6,   UECSSHOWDATA,   TempUNIT, T1NOTE6,  DUMMY, 0,&(t1tValue[6])	, 0, 0, T1T_DECIMAL_DIGIT},
  {T1TEMP7,   UECSSHOWDATA,   TempUNIT, T1NOTE7,  DUMMY, 0,&(t1tValue[7])	, 0, 0, T1T_DECIMAL_DIGIT},
  {T1TEMP6,   UECSSHOWDATA,   TempUNIT, T1NOTE6,  DUMMY, 0,&(t1tValue[6])	, 0, 0, T1T_DECIMAL_DIGIT}, // #20
  {T1TEMP7,   UECSSHOWDATA,   TempUNIT, T1NOTE7,  DUMMY, 0,&(t1tValue[7])	, 0, 0, T1T_DECIMAL_DIGIT},
  {CO2NAME,   UECSSHOWDATA,   CO2UNIT1, CO2NOTE1, DUMMY, 0,&(co2detail), 0, 0, CO2_DIGIT},
  {CO2NAME,   UECSSHOWDATA,   CO2UNIT2, CO2NOTE2, DUMMY, 0,&(co2bigger), 0, 0, CO2_DIGIT},
  {BURNER_STATUS,UECSSHOWSTRING, NONES, NONES,    StrBURNER,   2,&(ShowBurner),0,0,0},
  {BURNER_FIRE,UECSSHOWSTRING, NONES, NONES, StrMISSFIRE, 2,&(ShowMissfire),0,0,0},
  {WATER_LVL1, UECSSHOWSTRING, NONES, NONES, StrWATER_LVL,2,&(ShowWaterLevel[0]),0,0,0},
  {WATER_LVL2, UECSSHOWSTRING, NONES, NONES, StrWATER_LVL,2,&(ShowWaterLevel[1]),0,0,0},
  {PRESS_LVL,  UECSSHOWSTRING, NONES, NONES, StrPRESS_LVL,2,&(ShowPressLevel[0]),0,0,0},
  {DATEMODE,   UECSSELECTDATA, NONES, DATEMODEdesc1, StrDATEMODE, 2, &(vDateMode), 0, 0, 0},
  {DAYSTART,   UECSINPUTDATA,  HOURUNIT, NONES, DUMMY,    0,&(daystart_hour), 0, 24, 0},
  {NIGHTSTART, UECSINPUTDATA,  HOURUNIT, NONES, DUMMY,    0,&(nightstart_hour), 0, 24, 0},
  {DAYSTATUS,  UECSSHOWSTRING, NONES, NONES, StrDAYSTATUS,2,&(ShowDayStatus),0,0,0},
  {T6TESTFLG,  UECSSELECTDATA, NONES, NONES, StrT6TEST,   2,&(vT6TestFlg), 0, 0, 0},
  {T6TESTVAL,  UECSINPUTDATA,  TempUNIT,NONES, DUMMY,     0,&(vT6tValue),-100,1000,T1T_DECIMAL_DIGIT}
};


//////////////////////////////////
// UserCCM setting
//////////////////////////////////
//define CCMID for identify
//CCMID_dummy must put on last
enum {
  CCMID_cnd,
  CCMID_FUNCSEL,
  CCMID_BLOWER,
  CCMID_PUMP,
  CCMID_TCTemp1,
  CCMID_TCTemp2,
  CCMID_TCTemp3,
  CCMID_TCTemp4,
  CCMID_TCTemp5,
  CCMID_TCTemp6,
  CCMID_TCTemp7,
  CCMID_TCTemp8,
  CCMID_K33LP,
  CCMID_K33ICB,
  CCMID_WL1,
  CCMID_WL2,
  CCMID_PRS1,
  CCMID_MODE,
  CCMID_RUNMODE,
  CCMID_BURNER,
  CCMID_DATEMODE,
  CCMID_DAYSTART,
  CCMID_NIGHTSTART,
  CCMID_T6TEST,
  CCMID_T6TESTVALUE,
  CCMID_dummy,
};


const int U_MAX_CCM = CCMID_dummy;
UECSCCM U_ccmList[U_MAX_CCM];

const char ccmUnitTemp[] PROGMEM= "C";
const char ccmNameTemp1[] PROGMEM= "T1温度";
const char ccmTypeTemp1[] PROGMEM= "TCTemp1.mCD";
const char ccmNameTemp2[] PROGMEM= "T2温度";
const char ccmTypeTemp2[] PROGMEM= "TCTemp2.mCD";
const char ccmNameTemp3[] PROGMEM= "T3温度";
const char ccmTypeTemp3[] PROGMEM= "TCTemp3.mCD";
const char ccmNameTemp4[] PROGMEM= "T4温度";
const char ccmTypeTemp4[] PROGMEM= "TCTemp4.mCD";
const char ccmNameTemp5[] PROGMEM= "T5温度";
const char ccmTypeTemp5[] PROGMEM= "TCTemp5.mCD";
const char ccmNameTemp6[] PROGMEM= "T6温度";
const char ccmTypeTemp6[] PROGMEM= "TCTemp6.mCD";
const char ccmNameTemp7[] PROGMEM= "T7温度";
const char ccmTypeTemp7[] PROGMEM= "TCTemp7.mCD";
const char ccmNameTemp8[] PROGMEM= "T8温度";
const char ccmTypeTemp8[] PROGMEM= "TCTemp8.mCD";

const char ccmUnitCO2[] PROGMEM= "ppm";
const char ccmNameCO2LP[] PROGMEM= "CO2LP";
const char ccmTypeCO2LP[] PROGMEM= "CO2LP1.mCD";
const char ccmNameCO2ICB[] PROGMEM= "CO2ICB";
const char ccmTypeCO2ICB[] PROGMEM= "CO2ICB1.mCD";

const char ccmNameWL1[] PROGMEM= "液面レベル1";
const char ccmTypeWL1[] PROGMEM= "waterLvl1.mCD";
const char ccmNameWL2[] PROGMEM= "液面レベル2";
const char ccmTypeWL2[] PROGMEM= "waterLvl2.mCD";

const char ccmNamePRS1[] PROGMEM= "圧力1";
const char ccmTypePRS1[] PROGMEM= "AirPress1.mCD";

const char ccmNameFUNC[] PROGMEM="FUNCSEL";
const char ccmTypeFUNC[] PROGMEM="funcsel.mCD";
const char ccmUnitFUNC[] PROGMEM= "";

const char ccmNameBLOWER[] PROGMEM="ブロアー";
const char ccmTypeBLOWER[] PROGMEM="blower.mCD";
const char ccmUnitBLOWER[] PROGMEM= "";

const char ccmNamePUMP[] PROGMEM="ポンプ";
const char ccmTypePUMP[] PROGMEM="pump.mCD";
const char ccmUnitPUMP[] PROGMEM= "";

const char ccmNameBURNER[] PROGMEM="バーナー状態";
const char ccmTypeBURNER[] PROGMEM="burner.mCD";
const char ccmUnitBURNER[] PROGMEM= "";

const char ccmNameMODE[] PROGMEM="Mode";
const char ccmTypeMODE[] PROGMEM="mode.mCD";
const char ccmUnitMODE[] PROGMEM= "";

const char ccmNameRunMODE[] PROGMEM="RunMode";
const char ccmTypeRunMODE[] PROGMEM="runmode.mCD";
const char ccmUnitRunMODE[] PROGMEM= "";

const char ccmNameDateMode[] PROGMEM="昼夜設定";
const char ccmTypeDateMode[] PROGMEM="dateMode.mCD";
const char ccmUnitDateMode[] PROGMEM="時";

const char ccmNameDayStart[] PROGMEM="昼間開始時刻";
const char ccmTypeDayStart[] PROGMEM="dayStart.mCD";
const char ccmUnitDayStart[] PROGMEM="時";

const char ccmNameNightStart[] PROGMEM="夜間開始時刻";
const char ccmTypeNightStart[] PROGMEM="nightStart.mCD";
const char ccmUnitNightStart[] PROGMEM="時";

const char ccmNameT6Test[] PROGMEM = "T-6温度設定";
const char ccmTypeT6Test[] PROGMEM = "t6test.mCD";
const char ccmUnitT6Test[] PROGMEM = "";

const char ccmNameT6TestTemp[] PROGMEM = "T-6温度";
const char ccmTypeT6TestTemp[] PROGMEM = "t6testTemp.mCD";

const char ccmNameCnd[] PROGMEM= "NodeCondition";
const char ccmTypeCnd[] PROGMEM= "cnd.mCD";
const char ccmUnitCnd[] PROGMEM= "";

//
//  Over Heat condition data
//
int   oht_status = 0;
int   oht_counter= 5;  // 単位は分 void UserEveryMinute() 内で減算していく

void UserInit(){
  //MAC address is printed on sticker of Ethernet Shield.
  //You must assign unique MAC address to each nodes.
  //MACアドレス設定、必ずEthernet Shieldに書かれた値を入力して下さい。
  //全てのノードに異なるMACアドレスを設定する必要があります。
  //U_orgAttribute.mac[0] = EEPROM.read(4090+0); // 0x02;
  //U_orgAttribute.mac[1] = EEPROM.read(4090+1); // 0xa2;
  //U_orgAttribute.mac[2] = EEPROM.read(4090+2); // 0x73;
  //U_orgAttribute.mac[3] = EEPROM.read(4090+3); // 0x8f;
  //U_orgAttribute.mac[4] = EEPROM.read(4090+4); // 0x00;
  //U_orgAttribute.mac[5] = EEPROM.read(4090+5); // 0x01;
  U_orgAttribute.mac[0] = 0x02;
  U_orgAttribute.mac[1] = 0xa2;
  U_orgAttribute.mac[2] = 0x73;
  U_orgAttribute.mac[3] = 0x8f;
  U_orgAttribute.mac[4] = 0x00;
  U_orgAttribute.mac[5] = 0x02;

  wdt_reset();
  //Set ccm list
  UECSsetCCM(true, CCMID_cnd   ,  ccmNameCnd ,  ccmTypeCnd ,  ccmUnitCnd , 29, 0, A_1S_0);
  UECSsetCCM(true, CCMID_MODE,    ccmNameMODE,  ccmTypeMODE,  ccmUnitMODE, 28, 0, A_1S_0);
  UECSsetCCM(true, CCMID_RUNMODE, ccmNameRunMODE,ccmTypeRunMODE,ccmUnitRunMODE, 28, 0, A_1S_0);
  UECSsetCCM(true, CCMID_FUNCSEL, ccmNameFUNC,  ccmTypeFUNC,  ccmUnitFUNC, 29, 0, A_1S_0);
  UECSsetCCM(true, CCMID_BLOWER,  ccmNameBLOWER,ccmTypeBLOWER,ccmUnitBLOWER,29, 0, A_10S_0);
  UECSsetCCM(true, CCMID_PUMP,    ccmNamePUMP,  ccmTypePUMP,  ccmUnitPUMP, 29, 0, A_10S_0);
  UECSsetCCM(true, CCMID_TCTemp1, ccmNameTemp1, ccmTypeTemp1, ccmUnitTemp, 29, 1, A_10S_0);
  UECSsetCCM(true, CCMID_TCTemp2, ccmNameTemp2, ccmTypeTemp2, ccmUnitTemp, 29, 1, A_10S_0);
  UECSsetCCM(true, CCMID_TCTemp3, ccmNameTemp3, ccmTypeTemp3, ccmUnitTemp, 29, 1, A_10S_0);
  UECSsetCCM(true, CCMID_TCTemp4, ccmNameTemp4, ccmTypeTemp4, ccmUnitTemp, 29, 1, A_10S_0);
  UECSsetCCM(true, CCMID_TCTemp5, ccmNameTemp5, ccmTypeTemp5, ccmUnitTemp, 29, 1, A_10S_0);
  UECSsetCCM(true, CCMID_TCTemp6, ccmNameTemp6, ccmTypeTemp6, ccmUnitTemp, 29, 1, A_10S_0);
  UECSsetCCM(true, CCMID_TCTemp7, ccmNameTemp7, ccmTypeTemp7, ccmUnitTemp, 29, 1, A_10S_0);
  UECSsetCCM(true, CCMID_TCTemp8, ccmNameTemp8, ccmTypeTemp8, ccmUnitTemp, 29, 1, A_10S_0);
  UECSsetCCM(true, CCMID_K33LP,   ccmNameCO2LP, ccmTypeCO2LP, ccmUnitCO2,  29, 0, A_10S_0);
  UECSsetCCM(true, CCMID_K33ICB,  ccmNameCO2ICB,ccmTypeCO2ICB,ccmUnitCO2,  29, 0, A_10S_0);
  UECSsetCCM(true, CCMID_WL1,     ccmNameWL1,   ccmTypeWL1,   NONES,       29, 0, A_10S_0);
  UECSsetCCM(true, CCMID_WL2,     ccmNameWL2,   ccmTypeWL2,   NONES,       29, 0, A_10S_0);
  UECSsetCCM(true, CCMID_PRS1,    ccmNamePRS1,  ccmTypePRS1,  NONES,       29, 0, A_10S_0);
  UECSsetCCM(true, CCMID_BURNER,  ccmNameBURNER,ccmTypeBURNER,NONES,       29, 0, A_10S_0);
  UECSsetCCM(true, CCMID_DATEMODE,ccmNameDateMode,ccmTypeDateMode,ccmUnitDateMode, 29, 0, A_10S_0);
  UECSsetCCM(true, CCMID_DAYSTART,ccmNameDayStart,ccmTypeDayStart,ccmUnitDayStart, 29, 0, A_10S_0);
  UECSsetCCM(true, CCMID_NIGHTSTART,ccmNameNightStart,ccmTypeNightStart,ccmUnitNightStart, 29, 0, A_10S_0);
  UECSsetCCM(true, CCMID_T6TEST,ccmNameT6Test,ccmTypeT6Test,NONES, 29, 0, A_10S_0);
  UECSsetCCM(true, CCMID_T6TESTVALUE,ccmNameT6TestTemp,ccmTypeT6TestTemp,ccmUnitTemp,29, 1, A_10S_0);
}


void OnWebFormRecieved() {
  U_ccmList[CCMID_BLOWER].value  = statusMOTO_ON_OFF_AUTO[0];
  U_ccmList[CCMID_PUMP].value    = statusMOTO_ON_OFF_AUTO[1];
  U_ccmList[CCMID_MODE].value    = modeRUN;
  U_ccmList[CCMID_RUNMODE].value = runMODE;
  U_ccmList[CCMID_DATEMODE].value = vDateMode;
  U_ccmList[CCMID_DAYSTART].value = daystart_hour;
  U_ccmList[CCMID_NIGHTSTART].value = nightstart_hour;
  U_ccmList[CCMID_T6TEST].value       = vT6TestFlg; /* 0: Normal, 1:Force */
  U_ccmList[CCMID_T6TESTVALUE].value  = vT6tValue;  /* Simulated Temperature */
  ChangeValve();
}

void UserEverySecond() {
  RtcDateTime now = Rtc.GetDateTime();
  // 昼夜の判定
  if (U_ccmList[CCMID_DATEMODE].value == 0) { // 昼夜設定自動
    if ((now.Hour()>6)&&(now.Hour()<18)) { // 自動なら
      DayTime = true;
    } else {
      DayTime = false;
    }
  } else { 
    if ((now.Hour()>U_ccmList[CCMID_DAYSTART].value)&&(now.Hour()<U_ccmList[CCMID_NIGHTSTART].value)) {
      DayTime = true;
    } else {
      DayTime = false;
    }
  }
  if (DayTime==true) {
    ShowDayStatus = 0;
  } else {
    ShowDayStatus = 1;
  }
  if (U_ccmList[CCMID_RUNMODE].value == 0) { // RUN MODE is RUNAUTO (自動)
    if (oht_status==0) { // オーバーヒートしていないならば
      //    Serial.begin(115200);
      //Serial.print(now.Hour(),DEC);
      if (DayTime) { // 昼ならば
	U_ccmList[CCMID_MODE].value = 2;
	modeRUN = 2;
	setMode2();
      } else { // 昼ではないならば
	if (U_ccmList[CCMID_BURNER].value == 0) { // Burner STOPPED
	  //Serial.println(" and BURNER STOPPED setMode0()");
	  U_ccmList[CCMID_MODE].value = 0;
	  modeRUN = 0;
	  setMode0();
	} else {      // Burner RUNNING
	  //Serial.println(" and BURNER RUNNING setMode1()");
	  U_ccmList[CCMID_MODE].value = 1;
	  modeRUN = 1;
	  setMode1();
	}
      }
    //Serial.end();
    } else { // オーバーヒートしているならば
    }
  }
  wdt_reset();
}
void UserEveryMinute() {
  float temp;
  int   mcp_id;
  k33_ope();
  if ( oht_status != 0 ) {
    oht_counter--;
    if ( oht_counter < 0 ) oht_counter = 0;
  }
  for(mcp_id=0;mcp_id<8;mcp_id++) {
    if (mcp96_present[mcp_id]) {
      temp = mcp[mcp_id].readThermocouple();
    } else {
      temp = -10.0;
    }
    if ((mcp_id==5)&&(U_ccmList[CCMID_T6TEST].value==1)) {  /* mcp_id=5 is T-6 , 0: Normal, 1:Force */
      temp = U_ccmList[CCMID_T6TESTVALUE].value ;  /* Simulated Temperature */
    }
    t1tValue[mcp_id] = temp;
    U_ccmList[CCMID_TCTemp1+mcp_id].value = temp;
  }
  over_heat_recovery(50.0);  //  T-6 オーバーヒート確認
}
void UserEveryLoop() {
}

/////////////////////////////////////////////////////
// LOOP MAIN ROUTINE
/////////////////////////////////////////////////////
void loop(){
  int rc,co2lp,co2icb;
  int mcp_id;
  UECSloop();
  if (U_ccmList[CCMID_cnd].value==ccmver) {
    U_ccmList[CCMID_cnd].value=0;
  }
  if (digitalRead(EMGSTOP)==LOW) emgstop(); // 緊急停止の確認　割り込みが効かないのでここに入れる。
  a2in = analogRead(A2);
  U_ccmList[CCMID_FUNCSEL].value= a2in;
  disp_select(a2in);
  if (digitalRead(PRSLVL1)==HIGH) {
    ShowPressLevel[0] = 0; // HIGH
  } else {
    ShowPressLevel[0] = 1; // LOW
  }
  U_ccmList[CCMID_PRS1].value = ShowPressLevel[0];
  if (digitalRead(WLVL1)==HIGH) {
    ShowWaterLevel[0] = 0; // HIGH
  } else {
    ShowWaterLevel[0] = 1; // LOW
  }
  U_ccmList[CCMID_WL1].value = ShowWaterLevel[0];
  if (digitalRead(WLVL2)==HIGH) {
    ShowWaterLevel[1] = 0; // HIGH
  } else {
    ShowWaterLevel[1] = 1; // LOW
  }
  U_ccmList[CCMID_WL2].value = ShowWaterLevel[1];
  if (digitalRead(BLOWER)==HIGH) {
    U_ccmList[CCMID_BLOWER].value = 1; // RUN
    statusMOTO_ON_OFF_AUTO[0] = 1;
  } else {
    U_ccmList[CCMID_BLOWER].value = 2; // STOP
    statusMOTO_ON_OFF_AUTO[0] = 2;
  }
  if (digitalRead(PUMP)==HIGH) {
    U_ccmList[CCMID_PUMP].value = 1; // RUN
    statusMOTO_ON_OFF_AUTO[1] = 1;
  } else {
    U_ccmList[CCMID_PUMP].value = 2; // STOP
    statusMOTO_ON_OFF_AUTO[1] = 2;
  }
  if (digitalRead(BURNER)==HIGH) {
    U_ccmList[CCMID_BURNER].value = 0;
    ShowBurner = 0; // STOP
  } else {
    U_ccmList[CCMID_BURNER].value = 1;
    ShowBurner = 1; // RUN
  }
  ChangeValve();
}

//
//  SETUP START HERE
//
//
void setup(){
  //  extern byte megaEtherSS;
  int i;
  //  megaEtherSS = 53; // SS is pin 53
  //  pinMode(26,OUTPUT);
  //  digitalWrite(26,LOW);
  configure_wdt();
  UECSsetup();
  U_ccmList[CCMID_cnd].value = ccmver;
  pinMode(A2,INPUT);
  for (i=0;i<8;i++) {
    VLVStatus[i] = 1;
  }
  for (i=0;i<2;i++) {
    statusMOTO_ON_OFF_AUTO[i] = MOTO_STOP;
  }
  //  delay(500);
  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  pinMode(18,INPUT_PULLUP);
  pinMode(BURNER,INPUT_PULLUP);
  pinMode(ALARM_MISSFIRE,INPUT_PULLUP);
  pinMode(WLVL1,INPUT);
  pinMode(WLVL2,INPUT);
  pinMode(PRSLVL1,INPUT);
  pinMode(PRSLVL2,INPUT);

  // Setup OUTPUT Pins
  for(i=22;i<=24;i++) {
    pinMode(i,OUTPUT);
    digitalWrite(i,LOW);
  }
  for(i=30;i<=43;i++) {
    pinMode(i,OUTPUT);
    digitalWrite(i,LOW);
  }

  // Full Open Valve
  setMode6();

  init_mcp9600();
  sprintf(lcdtitle,"ABCO2 %6s",VERSION);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print(lcdtitle);
  sprintf(lcdtext,"%d.%d.%d.%d",U_orgAttribute.ip[0],U_orgAttribute.ip[1],U_orgAttribute.ip[2],U_orgAttribute.ip[3]);
  lcd.setCursor(0,1);
  lcd.print(Ethernet.localIP());
  ntprc = ntp_ope();
  wdt_reset();
  vT6TestFlg = 0;
  lcd.setCursor(0,1);
  lcd.print("EXIT setup()    ");
}

//---------------------------------------------------------
//バルブ動作を変化させる関数
//---------------------------------------------------------
void ChangeValve(){
  switch(U_ccmList[CCMID_RUNMODE].value) {
  case 1: //  プリセットモードならば
    switch(U_ccmList[CCMID_MODE].value) {
    case 0:  // MODE0:
      setMode0();
      return;
    case 1:  // MODE1:
      setMode1();
      return;
    case 2:  // MODE2:
      setMode2();
      return;
    case 3:  // MODE3:
      setMode3();
      return;
    case 4:  // MODE4:
      setMode4();
      return;
    case 5:  // MODE5:
      setMode5();
      return;
    case 6:  // MODE6:
      setMode6();
      return;
    case 7:  // MODE7:
      setMode7();
      return;
    }
    break;
  case 2:// 手動
    U_ccmList[CCMID_MODE].value = 8; // FREE-MODE
    modeRUN = 8;
    switch(U_ccmList[CCMID_BLOWER].value) {
    case 1: //MOTO_RUN
      run_blower();
      break;
    case 2: //MOTO_STOP
      stop_blower();
      break;
    }
    switch(U_ccmList[CCMID_PUMP].value) {
    case 1: //MOTO_RUN
      run_pump();
      break;
    case 2: //MOTO_STOP
      stop_pump();
      break;
    }
    switch(set_VLV_SELECT[0]) {
    case 0:
      vlv_ctrl(VLV1_UNKNOWN,CCMID_cnd);
      break;
    case 1:
      vlv_ctrl(VLV1_CLOSE,CCMID_cnd);
      break;
    case 2:
      vlv_ctrl(VLV1_OPEN,CCMID_cnd);
      break;
    }
    switch(set_VLV_SELECT[1]) {
    case 0:
      vlv_ctrl(VLV2_UNKNOWN,CCMID_cnd);
      break;
    case 1:
      vlv_ctrl(VLV2_CLOSE,CCMID_cnd);
      break;
    case 2:
      vlv_ctrl(VLV2_OPEN,CCMID_cnd);
      break;
    }
    switch(set_VLV_SELECT[2]) {
    case 0:
      vlv_ctrl(VLV3_UNKNOWN,CCMID_cnd);
      break;
    case 1:
      vlv_ctrl(VLV3_CLOSE,CCMID_cnd);
      break;
    case 2:
      vlv_ctrl(VLV3_OPEN,CCMID_cnd);
      break;
    }
    switch(set_VLV_SELECT[3]) {
    case 0:
      vlv_ctrl(VLV4_UNKNOWN,CCMID_cnd);
      break;
    case 1:
      vlv_ctrl(VLV4_CLOSE,CCMID_cnd);
      break;
    case 2:
      vlv_ctrl(VLV4_OPEN,CCMID_cnd);
      break;
    }
    switch(set_VLV_SELECT[4]) {
    case 0:
      vlv_ctrl(VLV5_UNKNOWN,CCMID_cnd);
      break;
    case 1:
      vlv_ctrl(VLV5_CLOSE,CCMID_cnd);
      break;
    case 2:
      vlv_ctrl(VLV5_OPEN,CCMID_cnd);
      break;
    }
    switch(set_VLV_SELECT[5]) {
    case 0:
      vlv_ctrl(VLV6_UNKNOWN,CCMID_cnd);
      break;
    case 1:
      vlv_ctrl(VLV6_CLOSE,CCMID_cnd);
      break;
    case 2:
      vlv_ctrl(VLV6_OPEN,CCMID_cnd);
      break;
    }
    switch(set_VLV_SELECT[6]) {
    case 0:
      vlv_ctrl(VLV7_UNKNOWN,CCMID_cnd);
      break;
    case 1:
      vlv_ctrl(VLV7_CLOSE,CCMID_cnd);
      break;
    case 2:
      vlv_ctrl(VLV7_OPEN,CCMID_cnd);
      break;
    }
    break;
  }
}

void run_blower(void) {
  U_ccmList[CCMID_cnd].value |= 0b100000000;  // RUN
  digitalWrite(BLOWER,HIGH);
}

void stop_blower(void) {
  U_ccmList[CCMID_cnd].value &= 0b01111111111111110000111011111111;  // STOP
  digitalWrite(BLOWER,LOW);
}

void run_pump(void) {
  U_ccmList[CCMID_cnd].value |= 0b10000000;  // RUN
  digitalWrite(D_PUMP,HIGH);
}

void stop_pump(void) {
  U_ccmList[CCMID_cnd].value &= 0b01111111111111110000111101111111;  // STOP
  digitalWrite(D_PUMP,LOW);
}

void Reset_lcdtext(void) {
  int i;
  for(i=0;i<17;i++) {
    lcdtext[i] = (char)NULL;
  }
}
//
//  float oht = オーバーヒート温度
//
void over_heat_recovery(float oht) {
  // T6 t1tValue[5]
  int i;
  Serial.begin(115200);
  Serial.print("Enter over_heat_recovery(");
  Serial.print(oht);
  Serial.println(")");
  for(i=0;i<8;i++) {
    Serial.print("t1tValue[");
    Serial.print(i);
    Serial.print("]=");
    Serial.println(t1tValue[i]);
  }
  Serial.print("oht_counter=");
  Serial.println(oht_counter);
  Serial.print("oht_status=");
  Serial.println(oht_status);
  Serial.end();
  if (t1tValue[5]>=(oht*10)) {
    if (oht_status==0) { // Over Heatになったばかり
      PmodeRUN = modeRUN; // 直前の動作モードを保存する
      oht_status = 1;     // Over Heat 冷却 Status に変更する
      oht_counter = 5;    // 冷却時間は5min UserEveryMinute() にてデクリメントする
    }
    switch(oht_status) {
    case 1: //  MODE-5 冷却
      if (oht_counter>0) {
	U_ccmList[CCMID_MODE].value = 5;
	modeRUN = 5;
	setMode5(); // 5min
      } else {
	oht_status = 2;
	oht_counter=5;
      }
      break;
    case 2: // MODE-6  全開
      if (oht_counter>0) {
	U_ccmList[CCMID_MODE].value = 6;
	modeRUN = 6;
	setMode6(); // 5min
      } else {
	oht_status = 1;
	oht_counter = 5;
      }
      break;
    }
  } else {
    oht_status = 0;
    oht_counter = 5;
    modeRUN = PmodeRUN;
    U_ccmList[CCMID_MODE].value = modeRUN;
  }
}

void emgstop(void) {
  lcd.setCursor(0,1);
  Reset_lcdtext();
  lcd.print(lcdtext);
  lcd.setCursor(0,1);
  lcd.print("EMG STOP        ");
  stop_blower();
  stop_pump();
  delay(500);
  if (digitalRead(EMGSTOP)==HIGH) {
    lcd.setCursor(0,1);
    lcd.print("EMG ABORT       ");
    return;
  }
  lcd.print("BLOWER/PUMP STOP");
  vlv_ctrl(VLV1_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV2_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV3_CLOSE,CCMID_cnd);
  vlv_ctrl(VLV4_OPEN,CCMID_cnd);
  vlv_ctrl(VLV5_OPEN,CCMID_cnd);
  vlv_ctrl(VLV6_OPEN,CCMID_cnd);
  vlv_ctrl(VLV7_CLOSE,CCMID_cnd);
  U_ccmList[CCMID_cnd].value &= 0b01111111111111110000111000111000;  // MOTORT and VALVE
  U_ccmList[CCMID_cnd].value |= 0b01000000000000000000000000000000;  // EMERGENCY STOP
  delay(2000);
}

void configure_wdt(void) {
  cli();                           // disable interrupts for changing the registers
  MCUSR = 0;                       // reset status register flags
                                   // Put timer in interrupt-only mode:
  WDTCSR |= 0b00011000;            // Set WDCE (5th from left) and WDE (4th from left) to enter config mode,
                                   // using bitwise OR assignment (leaves other bits unchanged).
  WDTCSR =  0b00001000 | 0b100001; // clr WDIE: interrupt enabled
                                   // set WDE: reset disabled
                                   // and set delay interval (right side of bar) to 8 seconds
  sei();                           // re-enable interrupts
                                   // reminder of the definitions for the time before firing
                                   // delay interval patterns:
                                   //  16 ms:     0b000000
                                   //  500 ms:    0b000101
                                   //  1 second:  0b000110
                                   //  2 seconds: 0b000111
                                   //  4 seconds: 0b100000
                                   //  8 seconds: 0b100001
}
