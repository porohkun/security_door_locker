#include "UnlockingState.h"
#include "../StateManager.h"
#include "../Defines.h"
#include "../Button.h"
#include "../Sound.h"


void UnlockingState::Init()
{
	digitalWrite(UNLOCK_SIGNAL, HIGH);
	delay(200);
	digitalWrite(UNLOCK_SIGNAL, LOW);
}

void UnlockingState::Loop()
{
	if (Button.GetDown(OPENED_STATUS))
	{
		Sound.PlayOpened();
		StateManager.SwitchStateTo(STATE_OPENED);
	}
}

void UnlockingState::Exit() {}