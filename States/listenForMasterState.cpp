#include "../StateManager.h"
#include "../Defines.h"
#include "../Button.h"
#include "../Tag.h"
#include "../Logger.h"


void listenForMasterState()
{
	Serial.println("==+> listen for master");
	digitalWrite(LED_01, HIGH);
	digitalWrite(LED_02, HIGH);
	digitalWrite(LED_04, HIGH);
	digitalWrite(LED_08, LOW);

	Tag.ActivateListener();
	StateManager.StartTimeout(10000);
}


void listenForMasterStateLoop()
{
	digitalWrite(LED_16, HIGH);

	if (Button.GetDown(REED_SWITCH))
		StateManager.SwitchStateTo(STATE_CLOSED);
	else if (StateManager.IsTimeout())
		StateManager.SwitchStateTo(STATE_OPENED);
	else if (Tag.HaveTag())
	{
		if (Tag.HaveMasterTag())
			if (Tag.CurrentTagIsMaster())
				StateManager.SwitchStateTo(STATE_LISTEN_FOR_EMPTY);
			else
				StateManager.SwitchStateTo(STATE_OPENED);
		else
		{
			Tag.SaveCurrentTagAsMaster();
			StateManager.SwitchStateTo(STATE_OPENED);
		}
		Tag.Stop();
	}
}


void listenForMasterStateExit()
{
	Tag.DeactivateListener();
}
