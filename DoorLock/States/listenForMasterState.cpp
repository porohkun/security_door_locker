#include "ListenForMasterState.h"
#include "../StateManager.h"
#include "../Defines.h"
#include "../Button.h"
#include "../Sound.h"
#include "../Tag.h"
#include "../Logger.h"


void ListenForMasterState::Init()
{
	Serial.println("==+> listen for master");
	StateManager.StartTimeout(1000);
	_activated = false;
}

void ListenForMasterState::Loop()
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
		if (Tag.HaveMasterTag())
			if (Tag.CurrentTagIsMaster())
			{
				Sound.PlayBeep();
				StateManager.SwitchStateTo(STATE_LISTEN_FOR_EMPTY);
			}
			else
			{
				Sound.PlayError();
				StateManager.SwitchStateTo(STATE_OPENED);
			}
		else
		{
			Tag.SaveCurrentTagAsMaster();
			Sound.PlayOK();
			StateManager.SwitchStateTo(STATE_OPENED);
		}
		Tag.Stop();
	}
}

void ListenForMasterState::Exit()
{
	Serial.println("<+== listen for master");
	Tag.DeactivateListener();
}