#include "../StateManager.h"
#include "../Defines.h"
#include "../Button.h"


void listenForEmptyState()
{
	Serial.println("==+> listen for empty");
	digitalWrite(LED_01, LOW);
	digitalWrite(LED_02, LOW);
	digitalWrite(LED_04, LOW);
	digitalWrite(LED_08, HIGH);

	StateManager.StartTimeout(10000);
}


void listenForEmptyStateLoop()
{
	digitalWrite(LED_16, HIGH);

	if (Button.GetDown(REED_SWITCH))
		StateManager.SwitchStateTo(STATE_CLOSED);
	else if (StateManager.IsTimeout())
		StateManager.SwitchStateTo(STATE_OPENED);
	//TODO: nfc
	else if (Button.GetUp(BTN_NFC))
		StateManager.SwitchStateTo(STATE_SAVE_NEW_TAG);
	//TODO: nfc
	//else if (/*nfc_invalid*/)
	//	StateManager.SwitchStateTo(STATE_OPENED);
}
