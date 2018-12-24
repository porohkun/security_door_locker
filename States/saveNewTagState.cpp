#include "../StateManager.h"
#include "../Defines.h"
#include "../Button.h"


void saveNewTagState()
{
	Serial.println("==+> save new tag");
	digitalWrite(LED_01, HIGH);
	digitalWrite(LED_02, LOW);
	digitalWrite(LED_04, LOW);
	digitalWrite(LED_08, HIGH);
}


void saveNewTagStateLoop()
{
	digitalWrite(LED_16, HIGH);

	StateManager.SwitchStateTo(STATE_OPENED);
}


void saveNewTagStateExit() {}
