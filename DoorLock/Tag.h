#include "Arduino.h"
#include "Defines.h"
#include <Wire.h>
#include <SPI.h>
#include "Adafruit_PN532.h"

#define MIFARE_Key_A 0x0
#define MIFARE_Key_B 0x1

class TagClass
{
public:
	void ActivateListener();
	void DeactivateListener();
	bool HaveTag();
	void FillBuffer(byte *key1, byte *accBits, byte *key2);
	//void PrintBuffer();
	//void PrintBuffer(byte block_);
	bool AuthKeyA(byte block_, bool silent);
	bool AuthKeyB(byte block_, bool silent);
	bool ReadBlockToBuffer(byte block_);
	bool WriteBlockFromBuffer(byte block_);
	void Stop();

	bool HaveMasterTag();
	bool SaveCurrentTagAsMaster();
	bool SaveCurrentTag();
	bool CurrentTagIsMaster(bool uidOnly);
	bool CurrentTagIsKnown(bool uidOnly);
	byte * GetCurrentTag();
	byte GetCurrentTagLength();

	unsigned long WriteSeedSector(byte tagIndex, unsigned long dataSeed);


private:
	void PrintBufferInternal();
	bool AuthKey(byte command, byte block_, bool silent);
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
	byte _status;
	byte _buffer[16];
	byte _byteCount = sizeof(_buffer);
	byte _masterKey[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	byte _key[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	Adafruit_PN532 _pn532 = Adafruit_PN532(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);
	byte _uid[7] = { 0, 0, 0, 0, 0, 0, 0 };
	byte _uidLength;
};

extern TagClass Tag;
