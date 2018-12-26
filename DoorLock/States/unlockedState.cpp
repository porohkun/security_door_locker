#include "../StateManager.h"
#include "../Defines.h"
#include "../Button.h"


void unlockedState()
{
	Serial.println("==+> unlocked");
	digitalWrite(LED_01, HIGH);
	digitalWrite(LED_02, HIGH);
	digitalWrite(LED_04, LOW);
	digitalWrite(LED_08, LOW);

	StateManager.StartTimeout(5000);
}


void unlockedStateLoop()
{
	digitalWrite(LED_16, HIGH);

	if (StateManager.IsTimeout())
		StateManager.SwitchStateTo(STATE_CLOSED);
	else if (Button.GetUp(REED_SWITCH))
		StateManager.SwitchStateTo(STATE_OPENED);
}


void unlockedStateExit() {}
