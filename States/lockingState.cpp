#include "../StateManager.h"
#include "../Defines.h"
#include "../Button.h"


void lockingState()
{
	Serial.println("==+> locking");
	digitalWrite(LED_01, LOW);
	digitalWrite(LED_02, HIGH);
	digitalWrite(LED_04, HIGH);
	digitalWrite(LED_08, LOW);
}


void lockingStateLoop()
{
	digitalWrite(LED_16, HIGH);

	if (Button.GetDown(BTN_LOCKED))
		StateManager.SwitchStateTo(STATE_LOCKED);
}


void lockingStateExit() {}
