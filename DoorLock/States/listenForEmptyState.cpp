#include "../StateManager.h"
#include "../Defines.h"
#include "../Button.h"
#include "../Tag.h"
#include "../Logger.h"


void listenForEmptyState()
{
	Serial.println("==+> listen for empty");
	digitalWrite(LED_01, LOW);
	digitalWrite(LED_02, LOW);
	digitalWrite(LED_04, LOW);
	digitalWrite(LED_08, HIGH);

	Tag.ActivateListener();
	StateManager.StartTimeout(10000);
}


void listenForEmptyStateLoop()
{
	digitalWrite(LED_16, HIGH);

	if (Button.GetDown(REED_SWITCH))
		StateManager.SwitchStateTo(STATE_CLOSED);
	else if (StateManager.IsTimeout())
		StateManager.SwitchStateTo(STATE_OPENED);
	else if (Tag.HaveTag() && !Tag.CurrentTagIsMaster() && !Tag.CurrentTagIsKnown())
	{
		Tag.SaveCurrentTag();
		StateManager.SwitchStateTo(STATE_OPENED);
		Tag.Stop();
	}
}


void listenForEmptyStateExit()
{
	Tag.DeactivateListener();
}
