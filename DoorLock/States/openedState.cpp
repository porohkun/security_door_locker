#include "OpenedState.h"
#include "../StateManager.h"
#include "../Defines.h"
#include "../Button.h"
#include "../Sound.h"


void OpenedState::Init()
{
	Serial.println("==+> opened");

	digitalWrite(LED_BUTTON, HIGH);
}

void OpenedState::Loop()
{
	if (!Button.GetState(OPENED_STATUS))
	{
		Sound.PlayClosed();
		StateManager.SwitchStateTo(STATE_LOCKED);
	}
	else if (Button.GetDown(BTN_INTERNAL))
	{
		Sound.PlayBeep();
		StateManager.SwitchStateTo(STATE_LISTEN_FOR_MASTER);
	}
}

void OpenedState::Exit()
{
	digitalWrite(LED_BUTTON, LOW);
	Serial.println("<+== opened");
}