#define RXD     0    /* PROGRAMING INTERFACE */
#define TXD     1    /* PROGRAMING INTERFACE */
#define EMGSTOP 2    /* EMERGENCY STOP SWITCH */
#define OKGOSW  3    /* OK,GO,YES SWITCH */
#define VLV1OLS 4    /* VLV1 OLS(バルブ全開) */
#define VLV1SLS 5    /* VLV1 SLS(バルブ全閉) */
#define VLV2OLS 6    /* VLV2 OLS(バルブ全開) */
#define VLV2SLS 7    /* VLV2 SLS(バルブ全閉) */
#define VLV3OLS 8    /* VLV3 OLS(バルブ全開) */
#define VLV3SLS 9    /* VLV3 SLS(バルブ全閉) */
#define VLV4OLS 10   /* VLV4 OLS(バルブ全開) */
#define VLV4SLS 11   /* VLV4 SLS(バルブ全閉) */
#define VLV5OLS 12   /* VLV5 OLS(バルブ全開) */
#define VLV5SLS 13   /* VLV5 SLS(バルブ全閉) */
#define VLV6OLS 14   /* VLV6 OLS(バルブ全開) */
#define VLV6SLS 15   /* VLV6 SLS(バルブ全閉) */
#define VLV7OLS 16   /* VLV7 OLS(バルブ全開) */
#define VLV7SLS 17   /* VLV7 SLS(バルブ全閉) */
#define CANCEL  18   /* CANCEL BUTTON */
#define BURNER  19   /* RELAY RESERVED */
#define SDA     20   /* I2C Data  LCD,熱電対,K33LP,K33ICB */
#define SCL     21   /* I2C Clock */
#define BLOWER  22   /* BLOWER MOTOR RELAY-1 */
#define PUMP    23   /* PUMP MOTOR RELAY-2 */
#define RLY3    24   /* RELAY RESERVED */
#define UECSDHCP 25   /* UECS RESERVED */
#define UECSSAFE 26   /* UECS RESERVED */
#define ALARM_MISSFIRE     27   /* 着火エラー */
#define RLY7    28   /* RELAY RESERVED */
#define RLY8    29   /* RELAY RESERVED */
#define VLV1A   30   /* VALVE1 OPENNER A */
#define VLV1B   31   /* VALVE1 OPENNER B */
#define VLV2A   32   /* VALVE2 OPENNER A */
#define VLV2B   33   /* VALVE2 OPENNER B */
#define VLV3A   34   /* VALVE3 OPENNER A */
#define VLV3B   35   /* VALVE3 OPENNER B */
#define VLV4A   36   /* VALVE4 OPENNER A */
#define VLV4B   37   /* VALVE4 OPENNER B */
#define VLV5A   38   /* VALVE5 OPENNER A */
#define VLV5B   39   /* VALVE5 OPENNER B */
#define VLV6A   40   /* VALVE6 OPENNER A */
#define VLV6B   41   /* VALVE6 OPENNER B */
#define VLV7A   42   /* VALVE7 OPENNER A */
#define VLV7B   43   /* VALVE7 OPENNER B */
#define VLV8A   44   /* VALVE8 OPENNER A */
#define VLV8B   45   /* VALVE8 OPENNER B */
#define WLVL1   46   /* 液面レベル1 */
#define WLVL2   47   /* 液面レベル2 */
#define PRSLVL1 48   /* 圧力センサ1 */
#define PRSLVL2 49   /* 圧力センサ2 */
#define MISO    50   /* W5500 LAN Interface */
#define MOSI    51   /* W5500 LAN Interface */
#define SCK     52   /* W5500 LAN Interface */
#define SS      53   /* W5500 LAN Interface */

#define  MAC_ADDR_PTR   0xffa   // DEC=4090 for MEGA
#define  ATMEM_LTT      0xfd0   // EEPROM ADDRESS IN AT24C32
#define  ATMEM_LGT      0xfd8   // EEPROM ADDRESS IN AT24C32
#define  ATMEM_NTPSRV   0xfe0   // EEPROM ADDRESS IN AT24C32

#define  D_BLOWER      22
#define  D_PUMP        23
#define  D_SPARE       24
#define  D_VLV1_NORM   30
#define  D_VLV1_REV    31
#define  D_VLV1_OLS     4
#define  D_VLV1_SLS     5
#define  VLV1_UNKNOWN 310
#define  VLV1_OPEN    311
#define  VLV1_CLOSE   312
#define  D_VLV2_NORM   32
#define  D_VLV2_REV    33
#define  D_VLV2_OLS     6
#define  D_VLV2_SLS     7
#define  VLV2_UNKNOWN 320
#define  VLV2_OPEN    321
#define  VLV2_CLOSE   322

#define  D_VLV3_NORM   34
#define  D_VLV3_REV    35
#define  D_VLV3_OLS     8
#define  D_VLV3_SLS     9
#define  VLV3_UNKNOWN 330
#define  VLV3_OPEN    331
#define  VLV3_CLOSE   332
#define  D_VLV4_NORM   36
#define  D_VLV4_REV    37
#define  D_VLV4_OLS    10
#define  D_VLV4_SLS    11
#define  VLV4_UNKNOWN 340
#define  VLV4_OPEN    341
#define  VLV4_CLOSE   342

#define  D_VLV5_NORM   38
#define  D_VLV5_REV    39
#define  D_VLV5_OLS    12
#define  D_VLV5_SLS    13
#define  VLV5_UNKNOWN 350
#define  VLV5_OPEN    351
#define  VLV5_CLOSE   352
#define  D_VLV6_NORM   40
#define  D_VLV6_REV    41
#define  D_VLV6_OLS    14
#define  D_VLV6_SLS    15
#define  VLV6_UNKNOWN 360
#define  VLV6_OPEN    361
#define  VLV6_CLOSE   362

#define  D_VLV7_NORM   42
#define  D_VLV7_REV    43
#define  D_VLV7_OLS    16
#define  D_VLV7_SLS    17
#define  VLV7_UNKNOWN 370
#define  VLV7_OPEN    371
#define  VLV7_CLOSE   372

#define  RMODE0     10000
#define  RMODE1     10001
#define  RMODE2     10002
#define  RMODE3     10003
#define  RMODE4     10004
#define  RMODE5     10005
#define  RMODE6     10006
#define  RMODE7     10007



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
const char CO2UNIT1[] PROGMEM= "ppm";
const char CO2UNIT2[] PROGMEM= "(x10) ppm";
const char CO2UNIT3[] PROGMEM= "ppm";
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
signed long modeRUN,PmodeRUN; // 現在の動作モードと直近の動作モード

const char StrRUNMODE[]  PROGMEM="運転種別";
const char RUNAUTO[]     PROGMEM="自動";
const char RUNMANU[]     PROGMEM="プリセット";
const char RUNFREE[]     PROGMEM="手動";
const char *StrRUN[3] = {
  RUNAUTO,  //  0 - 自動
  RUNMANU,  //  1 - プリセット
  RUNFREE   //  2 - 手動
};
signed long runMODE,PrunMODE;

// 昼夜の時刻設定
const char DATEMODE[] PROGMEM = "昼夜設定";
const char DATEAUTO[] PROGMEM = "自動判定";
const char DATEMANU[] PROGMEM = "任意指定";
const char *StrDATEMODE[2] = {
			      DATEAUTO,
			      DATEMANU
};
const char DATEMODEdesc1[] PROGMEM = "運転種別が自動の時限定<br>6:00〜18:00が昼間";
signed long vDateMode;

const char DAYSTART[] PROGMEM = "昼開始時刻";
const char NIGHTSTART[] PROGMEM = "夜間開始時刻";
signed long daystart_hour;
signed long nightstart_hour;
const char HOURUNIT[] PROGMEM = "時";

const char DAYSTATUS[] PROGMEM = "昼夜状態";
const char DAYSTATE[] PROGMEM = "昼間";
const char NIGHTSTATE[] PROGMEM = "夜間";
const char *StrDAYSTATUS[2] = {
			       DAYSTATE,
			       NIGHTSTATE
};
signed long ShowDayStatus;

boolean DayTime;

/*   T-6 Debug用   */
const char T6TESTFLG[] PROGMEM = "T-6温度設定";
const char T6TESTNORMAL[] PROGMEM = "通常・受動";
const char T6TESTFORCE[] PROGMEM  = "試験・強制";
const char *StrT6TEST[2] = {
			    T6TESTNORMAL,
			    T6TESTFORCE
};
const char T6TESTVAL[] PROGMEM = "T-6強制設定値";

signed long vT6TestFlg,vT6tValue;

