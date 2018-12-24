#include "../StateManager.h"
#include "../Defines.h"
#include "../Button.h"


void lockedState()
{
	Serial.println("==+> locked");
	digitalWrite(LED_01, LOW);
	digitalWrite(LED_02, LOW);
	digitalWrite(LED_04, LOW);
	digitalWrite(LED_08, LOW);
}

void lockedStateLoop()
{
	digitalWrite(LED_16, HIGH);

	if (Button.GetDown(BTN_INTERNAL))
		StateManager.SwitchStateTo(STATE_UNLOCKING);
	else if (Button.GetDown(BTN_EXTERNAL))
		StateManager.SwitchStateTo(STATE_LISTEN_FOR_UNLOCK);
	else if (Button.GetUp(BTN_LOCKED))
		StateManager.SwitchStateTo(STATE_UNLOCKED);
}


void lockedStateExit() {}
