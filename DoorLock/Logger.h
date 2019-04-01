#include "Arduino.h"

class LoggerClass
{
public:
	void ActivateLogger();
	void LogUnlockWithTag(byte * uid);
	void LogUnlockWrongTag(byte * uid);

private:
};

extern LoggerClass Logger;