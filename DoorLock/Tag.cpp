#include "Arduino.h"
#include "Defines.h"
#include "Tag.h"
#include <Wire.h>
#include <SPI.h>
#include "Adafruit_PN532.h"
#include <EEPROM.h>
#include "Flags.h"
#include "Rnd.h"
#include "Logger.h"

void(*resetFunc) (void) = 0;

void TagClass::ActivateListener()
{
	_pn532.begin();
	_pn532.SAMConfig();

	uint32_t versiondata = _pn532.getFirmwareVersion();
	if (!versiondata) {
		Logger.TextMessage(MESSAGE_LEVEL_DEBUG, "Didn't find PN53x board");
		//while (true); // halt
	}
	Logger.TextMessage(MESSAGE_LEVEL_DEBUG, "Found chip PN5");
	//Serial.println((versiondata >> 24) & 0xFF, HEX);
	//Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
	//Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);
}

void TagClass::DeactivateListener()
{
	Stop();
	SPI.end();
}


bool TagClass::HaveTag()
{
	return _pn532.readPassiveTargetID(PN532_MIFARE_ISO14443A, _uid, &_uidLength, 30);
}

void TagClass::FillBuffer(byte *key1, byte *accBits, byte *key2)
{
	for (byte i = 0; i < 6; i++)
		_buffer[i] = key1[i];
	for (byte i = 0; i < 4; i++)
		_buffer[i + 6] = accBits[i];
	for (byte i = 0; i < 6; i++)
		_buffer[i + 10] = key2[i];
}

//void TagClass::PrintBuffer()
//{
//	Serial.print("    Buffer:  ");
//	PrintBufferInternal();
//}
//
//void TagClass::PrintBuffer(byte block_)
//{
//	if (block_ < 10)
//		Serial.print("         ");
//	else
//		Serial.print("        ");
//	Serial.print(block_);
//	Serial.print(":  ");
//	PrintBufferInternal();
//}
//
//void TagClass::PrintBufferInternal()
//{
//	for (byte index = 0; index < 16; index++) {
//		if (_buffer[index] < 0x10)
//			Serial.print(F(" 0"));
//		else
//			Serial.print(F(" "));
//		Serial.print(_buffer[index], HEX);
//		if ((index % 4) == 3) {
//			Serial.print(F(" "));
//		}
//	}
//	Serial.println();
//}

bool TagClass::AuthKeyA(byte block_, bool silent)
{
	return AuthKey(MIFARE_Key_A, block_, silent);
}

bool TagClass::AuthKeyB(byte block_, bool silent)
{
	return AuthKey(MIFARE_Key_B, block_, silent);
}

bool TagClass::AuthKey(byte keyid, byte block_, bool silent)
{
	//if (!silent)
	//{
	//	switch (keyid)
	//	{
	//	case MIFARE_Key_A:
	//		Serial.print("> Auth key A: ");
	//		break;
	//	case MIFARE_Key_B:
	//		Serial.print("> Auth key B: ");
	//		break;
	//	default:
	//		return false;
	//	}
	//	Serial.print(block_);
	//	Serial.print(": ");
	//	for (byte i = 0; i < 6; i++) {
	//		if (_key[i] < 0x10)
	//			Serial.print(F(" 0"));
	//		else
	//			Serial.print(F(" "));
	//		Serial.print(_key[i], HEX);
	//	}
	//}

	_status = _pn532.mifareclassic_AuthenticateBlock(_uid, _uidLength, block_, keyid, _key);
	if (!silent)
		Logger.AuthBlockMessage(_uid, _uidLength, block_, keyid, _key, 6, _status);
	/*if (!_status) {
		Serial.print("  failed.");
		return false;
	}
	if (!silent)
		Serial.println("  done.");*/
	return _status;
}

bool TagClass::ReadBlockToBuffer(byte block_)
{
	//Serial.print("> Reading block ");
	//Serial.print(block_);

	_status = _pn532.mifareclassic_ReadDataBlock(block_, _buffer);
	Logger.LogBlockMessage(block_, "read", _buffer, _status);
	//if (!_status) {
	//	Serial.print("  failed.");
	//	return false;
	//}
	//Serial.println("  done.");
	//return true;
	return _status;
}

bool TagClass::WriteBlockFromBuffer(byte block_)
{
	//Serial.print("> Writing block ");
	//Serial.print(block_);

	_status = _pn532.mifareclassic_WriteDataBlock(block_, _buffer);
	Logger.LogBlockMessage(block_, "write", _buffer, _status);
	//if (!_status) {
	//	Serial.print("  failed.");
	//	return false;
	//}
	//Serial.println("  done.");
	//return true;
	return _status;
}

void TagClass::Stop()
{
	Logger.TextMessage(MESSAGE_LEVEL_DEBUG, "> Listener stopped.");
}


bool TagClass::HaveMasterTag()
{
	bool have = HaveFlag(EEPROM.read(ADDR_TAG_EXISTS), BIT_00);
	Logger.TextMessage(MESSAGE_LEVEL_SPECIAL, (have ? "> Have master tag: true." : "> Have master tag: false."));
	return have;
}

bool TagClass::SaveCurrentTagAsMaster()
{
	if (SaveCurrentTagAtIndex(0))
	{
		Logger.TextMessage(MESSAGE_LEVEL_SPECIAL, "> Saved current tag as master.");
		return true;
	}
	return false;
}

bool TagClass::SaveCurrentTag()
{
	Logger.TextMessage(MESSAGE_LEVEL_DEBUG, "> Saving current tag ... ");
	byte tags = EEPROM.read(ADDR_TAG_EXISTS);
	for (byte i = 1; i < 8; i++)
		if (!HaveFlag(tags, 1 << i))
		{
			Logger.TextMessage(MESSAGE_LEVEL_DEBUG, "at index " + i);
			return SaveCurrentTagAtIndex(i);
		}
	Logger.TextMessage(MESSAGE_LEVEL_DEBUG, "failed.");
	return false;
}

bool TagClass::SaveCurrentTagAtIndex(byte index)
{
	byte seedPos = 0;
	unsigned long seed = 0;
	if (WriteSeedSector(&seedPos, &seed))
	{
		if (!FillRandomSectorByRandom(seed))
			return false;

		EEPROM.write(ADDR_SEED_BEGIN + SEED_EEPROM_SIZE * index, seedPos);
		for (byte i = 0; i < 10; i++)
			EEPROM.write(ADDR_TAGS_BEGIN + TAG_EEPROM_SIZE * index + i, (i < _uidLength ? _uid[i] : 0));
		if (index == 0)
			EEPROM.write(ADDR_TAG_EXISTS, BIT_00);
		else
			EEPROM.write(ADDR_TAG_EXISTS, AddFlag(EEPROM.read(ADDR_TAG_EXISTS), 1 << index));
		Logger.TextMessage(MESSAGE_LEVEL_DEBUG, "> Saved current tag at index " + index);
		return true;
	}
	return false;
}

bool TagClass::CurrentTagIsMaster(bool uidOnly)
{
	bool master = CurrentTagIsKnownAsIndex(0, uidOnly);
	Logger.TextMessage(MESSAGE_LEVEL_DEBUG, (master ? "> Current tag is master: true." : "> Current tag is master: false."));
	return master;
}

bool TagClass::CurrentTagIsKnown(bool uidOnly)
{
	Logger.TextMessage(MESSAGE_LEVEL_DEBUG, "> Current tag is known?");
	byte tags = EEPROM.read(ADDR_TAG_EXISTS);
	for (int i = 1; i < 8; i++)
		if (HaveFlag(tags, 1 << i) && CurrentTagIsKnownAsIndex(i, uidOnly))
		{
			Logger.TextMessage(MESSAGE_LEVEL_DEBUG, "> Current tag is known: true.");
			return true;
		}
	Logger.TextMessage(MESSAGE_LEVEL_DEBUG, "> Current tag is known: false.");
	return false;
}

bool TagClass::CurrentTagIsKnownAsIndex(byte index, bool uidOnly)
{
	Logger.TextMessage(MESSAGE_LEVEL_DEBUG, "> Current tag is known as index: " + index);
	for (int i = 0; i < 10; i++)
		if (EEPROM.read(ADDR_TAGS_BEGIN + TAG_EEPROM_SIZE * index + i) != (i < _uidLength ? _uid[i] : 0))
		{
			Logger.TextMessage(MESSAGE_LEVEL_DEBUG, " : false.");
			return false;
		}
	Logger.TextMessage(MESSAGE_LEVEL_DEBUG, " : true.");

	if (uidOnly)
		return true;

	byte seedPos = EEPROM.read(ADDR_SEED_BEGIN + SEED_EEPROM_SIZE * index);
	unsigned long seed = 0;
	unsigned long newSeed = 0;
	SetMasterKeyAsCurrentKey();
	if (!ReadSeedSector(&seedPos, &seed, &newSeed) ||
		!CheckRandomSectorByRandom(seed) ||
		!FillRandomSectorByRandom(newSeed))
		return false;
	EEPROM.write(ADDR_SEED_BEGIN + SEED_EEPROM_SIZE * index, seedPos);

	return true;
}

byte * TagClass::GetCurrentTag()
{
	return _uid;
}

bool TagClass::WriteSeedSector(byte *__seedPos, unsigned long *__seed)
{
	byte seedPos = *__seedPos;
	unsigned long seed = *__seed;
	seedPos = GetRandomSeedPositionInSeedSector();
	byte seedBlock = seedPos / 16;
	byte seedIndex = seedPos % 16;
	seed = 0;

	SetMasterKeyAsCurrentKey();
	for (byte block = 60; block < 63; block++)
	{
		for (byte i = 0; i < 16; i += 2)
		{
			int data = Rnd.Next();
			_buffer[i] = data >> 8;
			_buffer[i + 1] = data;

			if (seedBlock + 60 == block)
				if (i >= seedIndex && i < seedIndex + SEED_SIZE)
					seed += data << 8 * ((SEED_SIZE - 2) - (seedIndex - i));
		}
		if (!AuthKeyB(block, false) ||
			!WriteBlockFromBuffer(block))
			return false;
	}
	return true;
}

bool TagClass::ReadSeedSector(byte* __seedPos, unsigned long *__seed, unsigned long *__newSeed)
{
	unsigned long seed = *__seed;
	unsigned long newSeed = *__newSeed;
	seed = 0;
	newSeed = 0;
	byte seedPos = *__seedPos;
	byte newSeedPos = seedPos;
	while (seedPos == newSeedPos)
		newSeedPos = GetRandomSeedPositionInSeedSector();
	SetMasterKeyAsCurrentKey();
	for (byte block = 60; block < 63; block++)
	{
		bool readed = false;
		if (block - 60 == seedPos / 16)
		{
			if (!AuthKeyB(block, false) ||
				!ReadBlockToBuffer(block))
				return false;
			readed = true;
			for (byte i = seedPos % 16; i < seedPos % 16 + SEED_SIZE; i++)
				seed += _buffer[i] << 8 * (seedPos % 16 + SEED_SIZE - 1 - i);
		}
		if (block - 60 == newSeedPos / 16)
		{
			if (!readed)
				if (!AuthKeyB(block, false) ||
					!ReadBlockToBuffer(block))
					return false;
			for (byte i = newSeedPos % 16; i < newSeedPos % 16 + SEED_SIZE; i++)
				newSeed += _buffer[i] << 8 * (newSeedPos % 16 + SEED_SIZE - 1 - i);
		}
	}
	seedPos = newSeedPos;
	return true;
}

byte TagClass::GetRandomSeedPositionInSeedSector()
{
	byte seedBlock = Rnd.Next(0, 3);
	byte seedIndex = Rnd.Next(0, 16 / SEED_SIZE) * SEED_SIZE;
	return seedBlock * 16 + seedIndex;
}

void TagClass::SetMasterKeyAsCurrentKey()
{
	for (int i = 0; i < 6; i++)
		_key[i] = _masterKey[i];
}

bool TagClass::FillRandomSectorByRandom(unsigned long seed)
{
	Rnd.SetSeed(0, seed);
	byte sector = Rnd.Next(0, 8, 15);
	SetMasterKeyAsCurrentKey();
	for (byte block = sector * 4; block < sector * 4 + 3; block++)
	{
		for (byte i = 0; i < 16; i += 2)
		{
			int data = Rnd.Next(0);
			_buffer[i] = data >> 8;
			_buffer[i + 1] = data;
		}
		//PrintBuffer(block);
		Logger.LogBlockMessage(block, "fill_rnd", _buffer, 1);
		if (!AuthKeyB(block, false) ||
			!WriteBlockFromBuffer(block))
			return false;
	}
	return true;
}

bool TagClass::CheckRandomSectorByRandom(unsigned long seed)
{
	Rnd.SetSeed(0, seed);
	byte sector = Rnd.Next(0, 8, 15);
	SetMasterKeyAsCurrentKey();
	for (byte block = sector * 4; _status != 0 && block < sector * 4 + 3; block++)
	{
		if (!AuthKeyB(block, false) ||
			!ReadBlockToBuffer(block))
			return false;
		//PrintBuffer(block);
		for (byte i = 0; _status != 0 && i < 16; i += 2)
		{
			int data = Rnd.Next(0);
			byte d1 = data >> 8;
			byte d2 = data - (d1 << 8);

			//if (d1 < 0x10)
			//	Serial.print(F(" 0"));
			//else
			//	Serial.print(F(" "));
			//Serial.print(d1, HEX);

			//if (d2 < 0x10)
			//	Serial.print(F(" 0"));
			//else
			//	Serial.print(F(" "));
			//Serial.print(d2, HEX);

			if (_buffer[i] != d1 || _buffer[i + 1] != d2)
				_status = 0;
		}
		Logger.LogBlockMessage(block, "check_rnd", _buffer, 1);
	}
	return _status;
}

//void TagClass::FillBufferByRandom(byte rndIndex)
//{
//	for (byte i = 0; i < 16; i += 2)
//	{
//		int data = Rnd.Next(rndIndex);
//		_buffer[i] = data >> 8;
//		_buffer[i + 1] = data;
//	}
//}
//
//bool TagClass::CompareBufferByRandom(byte rndIndex)
//{
//	for (byte i = 0; i < 16; i += 2)
//	{
//		int data = Rnd.Next(rndIndex);
//		if (_buffer[i] != data >> 8) return false;
//		if (_buffer[i + 1] != data) return false;
//	}
//	return true;
//}

TagClass Tag;