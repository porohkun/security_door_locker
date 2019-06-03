#include "Arduino.h"
#include "Logger.h"

void LoggerClass::ActivateLogger()
{
	TextMessage(MESSAGE_LEVEL_DEBUG, "!!! Activate Logger");
}

void LoggerClass::TextMessage(byte level, String message)
{
	WriteHeader(TEXT_MESSAGE_HEADER, level, message.length());

	Serial.print(message);

	Serial.flush();
}

void LoggerClass::InitStateMessage(byte state)
{
	WriteHeader(INIT_STATE_MESSAGE_HEADER, MESSAGE_LEVEL_SPECIAL, 1);

	Serial.write(state);

	Serial.flush();
}

void LoggerClass::ExitStateMessage(byte state)
{
	WriteHeader(EXIT_STATE_MESSAGE_HEADER, MESSAGE_LEVEL_SPECIAL, 1);

	Serial.write(state);

	Serial.flush();
}

void LoggerClass::StateChangedMessage(byte fromState, byte toState)
{
	WriteHeader(STATE_CHANGED_MESSAGE_HEADER, MESSAGE_LEVEL_SPECIAL, 2);

	Serial.write(fromState);
	Serial.write(toState);

	Serial.flush();
}

void LoggerClass::AuthBlockMessage(byte * uid, byte uidLen, byte blockNumber, byte keyNumber, byte * keyData, byte keyDataLen, byte result)
{
	WriteHeader(AUTH_BLOCK_MESSAGE_HEADER, MESSAGE_LEVEL_SPECIAL, uidLen + keyDataLen + 5);

	Serial.write(uidLen); //write card uid length
	WriteUint8Array(uid, uidLen); //write card uid
	Serial.write(blockNumber);
	Serial.write(keyNumber);
	Serial.write(keyDataLen); //write key data length
	WriteUint8Array(keyData, keyDataLen); //write key data
	Serial.write(result);

	Serial.flush();
}

void LoggerClass::LogBlockMessage(byte blockNumber, String message, byte * data, byte result)
{
	int messageLen = message.length();
	WriteHeader(LOG_BLOCK_MESSAGE, MESSAGE_LEVEL_DEBUG, messageLen + 16 + 4);

	Serial.write(blockNumber);
	Serial.write(messageLen);
	Serial.print(message);
	Serial.write(16);
	WriteUint8Array(data, 16);
	Serial.write(result);

	Serial.flush();
}




void LoggerClass::LogUnlockWithTag(byte * uid)
{
	TextMessage(MESSAGE_LEVEL_WARNING, "!!! Unlock With Tag");
}

void LoggerClass::LogUnlockWrongTag(byte * uid)
{
	TextMessage(MESSAGE_LEVEL_WARNING, "!!! Unlock Wrong Tag");
}




void LoggerClass::WriteUint16(unsigned int value)
{
	Serial.write(value >> 8);
	Serial.write((byte)value);
}

void LoggerClass::WriteUint8Array(byte * array, byte length)
{
	for (byte i = 0; i < length; i++)
		Serial.write(array[i]);
}

void LoggerClass::WriteHeader(byte type, byte level, unsigned int length)
{
	Serial.write(type);
	Serial.write(level);
	WriteUint16(length);
}

LoggerClass Logger;