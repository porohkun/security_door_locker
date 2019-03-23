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
	bool ReadBlockToBuffer(byte block_);
	bool WriteBlockFromBuffer(byte block_);
	void Stop();

	bool HaveMasterTag();
	bool SaveCurrentTagAsMaster();
	bool SaveCurrentTag();
	bool CurrentTagIsMaster(bool uidOnly);
	bool CurrentTagIsKnown(bool uidOnly);
	MFRC522::Uid GetCurrentTag();

	unsigned long WriteSeedSector(byte tagIndex, unsigned long dataSeed);


private:
	void PrintBufferInternal();
	bool AuthKey(byte command, MFRC522::MIFARE_Key *key, byte block_, bool silent);
	bool SaveCurrentTagAtIndex(byte index);
	bool CurrentTagIsKnownAsIndex(byte index, bool uidOnly);
	bool WriteSeedSector(byte *__seedPos, unsigned long * __seed);
	bool ReadSeedSector(byte* __seedPos, unsigned long *__seed, unsigned long *__newSeed);
	byte GetRandomSeedPositionInSeedSector();
	void SetMasterKeyAsCurrentKey();
	bool FillRandomSectorByRandom(unsigned long seed);
	bool CheckRandomSectorByRandom(unsigned long seed);
	/*void FillBufferByRandom(byte rndIndex);
	bool CompareBufferByRandom(byte rndIndex);*/
	MFRC522::StatusCode status;
	byte _buffer[18];
	byte _byteCount = sizeof(_buffer);
	byte _masterKey[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	MFRC522 _mfrc522 = MFRC522(SS_PIN, RST_PIN);
	MFRC522::MIFARE_Key _key;
};

extern TagClass Tag;
