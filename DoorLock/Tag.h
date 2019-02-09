#include "Arduino.h"
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

class TagClass
{
public:
	void ActivateListener();
	void DeactivateListener();
	bool HaveTag();
	void DumpToSerial(MFRC522::Uid * uid);
	void PICC_DumpMifareClassicToSerial(MFRC522::Uid * uid, MFRC522::PICC_Type piccType, MFRC522::MIFARE_Key * key);
	void PICC_DumpMifareClassicSectorToSerial(MFRC522::Uid * uid, MFRC522::MIFARE_Key * key, byte sector);
	void FillBuffer(byte *key1, byte *accBits, byte *key2);
	void PrintBuffer();
	void PrintBuffer(byte block_);
	bool AuthKeyA(MFRC522::MIFARE_Key *key, byte block_, bool silent);
	bool AuthKeyB(MFRC522::MIFARE_Key *key, byte block_, bool silent);
	bool ReadBlock(byte block_);
	bool WriteBlock(byte block_);
	void Stop();

	bool HaveMasterTag();
	void SaveCurrentTagAsMaster();
	bool SaveCurrentTag();
	bool CurrentTagIsMaster();
	bool CurrentTagIsKnown();
	MFRC522::Uid GetCurrentTag();

private:
	void PrintBufferInternal();
	bool AuthKey(byte command, MFRC522::MIFARE_Key *key, byte block_, bool silent);
	void SaveCurrentTagAtIndex(byte index);
	bool CurrentTagIsKnownAsIndex(byte index);
	MFRC522::StatusCode status;
	byte buffer[18];
	byte byteCount = sizeof(buffer);
	MFRC522 mfrc522 = MFRC522(SS_PIN, RST_PIN);
	MFRC522::MIFARE_Key key;
};

extern TagClass Tag;
