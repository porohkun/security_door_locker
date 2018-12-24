/*
	Name:       DoorLock.ino
	Created:	11.12.2018 0:34:21
	Author:     POROH-PC\P
*/

//#include <EEPROM.h>
#include "Button.h"
#include "Defines.h"
#include "StateManager.h"
#include <EEPROM.h>
#include "Flags.h"


void setup()
{
	pinMode(LED_01, OUTPUT);
	pinMode(LED_02, OUTPUT);
	pinMode(LED_04, OUTPUT);
	pinMode(LED_08, OUTPUT);
	pinMode(LED_16, OUTPUT);

	Serial.begin(115200);

	for (int i = 0; i < EEPROM.length(); i++) {
		EEPROM.write(i, 0);
	}

	StateManager.Init();

	randomSeed(analogRead(7));
}

void loop()
{
	Button.Loop();
	StateManager.Loop();
}
