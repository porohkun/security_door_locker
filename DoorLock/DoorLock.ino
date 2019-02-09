/*
	Name:       DoorLock.ino
	Created:	11.12.2018 0:34:21
	Author:     POROH-PC\P
*/

//#include <EEPROM.h>
#include "Button.h"
#include "Defines.h"
#include "StateManager.h"
#include "Sound.h"
#include <EEPROM.h>
#include "Flags.h"


void setup()
{
	pinMode(UNLOCK_SIGNAL, OUTPUT);
	pinMode(LED_BUTTON, OUTPUT);

	Serial.begin(115200);
	Serial.println("!!! Started");


	//digitalWrite(UNLOCK_SIGNAL, HIGH);
	//digitalWrite(LED_BUTTON, HIGH);

	/*for (int i = 0; i < EEPROM.length(); i++) {
		EEPROM.write(i, 0);
	}*/

	Sound.Initial(BEEEPER);
	Sound.PlayClosed();
	StateManager.Init();

	randomSeed(analogRead(7));
}

void loop()
{
	Button.Loop();
	Sound.Loop();
	StateManager.Loop();
}
