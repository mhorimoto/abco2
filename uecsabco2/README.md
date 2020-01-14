# abco2/uecsabco2
# ABCO2 program on UECS library

## Require Library

* AT24Cx [https://github.com/cyberp/AT24Cx]
* Ethernet2 from Library Manager
* LiquidCrystal_I2C (https://github.com/johnrickman/LiquidCrystal_I2C)
* Rtc_by_Makuna (https://github.com/Makuna/Rtc/)
* Adafruit_MCP9600_Library from Library Manager
* Adafruit_BusIO from Library Manager
* K30_CO2_I2C_Arduino [https://github.com/FirstCypress/K30_CO2_I2C_Arduino]


## RUN MODE

現状では実行モード(RUN MODE)は以下の通り。  

----------------------------------------------------------------------
| MODE |   Name   | V1 | V2 | V3 | V4 | V5 | V6 | V7 | BLOWER | PUMP |
|:----:|:--------:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:------:|:----:|
|  0   | 全閉     | X  | X  | X  | X  | X  | X  | X  |   X    |  X   |
|  1   | CO2貯蔵  | X  | O  | X  | O  | X  | O  | X  |   O    |  O   |
|  2   | 放散(1)  | X  | X  | O  | X  | O  | X  | O  |   O    |  X   |
|  3   | 放散(2)  | O  | X  | X  | X  | O  | X  | O  |   O    |  X   |
|  4   | 外気導入 | O  | X  | X  | X  | X  | O  | O  |   O    |  X   |
|  5   | 冷却     | O  | X  | X  | O  | O  | X  | X  |   O    |  X   |
|  6   | 全開     | O  | O  | O  | O  | O  | O  | O  |   X    |  X   |
|  7   | 緊急停止 | X  | X  | X  | O  | O  | O  | X  |   X    |  X   |  

----------------------------------------------------------------------
* 放散(1): チャンバーair導入
* 放散(2): 外部air導入

## UTILITY MODE

### OPUTPUT TEST

### INPUT TEST

### TEMPERATURE
 T型熱電対による温度計測の結果をLCDに表示する。
 
### CO2 K33 TEST
 2個のK33によるCO2濃度計測の結果をLCDに表示する。

### RTC TEST
 RTC Real Time Clockの結果をLCDに表示する。

### NETWORK UTIL
 IPアドレスなどをLCDに表示する。
 
### SYSTEM RESET
