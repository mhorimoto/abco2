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
