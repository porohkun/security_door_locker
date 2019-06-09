#include "LockedState.h"
#include "../StateManager.h"
#include "../Defines.h"
#include "../Button.h"
#include "../Sound.h"
#include "../Tag.h"
#include "../Logger.h"


void LockedState::Init()
{
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
		Logger.UnlockedWithoutTag();
		Sound.PlayOpened();
		StateManager.SwitchStateTo(STATE_OPENED);
	}
	else if (_activated && Tag.HaveTag())
	{
		if (Tag.CurrentTagIsKnown(false))
		{
			Logger.UnlockedWithTag(Tag.GetCurrentTag(), Tag.GetCurrentTagLength(), true);
			Sound.PlayOK();
			StateManager.SwitchStateTo(STATE_UNLOCKING);
		}
		else
		{
			Logger.UnlockedWithTag(Tag.GetCurrentTag(), Tag.GetCurrentTagLength(), false);
			Sound.PlayError();
			StateManager.SwitchStateTo(STATE_LOCKED);
		}
		Tag.Stop();
	}
}

void LockedState::Exit()
{
	Tag.DeactivateListener();
}