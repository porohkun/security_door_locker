#include "Arduino.h"

#define MESSAGE_LEVEL_DEBUG   0x01
#define MESSAGE_LEVEL_INFO    0x02
#define MESSAGE_LEVEL_WARNING 0x04
#define MESSAGE_LEVEL_ERROR   0x08
#define MESSAGE_LEVEL_SPECIAL 0x10

#define TEXT_MESSAGE_HEADER          0x00
#define INIT_STATE_MESSAGE_HEADER    0x01
#define EXIT_STATE_MESSAGE_HEADER    0x02
#define STATE_CHANGED_MESSAGE_HEADER 0x03
#define AUTH_BLOCK_MESSAGE_HEADER    0x04
#define LOG_BLOCK_MESSAGE            0x05


class LoggerClass
{
public:
	void ActivateLogger();

	void TextMessage(byte level, String message);
	void InitStateMessage(byte state);
	void ExitStateMessage(byte state);
	void StateChangedMessage(byte fromState, byte toState);
	void AuthBlockMessage(byte * uid, byte uidLen, byte blockNumber, byte keyNumber, byte * keyData, byte keyDataLen, byte result);
	void LogBlockMessage(byte blockNumber, String message, byte * data, byte result);
	void LogUnlockWithTag(byte * uid);
	void LogUnlockWrongTag(byte * uid);

private:
	void WriteUint16(unsigned int value);
	void WriteUint8Array(byte * array, byte length);
	void WriteHeader(byte type, byte level, unsigned int length);
};

extern LoggerClass Logger;