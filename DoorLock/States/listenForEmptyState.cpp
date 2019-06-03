#include "ListenForEmptyState.h"
#include "../StateManager.h"
#include "../Defines.h"
#include "../Button.h"
#include "../Sound.h"
#include "../Tag.h"
#include "../Logger.h"


void ListenForEmptyState::Init()
{
	StateManager.StartTimeout(1000);
	_activated = false;
}

void ListenForEmptyState::Loop()
{
	if (!_activated && StateManager.IsTimeout())
	{
		_activated = true;
		Tag.ActivateListener();
		StateManager.StartTimeout(10000);
	}

	if (!Button.GetState(OPENED_STATUS))
	{
		Sound.PlayClosed();
		StateManager.SwitchStateTo(STATE_LOCKED);
	}
	else if (_activated && StateManager.IsTimeout())
	{
		Sound.PlayError();
		StateManager.SwitchStateTo(STATE_OPENED);
	}
	else if (_activated && Tag.HaveTag())
	{
		if (!Tag.CurrentTagIsMaster(true) && !Tag.CurrentTagIsKnown(true))
		{
			Sound.PlayOK();
			Tag.SaveCurrentTag();
		}
		else
			Sound.PlayError();
		StateManager.SwitchStateTo(STATE_OPENED);
		Tag.Stop();
	}
}

void ListenForEmptyState::Exit()
{
	Tag.DeactivateListener();
}