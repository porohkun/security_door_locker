#define UNLOCK_SIGNAL  0x02
#define OPENED_STATUS  0x03
#define LED_BUTTON     0x04
#define BTN_INTERNAL   0x05
#define BTN_EXTERNAL   0x06
#define BEEEPER        0x07
#define BTN_CLR_EEPROM 0x08

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
#define ADDR_SEED_BEGIN 0x5A

#define TAG_EEPROM_SIZE 0x0A
#define SEED_EEPROM_SIZE 0x1
#define SEED_SIZE 0x4

#define KEY_SEED_BLOCK 0x3D
#define KEY_SEED_POS 0x06

#define STATE_LOCKED            0x00
#define STATE_UNLOCKING         0x01
#define STATE_OPENED            0x02
#define STATE_LISTEN_FOR_MASTER 0x03
#define STATE_LISTEN_FOR_EMPTY  0x04

