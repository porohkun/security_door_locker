#include "../StateManager.h"
#include "../Defines.h"
#include "../Button.h"


void openedState()
{
	Serial.println("==+> opened");
	digitalWrite(LED_01, LOW);
	digitalWrite(LED_02, LOW);
	digitalWrite(LED_04, HIGH);
	digitalWrite(LED_08, LOW);
}


void openedStateLoop()
{
	digitalWrite(LED_16, HIGH);

	if (Button.GetDown(REED_SWITCH))
		StateManager.SwitchStateTo(STATE_CLOSED);
	else if ((Button.GetDown(BTN_INTERNAL) && Button.GetState(BTN_EXTERNAL)) || (Button.GetDown(BTN_EXTERNAL) && Button.GetState(BTN_INTERNAL)))
		StateManager.SwitchStateTo(STATE_LISTEN_FOR_MASTER);
}


void openedStateExit() {}
