/* 

  Calculation time for Sun-set and Sun-rise by Arduino

  Author: M.Horimoto <horimoto@holly-linux.com>
  Version:
  Date:

*/

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

#define  M_PI   3.14159265358979323846    // πの値
#define DEG(a)  ((a) * 180 / M_PI)        // ラジアンを度に変換するマクロ
#define RAD(a)  ((a) * M_PI /180)         // 度をラジアンに変換するマクロ
 
float t1;                                 // 日の入り時刻（単位：時）
float t2;                                 // 日の入り時刻（単位：時）
float tm;                                 // 南中時刻（単位：時）
float tL;                                 // 昼の長さ（単位：時）

float Longitude = 130.214750;             // 経度（東経を入力）
float Latitude  = 33.592735;              // 緯度（北緯を入力）

void setup() {
  int todayn ;                            // 今日の日付連番
  Serial.begin(115200);
  todayn = calc_0_days(2019,10,30) - calc_0_days(2019,1,1) + 1;
  Serial.print("TODAY=");
  Serial.println(todayn);
  lcd.init();
  lcd.backlight();
}
 
void loop() {
  int n;
  for (n = 0; n < 1; n++) {              // 日付連番 0から364日の範囲を計算
    t1 = SunRiseTime(Longitude, Latitude, n);  // 経緯度、日付連番から日の出時刻を求める
    t2 = SunSetTime(Longitude, Latitude, n);   // 経緯度、日付連番から日の入り出時刻を求める
    tm = (t1 + t2) / 2;                        // 太陽南中時刻
    tL = (t2 - t1);                            // 昼の時間
    Serial.print(n + 1), Serial.print(", ");
    Serial.print(t1, 4), Serial.print(", ");
    Serial.print(t2, 4), Serial.print(", ");
    Serial.print(tm, 4), Serial.print(", ");
    Serial.print(tL, 4), Serial.println();
  }
  lcd.setCursor(1,0);
  lcd.print(n);
  lcd.setCursor(4,0);
  lcd.print(t1,4);
  for (;;) {}                                  // ここで停止
}
 
float SunRiseTime(float x, float y, int n) {   // 日の出時刻を求める関数
  float d, e, t;
  y = RAD(y);                                  // 緯度をラジアンに変換
  d = dCalc(n);                                // 太陽赤緯を求める
  e = eCalc(n);                                // 均時差を求める
  // 太陽の時角幅を求める（視半径、大気差などを補正 (-0.899度)　）
  t = DEG(acos( (sin(RAD(-0.899)) - sin(d) * sin(y)) / (cos(d) * cos(y)) ) );
  return ( -t + 180.0 - x + 135.0) / 15.0 - e; // 日の出時刻を返す
}
 
float SunSetTime(float x, float y, int n) {    // 日の入り時刻を求める関数
  float d, e, t;
  y = RAD(y);                                  // 緯度をラジアンに変換
  d = dCalc(n);                                // 太陽赤緯を求める
  e = eCalc(n);                                // 均時差を求める
  // 太陽の時角幅を求める（視半径、大気差などを補正 (-0.899度)　）
  t = DEG(acos( (sin(RAD(-0.899)) - sin(d) * sin(y)) / (cos(d) * cos(y)) ) );
  return ( t + 180.0 - x + 135.0) / 15.0 - e;  // 日の入り時刻を返す
}
 
float dCalc(int n) {                           // 近似式で太陽赤緯を求める
  float d, w;
  w = (n + 0.5) * 2 * M_PI / 365;              // 日付をラジアンに変換
  d = + 0.33281
      - 22.984 * cos(w) - 0.34990 * cos(2 * w) - 0.13980 * cos(3 * w)
      + 3.7872 * sin(w) + 0.03250 * sin(2 * w) + 0.07187 * sin(3 * w);
  return RAD(d);                               // 赤緯を返す（単位はラジアン）
}
 
float eCalc(int n) {                           // 近似式で均時差を求める
  float e, w;
  w = (n + 0.5) * 2 * M_PI / 365;              // 日付をラジアンに換算
  e = + 0.0072 * cos(w) - 0.0528 * cos(2 * w) - 0.0012 * cos(3 * w)
      - 0.1229 * sin(w) - 0.1565 * sin(2 * w) - 0.0041 * sin(3 * w);
  return e;                                    // 均一時差を返す（単位は時）
}

//----------------------------------------------------------------
// 西暦０年１月１日からの日数を返す
//----------------------------------------------------------------
unsigned long calc_0_days(int year, byte month, byte day) {
  unsigned long days;
  int daysinmonth_ruiseki[12] = {0,31,59,90,120,151,181,212,243,273,304,334};

  year--;                               // 当年は含まず 
  days = (unsigned long)year * 365;
  days += year / 4;                     // 閏年の日数を足しこむ
  days -= year/100;                     // 閏年で無い日数を差し引く
  days += year/400;                     // 差し引きすぎた日数を足しこむ
  days += (unsigned long)daysinmonth_ruiseki[month-1];
  if( is_leapyear( year ) &&  3 <= month ) {
    day++;
  }
  days += (unsigned long)day;

  return days;
}

//----------------------------------------------------------------
// 閏年か否かを返す
//----------------------------------------------------------------
boolean is_leapyear( int year ) {
  if( (year % 400) == 0 || ((year % 4) == 0 && (year % 100) != 0)) {
    return true;
  } else {
    return false;
  }
}
