#include "Arduino.h"

#define MESSAGE_LEVEL_DEBUG   0x01
#define MESSAGE_LEVEL_INFO    0x02
#define MESSAGE_LEVEL_WARNING 0x04
#define MESSAGE_LEVEL_ERROR   0x08
#define MESSAGE_LEVEL_SPECIAL 0x10

#define TEXT_MESSAGE_HEADER                 0x00
#define INIT_STATE_MESSAGE_HEADER           0x01
#define EXIT_STATE_MESSAGE_HEADER           0x02
#define STATE_CHANGED_MESSAGE_HEADER        0x03
#define AUTH_BLOCK_MESSAGE_HEADER           0x04
#define LOG_BLOCK_MESSAGE_HEADER            0x05
#define TAG_SAVED_MESSAGE_HEADER            0x06
#define TAG_REMOVED_MESSAGE_HEADER          0x07
#define TAG_AUTH_MESSAGE_HEADER             0x08
#define EEPROM_CLEARED_MESSAGE_HEADER       0x09
#define STARTED_MESSAGE_HEADER              0x0A
#define UNLOCKED_WITH_TAG_MESSAGE_HEADER    0x0B
#define UNLOCKED_WITHOUT_TAG_MESSAGE_HEADER 0x0C


class LoggerClass
{
public:
	void Started();

	void TextMessage(byte level, String message);
	void EEPROMClearedMessage();
	void InitStateMessage(byte state);
	void ExitStateMessage(byte state);
	void StateChangedMessage(byte fromState, byte toState);
	void TagSavedMessage(byte * uid, byte uidLen, byte index, byte result);
	void TagRemovedMessage(byte * uid, byte uidLen, byte index, byte result);
	void TagAuthMessage(byte * uid, byte uidLen, byte index, byte uidOnly, byte result);
	void AuthBlockMessage(byte * uid, byte uidLen, byte blockNumber, byte keyNumber, byte * keyData, byte keyDataLen, byte result);
	void LogBlockMessage(byte blockNumber, String message, byte * data, byte result);
	void UnlockedWithTag(byte * uid, byte uidLen, byte result);
	void UnlockedWithoutTag();

private:
	void WriteUint16(unsigned int value);
	void WriteUint8Array(byte * array, byte length);
	void WriteHeader(byte type, byte level, unsigned int length);
};

extern LoggerClass Logger;