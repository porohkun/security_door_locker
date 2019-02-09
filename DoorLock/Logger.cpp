#include "Arduino.h"
#include "Logger.h"
#include <MFRC522.h>

void LoggerClass::ActivateLogger()
{
	Serial.println("!!! Activate Logger");
}

void LoggerClass::LogUnlockWithTag(MFRC522::Uid uid)
{
	Serial.println("!!! Unlock With Tag");
}

void LoggerClass::LogUnlockWrongTag(MFRC522::Uid uid)
{
	Serial.println("!!! Unlock Wrong Tag");
}

LoggerClass Logger;