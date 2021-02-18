abco2 D0058A
============

# コンパイル前の警告

UECSライブラリは標準品ではなくて、HOLLY改造版(Power By HOLLY)を使用してください。
DHCPによるIPアドレス取得とHTML記述で最近のブラウザが言語混乱をしないようにja明記をしています。
なおHOLLY版ライブラリは

[HOLLY_UARDECS_MEGA_Library](https://horimoto@bitbucket.org/horimoto/holly_uardecs_mega_library.git)

こちらにあります。  
このライブラリを Arduino のライブラリが納められるディレクトリに入れておいてください。

現在、開発版で推奨されているBranchは D0058 です。master ではありませんので注意してください。
このD0058をベースに独自のブランチで変更を加えてください。

## 運転種別

### 自動

自動の時には、以下の振る舞いをする。

- 昼間：MODE-2
- 夜間でバーナーがONならば：MODE-1
- 夜間でバーナーがOFFならば：MODE-0

- T-6の温度が60℃を超えたら、冷却モード(5)に入る。
- 5分冷却モードで運転し全開モード(6)に移行する。

### プリセット

プリセットの時には「モード選択」でMODE-x を選択する。  
モードは2020/11/17に再設定されている。

- MODE-0 : 全閉
- MODE-1 : CO2貯蔵
- MODE-2 : 放散1 : チャンバーAIR導入
- MODE-3 : 放散2 : 外部AIR導入
- MODE-4 : 外気導入
- MODE-5 : 冷却
- MODE-6 : 全開

### 手動

バルブやブロア、ポンプの状態は全て手動で行う。

## How to compile and install

### Arduino IDEで実施する場合

　普通にコンパイルしてアップロードしてください。

### arduino-cliで実施する場合

    # cd abco2作業ディレクトリの１つ上
    # arduino-cli compile -b arduino:avr:mega abco2
    # arduino-cli upload -p /dev/ttyUSB0 -b arduino:avr:mega abco2

## Need external libraries

* [Ethernet2](https://github.com/adafruit/Ethernet2)
* [HOLLY_UARDECS_MEGA_Library](https://horimoto@bitbucket.org/horimoto/holly_uardecs_mega_library.git)
* [LiquidCrystal_I2C](https://github.com/marcoschwartz/LiquidCrystal_I2C)
* [K30_I2C](https://github.com/FirstCypress/K30_CO2_I2C_Arduino)
* [RtcDS3231](https://github.com/Makuna/Rtc/wiki)
* [AT24CX](https://oberguru.net/elektronik/eeprom/at24cx-at24c32-at24c64-at24c128-at24c256-at24c512.html)
* [Adafruit_I2C*](https://github.com/adafruit/Adafruit_BusIO)
* [Adafruit_MCP9600](https://github.com/adafruit/Adafruit_MCP9600)

