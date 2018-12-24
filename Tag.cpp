#include "Arduino.h"
#include "Defines.h"
#include "Tag.h"
#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>
#include "Flags.h"


void TagClass::ActivateListener()
{
	SPI.begin();           // MFRC522 Hardware uses SPI protocol
	mfrc522.PCD_Init();    // Initialize MFRC522 Hardware

	//If you set Antenna Gain to Max it will increase reading distance
	//mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max);
}

void TagClass::DeactivateListener()
{
	Stop();
	SPI.end();
}


bool TagClass::HaveTag()
{
	// Look for new cards
	if (!mfrc522.PICC_IsNewCardPresent())
		return false;

	// Select one of the cards
	if (!mfrc522.PICC_ReadCardSerial())
		return false;

	Serial.println("> Card found.");
	return true;
}

void TagClass::FillBuffer(byte *key1, byte *accBits, byte *key2)
{
	for (int i = 0; i < 6; i++)
		buffer[i] = key1[i];
	for (int i = 0; i < 4; i++)
		buffer[i + 6] = accBits[i];
	for (int i = 0; i < 6; i++)
		buffer[i + 10] = key2[i];
}

void TagClass::PrintBuffer()
{
	Serial.print("    Buffer:  ");
	PrintBufferInternal();
}

void TagClass::PrintBuffer(byte block_)
{
	if (block_ < 10)
		Serial.print("         ");
	else
		Serial.print("        ");
	Serial.print(block_);
	Serial.print(":  ");
	PrintBufferInternal();
}

void TagClass::PrintBufferInternal()
{
	for (byte index = 0; index < 16; index++) {
		if (buffer[index] < 0x10)
			Serial.print(F(" 0"));
		else
			Serial.print(F(" "));
		Serial.print(buffer[index], HEX);
		if ((index % 4) == 3) {
			Serial.print(F(" "));
		}
	}
	Serial.println();
}

bool TagClass::AuthKeyA(MFRC522::MIFARE_Key *key, byte block_, bool silent)
{
	return AuthKey(MFRC522::PICC_CMD_MF_AUTH_KEY_A, key, block_, silent);
}

bool TagClass::AuthKeyB(MFRC522::MIFARE_Key *key, byte block_, bool silent)
{
	return AuthKey(MFRC522::PICC_CMD_MF_AUTH_KEY_B, key, block_, silent);
}

bool TagClass::AuthKey(byte command, MFRC522::MIFARE_Key *key, byte block_, bool silent)
{
	if (!silent)
	{
		switch (command)
		{
		case MFRC522::PICC_CMD_MF_AUTH_KEY_A:
			Serial.print("> Auth key A: ");
			break;
		case MFRC522::PICC_CMD_MF_AUTH_KEY_B:
			Serial.print("> Auth key B: ");
			break;
		default:
			return false;
		}
		Serial.print(block_);
		Serial.print(": ");
		for (byte i = 0; i < 6; i++) {
			if (key->keyByte[i] < 0x10)
				Serial.print(F(" 0"));
			else
				Serial.print(F(" "));
			Serial.print(key->keyByte[i], HEX);
		}
	}

	status = mfrc522.PCD_Authenticate(command, block_, key, &(mfrc522.uid));
	if (status != MFRC522::STATUS_OK) {
		if (!silent)
		{
			Serial.print("  failed: ");
			Serial.println(mfrc522.GetStatusCodeName(status));
		}
		return false;
	}
	if (!silent)
		Serial.println("  done.");
	return true;
}

bool TagClass::ReadBlock(byte block_)
{
	Serial.print("> Reading block ");
	Serial.print(block_);

	status = mfrc522.MIFARE_Read(block_, buffer, &byteCount);
	if (status != MFRC522::STATUS_OK) {
		Serial.print(": failed: ");
		Serial.println(mfrc522.GetStatusCodeName(status));
		return false;
	}
	Serial.println("  done.");
	return true;
}

bool TagClass::WriteBlock(byte block_)
{
	Serial.print("> Writing block ");
	Serial.print(block_);

	status = mfrc522.MIFARE_Write(block_, buffer, 16);
	if (status != MFRC522::STATUS_OK) {
		Serial.print(": failed: ");
		Serial.println(mfrc522.GetStatusCodeName(status));
		return false;
	}
	Serial.println("  done.");
	return true;
}

void TagClass::Stop()
{
	Serial.println("> Listener stopped.");
	mfrc522.PICC_HaltA(); // Halt the PICC before stopping the encrypted session.
	mfrc522.PCD_StopCrypto1();
}


bool TagClass::HaveMasterTag()
{
	bool have = HaveFlag(EEPROM.read(ADDR_TAG_EXISTS), BIT_00);
	Serial.print("> Have master tag: ");
	Serial.println(have ? "true." : "false.");
	return have;
}

void TagClass::SaveCurrentTagAsMaster()
{
	SaveCurrentTagAtIndex(0);
	Serial.println("> Saved current tag as master.");
}

bool TagClass::SaveCurrentTag()
{
	Serial.print("> Saving current tag ... ");
	byte tags = EEPROM.read(ADDR_TAG_EXISTS);
	for (int i = 1; i < 8; i++)
		if (!HaveFlag(tags, 1 << i))
		{
			Serial.print("at index ");
			Serial.println(i);
			SaveCurrentTagAtIndex(i);
			return true;
		}
	Serial.println("failed.");
	return false;
}

void TagClass::SaveCurrentTagAtIndex(byte index)
{
	Serial.print("  keys: ");
	Serial.println(EEPROM.read(ADDR_TAG_EXISTS));
	for (int i = 0; i < 10; i++)
		EEPROM.write(ADDR_TAGS_BEGIN + TAG_EEPROM_SIZE * index + i, (i < mfrc522.uid.size ? mfrc522.uid.uidByte[i] : 0));
	if (index == 0)
		EEPROM.write(ADDR_TAG_EXISTS, BIT_00);
	else
	{
		Serial.print("  index: ");
		Serial.print(index);
		Serial.print(" 1<<index: ");
		Serial.print(1 << index);
		Serial.print(" flags: ");
		Serial.print(EEPROM.read(ADDR_TAG_EXISTS));
		Serial.print(" addflag: ");
		Serial.print(AddFlag(EEPROM.read(ADDR_TAG_EXISTS), 1 << index));
		EEPROM.write(ADDR_TAG_EXISTS, AddFlag(EEPROM.read(ADDR_TAG_EXISTS), 1 << index));
	}
	Serial.print("> Saved current tag at index ");
	Serial.println(index);
	Serial.print("  keys: ");
	Serial.println(EEPROM.read(ADDR_TAG_EXISTS));
}

bool TagClass::CurrentTagIsMaster()
{
	bool master = CurrentTagIsKnownAsIndex(0);
	Serial.print("> Current tag is master: ");
	Serial.println(master ? "true." : "false.");
	return master;
}

bool TagClass::CurrentTagIsKnown()
{
	Serial.println("> Current tag is known?");
	byte tags = EEPROM.read(ADDR_TAG_EXISTS);
	for (int i = 1; i < 8; i++)
		if (HaveFlag(tags, 1 << i) && CurrentTagIsKnownAsIndex(i))
		{
			Serial.println("> Current tag is known: true.");
			return true;
		}
	Serial.println("> Current tag is known: false.");
	return false;
}

bool TagClass::CurrentTagIsKnownAsIndex(byte index)
{
	Serial.print("> Current tag is known as index: ");
	Serial.print(index);
	for (int i = 0; i < 10; i++)
		if (EEPROM.read(ADDR_TAGS_BEGIN + TAG_EEPROM_SIZE * index + i) != (i < mfrc522.uid.size ? mfrc522.uid.uidByte[i] : 0))
		{
			Serial.println(" : false.");
			return false;
		}
	Serial.println(" : true.");
	return true;
}

MFRC522::Uid TagClass::GetCurrentTag()
{
	return mfrc522.uid;
}

TagClass Tag;