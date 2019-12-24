//
// All Interface LOW
//

/*
D2	I 緊急停止ボタン(赤)	Interrupt available INT.0
D3	I セレクタ決定ボタン(緑)	Interrupt available INT.1
D4	I VLV1 OLS	ボールバルブ1全開
D5	I VLV1 SLS	ボールバルブ1全閉
D6	I VLV2 OLS	ボールバルブ2全開
D7	I VLV2 SLS	ボールバルブ2全閉
D8	I VLV3 OLS	ボールバルブ3全開
D9	I VLV3 SLS	ボールバルブ3全閉
D10	I VLV4 OLS	ボールバルブ4全開
D11	I VLV4 SLS	ボールバルブ4全閉
D12	I VLV5 OLS	ボールバルブ5全開
D13	I VLV5 SLS	ボールバルブ5全閉
TX3	I VLV6 OLS	ボールバルブ6全開
RX3	I VLV6 SLS	ボールバルブ6全閉
TX2	I VLV7 OLS	ボールバルブ7全開
RX2	I VLV7 SLS	ボールバルブ7全閉
TX1	I セレクタキャンセルボタン(黄)	Interrupt available INT.5
RX1	I アラーム未着火など             Interrupt available INT.4
SDA	B LCD,T熱電対,K33LP,K33ICB	I2CインタフェースInterrupt available INT.3
SCL	B LCD,T熱電対,K33LP,K33ICB	I2CインタフェースInterrupt available INT.2
D22	O RELAY1-ブロアモーター開閉器	
D23	O RELAY2-ポンプFP-15S	
D24	O RELAY3-予備	
D25	I UECS DHCP MODE SWITCH	HIGHでDHCPモード
D26	I UECS SAFE MODE SWITCH	LOWでセーフモード
D27	I バーナーON/OFF
D28	X 予約	
D29	X 予約	
D30	O ボールバルブ1-A	
D31	O ボールバルブ1-B	
D32	O ボールバルブ2-A	
D33	O ボールバルブ2-B	
D34	O ボールバルブ3-A	
D35	O ボールバルブ3-B	
D36	O ボールバルブ4-A	
D37	O ボールバルブ4-B	
D38	O ボールバルブ5-A	
D39	O ボールバルブ5-B	
D40	O ボールバルブ6-A	
D41	O ボールバルブ6-B	
D42	O ボールバルブ7-A	
D43	O ボールバルブ7-B	
D44	X 予約	
D45	X 予約	
D46	I 液面レベル1	抵抗分圧でL/Hを検出
D47	I 液面レベル2	抵抗分圧でL/Hを検出
D48	I 圧力センサ出力１	抵抗分圧でL/Hを検出
D49	I 圧力センサ出力２(当面未使用)	抵抗分圧でL/Hを検出
D50	X W5500-MISO	LANインタフェース
D51	X W5500-MOSI	
D52	X W5500-SCK	
D53	X W5500-SS	
RESET	X W5500-RESET

A00		
A01		
A02	セレクタ入力	10kΩ VR
A03		
A04		
A05		
A06		
A07		
A08		
A09		
A10		
A11		
A12		
A13		
A14		
A15			
*/

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <RtcDS3231.h>
#include <Ethernet2.h>
#include <EEPROM.h>
#include "setup1.h"

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
RtcDS3231<TwoWire> Rtc(Wire);

int a02val = 0;
int nmval  = 0;
char lcdtext[17];
char lcdtitle[17];
byte MACAddress[6];

volatile int prev_menu = 0;
volatile int menu_state = 0; // 0: MAIN MENU
                             // 1: OUTPUT TEST
                             // 2: INPUT TEST
                             // 3: TEMPERATURE
                             // 4: CO2 TEST
                             // 5: RTC TEST
volatile int output_test_toggle; // 0:OFF, 1:ON

const char *VERSION = "00D";

void setup() {
  int i;
  // Setup INPUT Pins
  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  pinMode(18,INPUT_PULLUP);
  // Setup OUTPUT Pins
  for(i=22;i<=24;i++) {
    pinMode(i,OUTPUT);
    digitalWrite(i,LOW);
  }
  for(i=30;i<=43;i++) {
    pinMode(i,OUTPUT);
    digitalWrite(i,LOW);
  }
  // Ethernet Initilize
  Get_MACAddress();
  //  Serial.end();
  attachInterrupt(0, emgstop, FALLING);
  attachInterrupt(1, okgo, FALLING);
  attachInterrupt(5, backret, FALLING);
  sprintf(lcdtitle,"MAIN MENU %6s",VERSION);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print(lcdtitle);
}


void loop() {
  char *tn;
  int  d18;

  a02val = analogRead(2);
  nmval = (a02val/100)+1;
  d18 = digitalRead(18);
  if (d18==0) {
    menu_state = 0;
    prev_menu = 9;
  }
  switch(menu_state) {
  case 0: // MAIN MENU
    if (prev_menu!=menu_state) {
      lcd.setCursor(0,0);
      lcd.print(lcdtitle);
    }
    switch(nmval) {
    case 1:
      tn = "OUTPUT TEST";
      break;
    case 2:
      tn = "INPUT TEST ";
      break;
    case 3:
      tn = "TEMPERATURE";
      break;
    case 4:
      tn = "CO2 TEST   ";
      break;
    case 5:
      tn = "RTC UTIL   ";
      break;
    case 6:
      tn = "NET UTIL   ";
      break;
    default:
      tn = "NO DEFINED ";
    }
    sprintf(lcdtext,"%4d %s",a02val,tn);
    lcd.setCursor(0,1);
    lcd.print(lcdtext);
    break;
  case 1: // OUTPUT TEST
    if (prev_menu!=menu_state) {
      lcd.setCursor(0,0);
      lcd.print("OUTPUT MENU     ");
    }
    test_output();
    prev_menu = 1;
    break;
  case 2:
    break;
  case 3:
    break;
  case 4:
    break;
  case 5: // RTC UTIL
    if (prev_menu!=menu_state) {
      lcd.setCursor(0,0);
      lcd.print("RTC UTIL        ");
    }
    prev_menu = 5;
    rtc_util();
    break;
  case 6: // NETWORK UTIL
    if (prev_menu!=menu_state) {
      lcd.setCursor(0,0);
      lcd.print("NET UTIL        ");
    }
    prev_menu = 6;
    net_util();
    break;
  }
}


// void test_output() {
//   int d;
//   int i,aval,smenu;
//   byte maca[6];
//   char lcdtext[17];
//   aval = analogRead(2);
//   smenu = (aval/100)+1;
//   switch(smenu) {
//   case 1: // BLOWER TEST
//     sprintf(lcdtext,"%16s","BLOWER TEST");
//     lcd.setCursor(0,0);
//     lcd.print(lcdtext);
//     d = digitalRead(D_BLOWER);
//     if (d==HIGH) {
//       sprintf(lcdtext,"%16s","BLOWER IS ON");
//       if (output_test_toggle==1) {
// 	digitalWrite(D_BLOWER,LOW);
// 	output_test_toggle = 0;
//       }
//     } else {
//       sprintf(lcdtext,"%16s","BLOWER IS OFF");
//       if (output_test_toggle==1) {
// 	digitalWrite(D_BLOWER,HIGH);
// 	output_test_toggle = 0;
//       }
//     }
//     lcd.setCursor(0,1);
//     lcd.print(lcdtext);
//   }
// }

void emgstop(void) {
  Serial.begin(115200);
  Serial.println("EMG STOP");
  Serial.end();
}

void okgo(void) {
  Serial.begin(115200);
  Serial.println("OKGO");
  Serial.end();
  switch(menu_state) {
  case 0:
    menu_state = nmval;
    break;
  case 1: // IN OUTPUT_TEST
    output_test_toggle = 1;
    break;
  }
}

void backret(void) {
  Serial.begin(115200);
  Serial.println("BACK RETURN");
  Serial.end();
  menu_state = 0;
  prev_menu = 9;
}

void rtc_util(void) {
  char timeDate[17];
  int  sec;
  RtcDateTime now = Rtc.GetDateTime();
  sec = now.Second();
  switch(sec) {
  case 0:
  case 1:
  case 2:
  case 15:
  case 16:
  case 17:
  case 30:
  case 31:
  case 32:
  case 45:
  case 46:
  case 47:
    sprintf(timeDate,"%4d/%02d/%02d",now.Year(),now.Month(),now.Day());
    break;
  default:
    sprintf(timeDate,"%02d:%02d:%02d  ",now.Hour(),now.Minute(),now.Second());
    break;
  }
  lcd.setCursor(0,1);
  lcd.print(timeDate);
}

void net_util(void) {
  int i,aval,smenu;
  byte maca[6];
  char lcdtext[17];
  aval = analogRead(2);
  smenu = (aval/100)+1;
  switch(smenu) {
  case 1: // IP Address
    lcd.setCursor(0,0);
    lcd.print("IP Address      ");
    lcd.setCursor(0,1);
    lcd.print("192.168.31.000  ");
    break;
  case 2: // Netmask
    lcd.setCursor(0,0);
    lcd.print("Netmask         ");
    lcd.setCursor(0,1);
    lcd.print("255.255.255.0   ");
    break;
  case 3: // Gateway
    lcd.setCursor(0,0);
    lcd.print("Gateway         ");
    lcd.setCursor(0,1);
    lcd.print("192.168.31.254  ");
    break;
  case 4: // MAC Address
    Reset_lcdtext();
    sprintf(lcdtext,"%02X%02X.%02X%02X.%02X%02X",
	    MACAddress[0],MACAddress[1],MACAddress[2],
	    MACAddress[3],MACAddress[4],MACAddress[5]);
    lcd.setCursor(0,0);
    lcd.print("MAC Address     ");
    lcd.setCursor(0,1);
    lcd.print(lcdtext);
    break;
  }
}

void Reset_lcdtext(void) {
  int i;
  for(i=0;i<17;i++) {
    lcdtext[i] = (char)NULL;
  }
}

void Get_MACAddress(void) {
  int i;
  for(i=0;i<6;i++) {
    MACAddress[i]=EEPROM.read(4090+i);
  }
}
