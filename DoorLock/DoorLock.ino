/*
	Name:       DoorLock.ino
	Created:	11.12.2018 0:34:21
	Author:     POROH-PC\P
*/

#include "Button.h"
#include "Defines.h"
#include "StateManager.h"
#include "Sound.h"
#include <EEPROM.h>
#include "Flags.h"
#include "Rnd.h"
#include "Logger.h"

void setup()
{
	pinMode(UNLOCK_SIGNAL, OUTPUT);
	pinMode(LED_BUTTON, OUTPUT);

	Serial.begin(115200);

	//digitalWrite(UNLOCK_SIGNAL, HIGH);
	//digitalWrite(LED_BUTTON, HIGH);

	byte buttons[] = { BTN_INTERNAL, BTN_EXTERNAL, OPENED_STATUS, BTN_CLR_EEPROM, 0xff };
	Button.SetButtons(buttons);

	Sound.Initial(BEEEPER);
	Sound.PlayClosed();
	StateManager.Init();

	Logger.Started();
}

void loop()
{
	if (Button.GetUp(BTN_CLR_EEPROM))
	{
		for (int i = 0; i < EEPROM.length(); i++)
			EEPROM.write(i, 0);
		Sound.PlayClosed();
		Logger.EEPROMClearedMessage();
	}

	Button.Loop();
	Sound.Loop();
	StateManager.Loop();
}
