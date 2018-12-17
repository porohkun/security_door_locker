#include "../StateManager.h"
#include "../Defines.h"
#include "../Button.h"


void listenForMasterState()
{
	Serial.println("==+> listen for master");
	digitalWrite(LED_01, HIGH);
	digitalWrite(LED_02, HIGH);
	digitalWrite(LED_04, HIGH);
	digitalWrite(LED_08, LOW);

	StateManager.StartTimeout(10000);
}


void listenForMasterStateLoop()
{
	digitalWrite(LED_16, HIGH);

	if (Button.GetDown(REED_SWITCH))
		StateManager.SwitchStateTo(STATE_CLOSED);
	else if (StateManager.IsTimeout())
		StateManager.SwitchStateTo(STATE_OPENED);
	//TODO: nfc
	else if (Button.GetUp(BTN_NFC))
		StateManager.SwitchStateTo(STATE_LISTEN_FOR_EMPTY);
	//TODO: nfc
	//else if (/*nfc_invalid*/)
	//	StateManager.SwitchStateTo(STATE_OPENED);
}
