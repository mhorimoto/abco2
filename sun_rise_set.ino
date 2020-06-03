// -*- C++ -*- //////////////////////////////////////////////////////////
// 日の出、日の入り時刻を計算する
//

float t1;                                 // 日の出時刻（単位：時）
float t2;                                 // 日の入時刻（単位：時）
float tm;                                 // 南中時刻（単位：時）
float tL;                                 // 昼の長さ（単位：時）

float Longitude = 130.214750;             // 経度（東経を入力）
float Latitude  = 33.592735;              // 緯度（北緯を入力）

#define  M_PI   3.14159265358979323846    // πの値
#define DEG(a)  ((a) * 180 / M_PI)        // ラジアンを度に変換するマクロ
#define RAD(a)  ((a) * M_PI /180)         // 度をラジアンに変換するマクロ

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
