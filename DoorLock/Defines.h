#define UNLOCK_SIGNAL  0x2
#define OPENED_STATUS  0x3
#define LED_BUTTON     0x4
#define BTN_INTERNAL   0x5
#define BEEEPER        0x6

#define BIT_00 0x01
#define BIT_01 0x02
#define BIT_02 0x04
#define BIT_03 0x08
#define BIT_04 0x10
#define BIT_05 0x20
#define BIT_06 0x40
#define BIT_07 0x80

#define ADDR_TAG_EXISTS 0x00
#define ADDR_TAGS_BEGIN 0x0A

#define TAG_EEPROM_SIZE 0x0A

#define STATE_LOCKED            0x0
#define STATE_UNLOCKING         0x1
#define STATE_OPENED            0x2
#define STATE_LISTEN_FOR_MASTER 0x3
#define STATE_LISTEN_FOR_EMPTY  0x4

