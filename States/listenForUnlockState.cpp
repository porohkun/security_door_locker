#include "../StateManager.h"
#include "../Defines.h"
#include "../Button.h"


void listenForUnlockState()
{
	Serial.println("==+> listen for unlock");
	digitalWrite(LED_01, HIGH);
	digitalWrite(LED_02, LOW);
	digitalWrite(LED_04, LOW);
	digitalWrite(LED_08, LOW);
}


void listenForUnlockStateLoop()
{
	digitalWrite(LED_16, HIGH);

	if (Button.GetUp(BTN_EXTERNAL))
		StateManager.SwitchStateTo(STATE_LOCKED);
	//TODO: nfc
	else if (Button.GetUp(BTN_NFC))
		StateManager.SwitchStateTo(STATE_UNLOCKING);
	//TODO: nfc
	//else if (/*nfc_invalid*/)
	//	StateManager.SwitchStateTo(STATE_LOCKED);
}
