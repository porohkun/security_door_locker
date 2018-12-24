#include "../StateManager.h"
#include "../Defines.h"
#include "../Button.h"
#include "../Tag.h"
#include "../Logger.h"


void listenForUnlockState()
{
	Serial.println("==+> listen for unlock");
	digitalWrite(LED_01, HIGH);
	digitalWrite(LED_02, LOW);
	digitalWrite(LED_04, LOW);
	digitalWrite(LED_08, LOW);

	Tag.ActivateListener();
}


void listenForUnlockStateLoop()
{
	digitalWrite(LED_16, HIGH);

	if (Button.GetUp(BTN_EXTERNAL))
		StateManager.SwitchStateTo(STATE_LOCKED);
	else if (Tag.HaveTag())
	{
		if (Tag.CurrentTagIsKnown())
		{
			Logger.LogUnlockWithTag(Tag.GetCurrentTag());
			StateManager.SwitchStateTo(STATE_UNLOCKING);
		}
		else
		{
			Logger.LogUnlockWrongTag(Tag.GetCurrentTag());
			StateManager.SwitchStateTo(STATE_LOCKED);
		}
		Tag.Stop();
	}
}


void listenForUnlockStateExit()
{
	Tag.DeactivateListener();
}
