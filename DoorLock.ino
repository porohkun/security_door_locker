/*
	Name:       DoorLock.ino
	Created:	11.12.2018 0:34:21
	Author:     POROH-PC\P
*/

//#include <EEPROM.h>
#include "Toggle.h"
#include "Button.h"
#include "Defines.h"
#include "StateManager.h"


void setup()
{
	pinMode(LED_01, OUTPUT);
	pinMode(LED_02, OUTPUT);
	pinMode(LED_04, OUTPUT);
	pinMode(LED_08, OUTPUT);
	pinMode(LED_16, OUTPUT);

	Serial.begin(115200);

	StateManager.Init();
}

void loop()
{
	Button.Loop();
	StateManager.Loop();
}
