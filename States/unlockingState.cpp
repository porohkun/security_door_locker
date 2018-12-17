#include "../StateManager.h"
#include "../Defines.h"
#include "../Button.h"


void unlockingState()
{
	Serial.println("==+> unlocking");
	digitalWrite(LED_01, LOW);
	digitalWrite(LED_02, HIGH);
	digitalWrite(LED_04, LOW);
	digitalWrite(LED_08, LOW);
}


void unlockingStateLoop()
{
	digitalWrite(LED_16, HIGH);

	if (Button.GetUp(BTN_LOCKED))
		StateManager.SwitchStateTo(STATE_UNLOCKED);
	else if (Button.GetUp(REED_SWITCH))
		StateManager.SwitchStateTo(STATE_OPENED);
}
