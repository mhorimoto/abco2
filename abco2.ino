/////////////////////////////////////////
// -*- mode : C++ -*-
//[概要]
// ABCO2の基幹的プログラム
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


const char *VERSION = "D0053";
const signed long ccmver = 0x68010 + 53;

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
const char WATER_LVL1[] PROGMEM = "液面レベル1";
const char WATER_LVL2[] PROGMEM = "液面レベル2";
const char WLVL_HIGH[] PROGMEM = "HIGH";
const char WLVL_LOW[] PROGMEM  = "LOW";
const char *StrWATER_LVL[2] = {
			       WLVL_LOW,
			       WLVL_HIGH
};
boolean waterLevel[2];          // I/Oから信号を受電するため
signed long ShowWaterLevel[2];  // 表示のインデックスのため

// 圧力センサ
// H/Lの二者択一。UECSSHOWSTRING で表示する。
const char PRESS_LVL[] PROGMEM  = "圧力";
const char PRESS_HIGH[] PROGMEM = "HIGH";
const char PRESS_LOW[] PROGMEM  = "LOW";
const char *StrPRESS_LVL[2] = {
			       PRESS_LOW,
			       PRESS_HIGH
};
boolean pressLevel;          // I/Oから信号を受電するため
signed long ShowPressLevel[2];  // 表示のインデックスのため


// バーナー
// H/Lの二者択一。UECSSHOWSTRING で表示する。
const char BURNER_STATUS[] PROGMEM = "バーナー運転";
const char BURNER_FIRE[] PROGMEM = "バーナー火状態";
const char BURNER_RUN[] PROGMEM = "運転";
const char BURNER_STOP[] PROGMEM  = "停止";
const char *StrBURNER[2] = {
  BURNER_STOP,
  BURNER_RUN
};
boolean sts_burner;          // I/Oから信号を受電するため
signed long ShowBurner;      // 表示のインデックスのため

const char MISSFIRE[] PROGMEM = "失火";
const char FIRE[] PROGMEM  = "正常";
const char *StrMISSFIRE[2] = {
  FIRE,
  MISSFIRE
};
boolean sts_fire;          // I/Oから信号を受電するため
signed long ShowMissfire;  // 表示のインデックスのため

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
const char VLV_CLOSE[] PROGMEM= "CLOSE";
const char VLV_OPEN[] PROGMEM= "OPEN";
const char VLV_VOFF[] PROGMEM= "OFF";
const char *StrVLV_SELECT[3]={
  VLV_VOFF,
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

// Function Selector
//
const char FUNCSEL[] PROGMEM = "FUNCSEL";
unsigned long a2in;


//●ダミー素材の定義
//dummy value
const char NONES[] PROGMEM= "";
const char** DUMMY = NULL;

// 自動モードと手動モードの切り替え
const char StrMODESEL[] PROGMEM="モード選択";
const char MODE0[] PROGMEM="MODE-0";
const char MODE1[] PROGMEM="MODE-1";
const char MODE2[] PROGMEM="MODE-2";
const char MODE3[] PROGMEM="MODE-3";
const char MODE4[] PROGMEM="MODE-4";
const char MODE5[] PROGMEM="MODE-5";
const char MODE6[] PROGMEM="MODE-6";
const char MODE7[] PROGMEM="MODE-7";
const char MODEX[] PROGMEM="FREE";
const char *StrMODE[9] = {
  MODE0,     //  0
  MODE1,     //  1
  MODE2,     //  2
  MODE3,     //  3
  MODE4,     //  4
  MODE5,     //  5
  MODE6,     //  6
  MODE7,     //  7
  MODEX      //  8
};
signed long modeRUN,PmodeRUN;

const char StrRUNMODE[]  PROGMEM="運転種別";
const char RUNAUTO[]     PROGMEM="自動";
const char RUNMANU[]     PROGMEM="手動";
const char RUNFREE[]     PROGMEM="自由";
const char *StrRUN[3] = {
  RUNAUTO,  //  0
  RUNMANU,  //  1
  RUNFREE   //  2
};
signed long runMODE,PrunMODE;

//表示素材の登録
const int U_HtmlLine = 29; //Total number of HTML table rows.
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
  {CO2NAME,   UECSSHOWDATA,   CO2UNIT,  CO2NOTE1, DUMMY, 0,&(co2detail), 0, 0, CO2_DIGIT},
  {CO2NAME,   UECSSHOWDATA,   CO2UNIT,  CO2NOTE2, DUMMY, 0,&(co2bigger), 0, 0, CO2_DIGIT},
  {BURNER_STATUS,UECSSHOWSTRING, NONES, NONES,    StrBURNER,   2,&(ShowBurner),0,0,0},
  {BURNER_FIRE,UECSSHOWSTRING, NONES, NONES, StrMISSFIRE, 2,&(ShowMissfire),0,0,0},
  {WATER_LVL1, UECSSHOWSTRING, NONES, NONES, StrWATER_LVL,2,&(ShowWaterLevel[0]),0,0,0},
  {WATER_LVL2, UECSSHOWSTRING, NONES, NONES, StrWATER_LVL,2,&(ShowWaterLevel[1]),0,0,0},
  {PRESS_LVL,  UECSSHOWSTRING, NONES, NONES, StrPRESS_LVL,2,&(ShowPressLevel[0]),0,0,0},
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

const char ccmNameCnd[] PROGMEM= "NodeCondition";
const char ccmTypeCnd[] PROGMEM= "cnd.mCD";
const char ccmUnitCnd[] PROGMEM= "";


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
}


void OnWebFormRecieved() {
  U_ccmList[CCMID_BLOWER].value  = statusMOTO_ON_OFF_AUTO[0];
  U_ccmList[CCMID_PUMP].value    = statusMOTO_ON_OFF_AUTO[1];
  U_ccmList[CCMID_MODE].value    = modeRUN;
  U_ccmList[CCMID_RUNMODE].value = runMODE;
  ChangeValve();
}

void UserEverySecond() {
  wdt_reset();
}
void UserEveryMinute() {
  float temp;
  int   mcp_id;
  RtcDateTime now = Rtc.GetDateTime();
  if (U_ccmList[CCMID_RUNMODE].value == 0) { // RUN MODE is AUTO
    //    Serial.begin(115200);
    //Serial.print(now.Hour(),DEC);
    if ((now.Hour()>6)&&(now.Hour()<18)) {
      //Serial.println(" setMode2()");
      U_ccmList[CCMID_MODE].value = 2;
      modeRUN = 2;
      setMode2();
    } else {
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
  }
  k33_ope();
  for(mcp_id=0;mcp_id<8;mcp_id++) {
    if (mcp96_present[mcp_id]) {
      temp = mcp[mcp_id].readThermocouple();
    } else {
      temp = -10.0;
    }
    t1tValue[mcp_id] = temp * 10.0;
    U_ccmList[CCMID_TCTemp1+mcp_id].value = temp * 10.0;
  }
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
  vlv_ctrl(VLV1_OPEN,CCMID_cnd);
  vlv_ctrl(VLV2_OPEN,CCMID_cnd);
  vlv_ctrl(VLV3_OPEN,CCMID_cnd);
  vlv_ctrl(VLV4_OPEN,CCMID_cnd);
  vlv_ctrl(VLV5_OPEN,CCMID_cnd);
  vlv_ctrl(VLV6_OPEN,CCMID_cnd);
  vlv_ctrl(VLV7_OPEN,CCMID_cnd);
  stop_blower();
  stop_pump();
  U_ccmList[CCMID_cnd].value &= 0b00111111111111111111111111111111;  // RESET E-STOP BIT

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
  lcd.setCursor(0,1);
  lcd.print("EXIT setup()    ");
}

//---------------------------------------------------------
//バルブ動作を変化させる関数
//---------------------------------------------------------
void ChangeValve(){
  switch(U_ccmList[CCMID_RUNMODE].value) {
  case 1: //  手動モード切替
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
  case 2:// 自由切替
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
