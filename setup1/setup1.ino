//
// All Interface LOW
//

/*
D2	I 緊急停止ボタン(赤)	Interrupt available
D3	I セレクタ決定ボタン(緑)	Interrupt available
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
TX1	I セレクタキャンセルボタン(黄)	Interrupt available
RX1		                        Interrupt available
SDA	B LCD,T熱電対,K33LP,K33ICB	"I2CインタフェースInterrupt available"
SCL	B	
D22	O RELAY1-ブロアモーター開閉器	
D23	O RELAY2-ポンプFP-15S	
D24	O RELAY3-予備	
D25	I UECS DHCP MODE SWITCH	HIGHでDHCPモード
D26	I UECS SAFE MODE SWITCH	LOWでセーフモード
D27	X 予約	
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
*/




void setup() {
  int i;
  Serial.begin(115200);
  for(i=22;i<=24;i++) {
    pinMode(i,OUTPUT);
    digitalWrite(i,LOW);
    Serial.print(i);
    Serial.println(" set LOW.");
  }
  for(i=30;i<=43;i++) {
    pinMode(i,OUTPUT);
    digitalWrite(i,LOW);
    Serial.print(i);
    Serial.println(" set LOW.");
  }
  Serial.println("ALL DONE");
  Serial.end();
}

void loop() {
}
