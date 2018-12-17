#include "../StateManager.h"
#include "../Defines.h"
#include "../Button.h"


void closedState()
{
	Serial.println("==+> closed");
	digitalWrite(LED_01, HIGH);
	digitalWrite(LED_02, LOW);
	digitalWrite(LED_04, HIGH);
	digitalWrite(LED_08, LOW);

	StateManager.StartTimeout(10000);
}


void closedStateLoop()
{
	digitalWrite(LED_16, HIGH);

	if (StateManager.IsTimeout())
		StateManager.SwitchStateTo(STATE_LOCKING);
}
