#include "LockedState.h"
#include "../StateManager.h"
#include "../Defines.h"
#include "../Button.h"
#include "../Sound.h"
#include "../Tag.h"
#include "../Logger.h"


void LockedState::Init()
{
	Serial.println("==+> locked");
	StateManager.StartTimeout(1000);
	_activated = false;
}

void LockedState::Loop()
{
	if (!_activated && StateManager.IsTimeout())
	{
		_activated = true;
		Tag.ActivateListener();
	}

	if (Button.GetDown(BTN_INTERNAL))
		StateManager.SwitchStateTo(STATE_UNLOCKING);
	else if (Button.GetDown(OPENED_STATUS))
	{
		Sound.PlayOpened();
		StateManager.SwitchStateTo(STATE_OPENED);
	}
	else if (_activated && Tag.HaveTag())
	{
		if (Tag.CurrentTagIsKnown())
		{
			Logger.LogUnlockWithTag(Tag.GetCurrentTag());
			Sound.PlayOK();
			StateManager.SwitchStateTo(STATE_UNLOCKING);
		}
		else
		{
			Logger.LogUnlockWrongTag(Tag.GetCurrentTag());
			Sound.PlayError();
			StateManager.SwitchStateTo(STATE_LOCKED);
		}
		Tag.Stop();
	}
}

void LockedState::Exit()
{
	Serial.println("<+== locked");
	Tag.DeactivateListener();
}