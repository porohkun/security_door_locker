#include "Arduino.h"
#include "Logger.h"

void LoggerClass::ActivateLogger()
{
	Serial.println("!!! Activate Logger");
}

void LoggerClass::LogUnlockWithTag(byte * uid)
{
	Serial.println("!!! Unlock With Tag");
}

void LoggerClass::LogUnlockWrongTag(byte * uid)
{
	Serial.println("!!! Unlock Wrong Tag");
}

LoggerClass Logger;