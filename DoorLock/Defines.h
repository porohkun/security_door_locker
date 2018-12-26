#define BTN_INTERNAL 0x2
#define BTN_EXTERNAL 0x3
#define BTN_NFC      0x4
#define BTN_LOCKED   0x5
#define REED_SWITCH  0x6

#define BTN_02 0x2
#define BTN_03 0x3
#define BTN_04 0x4
#define BTN_05 0x5
#define BTN_06 0x6

#define LED_01 0xE
#define LED_02 0xF
#define LED_04 0x10
#define LED_08 0x11
#define LED_16 0x12

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

#define STATE_LOCKED 0x0
#define STATE_LISTEN_FOR_UNLOCK 0x1
#define STATE_UNLOCKING 0x2
#define STATE_UNLOCKED 0x3
#define STATE_OPENED 0x4
#define STATE_CLOSED 0x5
#define STATE_LOCKING 0x6
#define STATE_LISTEN_FOR_MASTER 0x7
#define STATE_LISTEN_FOR_EMPTY 0x8
#define STATE_SAVE_NEW_TAG 0x9

void lockedState();
void lockedStateLoop();
void lockedStateExit();

void listenForUnlockState();
void listenForUnlockStateLoop();
void listenForUnlockStateExit();
void dump_byte_array(byte *buffer, byte bufferSize);
bool getID();
void ShowReaderDetails();

void unlockingState();
void unlockingStateLoop();
void unlockingStateExit();

void unlockedState();
void unlockedStateLoop();
void unlockedStateExit();

void openedState();
void openedStateLoop();
void openedStateExit();

void closedState();
void closedStateLoop();
void closedStateExit();

void lockingState();
void lockingStateLoop();
void lockingStateExit();

void listenForMasterState();
void listenForMasterStateLoop();
void listenForMasterStateExit();

void listenForEmptyState();
void listenForEmptyStateLoop();
void listenForEmptyStateExit();

void saveNewTagState();
void saveNewTagStateLoop();
void saveNewTagStateExit();
