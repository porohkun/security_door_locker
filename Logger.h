#include "Arduino.h"
#include <MFRC522.h>

class LoggerClass
{
public:
	void ActivateLogger();
	void LogUnlockWithTag(MFRC522::Uid uid);
	void LogUnlockWrongTag(MFRC522::Uid uid);

private:
};

extern LoggerClass Logger;