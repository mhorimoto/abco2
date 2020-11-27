# abco2

## 運転種別

### 自動

自動の時には、以下の振る舞いをする。

- 昼間：MODE-2
- 夜間でバーナーがONならば：MODE-1
- 夜間でバーナーがOFFならば：MODE-0

## How to compile and install

    # cd ~/work
    # arduino-cli compile -b arduino:avr:mega abco2
    # arduino-cli upload -p /dev/ttyUSB0 -b arduino:avr:mega abco2

