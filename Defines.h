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

#define LED_01 0x7
#define LED_02 0x8
#define LED_04 0x9
#define LED_08 0xB
#define LED_16 0xC

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

void listenForUnlockState();
void listenForUnlockStateLoop();

void unlockingState();
void unlockingStateLoop();

void unlockedState();
void unlockedStateLoop();

void openedState();
void openedStateLoop();

void closedState();
void closedStateLoop();

void lockingState();
void lockingStateLoop();

void listenForMasterState();
void listenForMasterStateLoop();

void listenForEmptyState();
void listenForEmptyStateLoop();

void saveNewTagState();
void saveNewTagStateLoop();
