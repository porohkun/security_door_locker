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
	mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid));
	mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
	return true;
}

void TagClass::DumpToSerial(MFRC522::Uid *uid) {
	MFRC522::MIFARE_Key key;

	// Dump UID, SAK and Type
	mfrc522.PICC_DumpDetailsToSerial(uid);

	// Dump contents
	MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(uid->sak);
	switch (piccType) {
	case MFRC522::PICC_TYPE_MIFARE_MINI:
	case MFRC522::PICC_TYPE_MIFARE_1K:
	case MFRC522::PICC_TYPE_MIFARE_4K:
		// All keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
		for (byte i = 0; i < 6; i++) {
			key.keyByte[i] = 0xFF;
		}
		PICC_DumpMifareClassicToSerial(uid, piccType, &key);
		break;

	case MFRC522::PICC_TYPE_MIFARE_UL:
		mfrc522.PICC_DumpMifareUltralightToSerial();
		break;

	case MFRC522::PICC_TYPE_ISO_14443_4:
	case MFRC522::PICC_TYPE_MIFARE_DESFIRE:
	case MFRC522::PICC_TYPE_ISO_18092:
	case MFRC522::PICC_TYPE_MIFARE_PLUS:
	case MFRC522::PICC_TYPE_TNP3XXX:
		Serial.println(F("Dumping memory contents not implemented for that PICC type."));
		break;

	case MFRC522::PICC_TYPE_UNKNOWN:
	case MFRC522::PICC_TYPE_NOT_COMPLETE:
	default:
		break; // No memory dump here
	}

	Serial.println();
	mfrc522.PICC_HaltA(); // Already done if it was a MIFARE Classic PICC.
} // End PICC_DumpToSerial()

void TagClass::PICC_DumpMifareClassicToSerial(MFRC522::Uid *uid,			///< Pointer to Uid struct returned from a successful PICC_Select().
	MFRC522::PICC_Type piccType,	///< One of the PICC_Type enums.
	MFRC522::MIFARE_Key *key		///< Key A used for all sectors.
) {
	byte no_of_sectors = 0;
	switch (piccType) {
	case MFRC522::PICC_TYPE_MIFARE_MINI:
		// Has 5 sectors * 4 blocks/sector * 16 bytes/block = 320 bytes.
		no_of_sectors = 5;
		break;

	case MFRC522::PICC_TYPE_MIFARE_1K:
		// Has 16 sectors * 4 blocks/sector * 16 bytes/block = 1024 bytes.
		no_of_sectors = 16;
		break;

	case MFRC522::PICC_TYPE_MIFARE_4K:
		// Has (32 sectors * 4 blocks/sector + 8 sectors * 16 blocks/sector) * 16 bytes/block = 4096 bytes.
		no_of_sectors = 40;
		break;

	default: // Should not happen. Ignore.
		break;
	}

	// Dump sectors, highest address first.
	if (no_of_sectors) {
		Serial.println(F("Sector Block   0  1  2  3   4  5  6  7   8  9 10 11  12 13 14 15  AccessBits"));
		for (int8_t i = no_of_sectors - 1; i >= 0; i--) {
			PICC_DumpMifareClassicSectorToSerial(uid, key, i);
		}
	}
	mfrc522.PICC_HaltA(); // Halt the PICC before stopping the encrypted session.
	mfrc522.PCD_StopCrypto1();
} // End PICC_DumpMifareClassicToSerial()

void TagClass::PICC_DumpMifareClassicSectorToSerial(MFRC522::Uid *uid,			///< Pointer to Uid struct returned from a successful PICC_Select().
	MFRC522::MIFARE_Key *key,	///< Key A for the sector.
	byte sector			///< The sector to dump, 0..39.
) {
	MFRC522::StatusCode status;
	byte firstBlock;		// Address of lowest address to dump actually last block dumped)
	byte no_of_blocks;		// Number of blocks in sector
	bool isSectorTrailer;	// Set to true while handling the "last" (ie highest address) in the sector.

	// The access bits are stored in a peculiar fashion.
	// There are four groups:
	//		g[3]	Access bits for the sector trailer, block 3 (for sectors 0-31) or block 15 (for sectors 32-39)
	//		g[2]	Access bits for block 2 (for sectors 0-31) or blocks 10-14 (for sectors 32-39)
	//		g[1]	Access bits for block 1 (for sectors 0-31) or blocks 5-9 (for sectors 32-39)
	//		g[0]	Access bits for block 0 (for sectors 0-31) or blocks 0-4 (for sectors 32-39)
	// Each group has access bits [C1 C2 C3]. In this code C1 is MSB and C3 is LSB.
	// The four CX bits are stored together in a nible cx and an inverted nible cx_.
	byte c1, c2, c3;		// Nibbles
	byte c1_, c2_, c3_;		// Inverted nibbles
	bool invertedError;		// True if one of the inverted nibbles did not match
	byte g[4];				// Access bits for each of the four groups.
	byte group;				// 0-3 - active group for access bits
	bool firstInGroup;		// True for the first block dumped in the group

	// Determine position and size of sector.
	if (sector < 32) { // Sectors 0..31 has 4 blocks each
		no_of_blocks = 4;
		firstBlock = sector * no_of_blocks;
	}
	else if (sector < 40) { // Sectors 32-39 has 16 blocks each
		no_of_blocks = 16;
		firstBlock = 128 + (sector - 32) * no_of_blocks;
	}
	else { // Illegal input, no MIFARE Classic PICC has more than 40 sectors.
		return;
	}

	// Dump blocks, highest address first.
	byte byteCount;
	byte buffer[18];
	byte blockAddr;
	isSectorTrailer = true;
	invertedError = false;	// Avoid "unused variable" warning.
	for (int8_t blockOffset = no_of_blocks - 1; blockOffset >= 0; blockOffset--) {
		blockAddr = firstBlock + blockOffset;
		// Sector number - only on first line
		if (isSectorTrailer) {
			if (sector < 10)
				Serial.print(F("   ")); // Pad with spaces
			else
				Serial.print(F("  ")); // Pad with spaces
			Serial.print(sector);
			Serial.print(F("   "));
		}
		else {
			Serial.print(F("       "));
		}
		// Block number
		if (blockAddr < 10)
			Serial.print(F("   ")); // Pad with spaces
		else {
			if (blockAddr < 100)
				Serial.print(F("  ")); // Pad with spaces
			else
				Serial.print(F(" ")); // Pad with spaces
		}
		Serial.print(blockAddr);
		Serial.print(F("  "));
		// Establish encrypted communications before reading the first block
		if (isSectorTrailer) {
			status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, firstBlock, key, uid);
			if (status != MFRC522::STATUS_OK) {
				Serial.print(F("PCD_Authenticate() failed: "));
				Serial.println(mfrc522.GetStatusCodeName(status));
				continue;
			}
		}
		// Read block
		byteCount = sizeof(buffer);
		status = mfrc522.MIFARE_Read(blockAddr, buffer, &byteCount);
		if (status != MFRC522::STATUS_OK) {
			Serial.print(F("MIFARE_Read() failed: "));
			Serial.println(mfrc522.GetStatusCodeName(status));
			continue;
		}
		// Dump data
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
		// Parse sector trailer data
		if (isSectorTrailer) {
			c1 = buffer[7] >> 4;
			c2 = buffer[8] & 0xF;
			c3 = buffer[8] >> 4;
			c1_ = buffer[6] & 0xF;
			c2_ = buffer[6] >> 4;
			c3_ = buffer[7] & 0xF;
			invertedError = (c1 != (~c1_ & 0xF)) || (c2 != (~c2_ & 0xF)) || (c3 != (~c3_ & 0xF));
			g[0] = ((c1 & 1) << 2) | ((c2 & 1) << 1) | ((c3 & 1) << 0);
			g[1] = ((c1 & 2) << 1) | ((c2 & 2) << 0) | ((c3 & 2) >> 1);
			g[2] = ((c1 & 4) << 0) | ((c2 & 4) >> 1) | ((c3 & 4) >> 2);
			g[3] = ((c1 & 8) >> 1) | ((c2 & 8) >> 2) | ((c3 & 8) >> 3);
			isSectorTrailer = false;
		}

		// Which access group is this block in?
		if (no_of_blocks == 4) {
			group = blockOffset;
			firstInGroup = true;
		}
		else {
			group = blockOffset / 5;
			firstInGroup = (group == 3) || (group != (blockOffset + 1) / 5);
		}

		if (firstInGroup) {
			// Print access bits
			Serial.print(F(" [ "));
			Serial.print((g[group] >> 2) & 1, DEC); Serial.print(F(" "));
			Serial.print((g[group] >> 1) & 1, DEC); Serial.print(F(" "));
			Serial.print((g[group] >> 0) & 1, DEC);
			Serial.print(F(" ] "));
			if (invertedError) {
				Serial.print(F(" Inverted access bits did not match! "));
			}
		}

		if (group != 3 && (g[group] == 1 || g[group] == 6)) { // Not a sector trailer, a value block
			int32_t value = (int32_t(buffer[3]) << 24) | (int32_t(buffer[2]) << 16) | (int32_t(buffer[1]) << 8) | int32_t(buffer[0]);
			Serial.print(F(" Value=0x")); Serial.print(value, HEX);
			Serial.print(F(" Adr=0x")); Serial.print(buffer[12], HEX);
		}
		Serial.println();
	}

	return;
} // End PICC_DumpMifareClassicSectorToSerial()

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