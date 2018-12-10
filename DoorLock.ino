/*
	Name:       DoorLock.ino
	Created:	11.12.2018 0:34:21
	Author:     POROH-PC\P
*/

#define BACKLIGHT_ON_MESSAGE 0
#define BACKLIGHT_OFF_MESSAGE 1

//#include <EEPROM.h>
#include "Toggle.h"
#include "Button.h"
#include "AdvancedSerial.h"

Button btn1(2, btnClick);
Button btn2(3, btnClick);
Button btn3(4, btnClick);
Button btn4(5, btnClick);
Toggle toggle(6, NULL);

const int ledPin = 13;
bool led = false;

void setup()
{
	pinMode(ledPin, OUTPUT);
	Serial.begin(115200);
	//configure receiver callback
	AdvancedSerial.setReceiver(onMessage);
}

void btnClick(Button* sender)
{
	//Serial.println(count, DEC);
	led = !led;
}

void onMessage(AdvancedSerialMessage* message)
{
	switch (message->id)
	{
	case BACKLIGHT_ON_MESSAGE:
		digitalWrite(ledPin, HIGH);
		break;

	case BACKLIGHT_OFF_MESSAGE:
		digitalWrite(ledPin, LOW);
		break;
	}
}

void loop()
{
	btn1.Read();
	btn2.Read();
	btn3.Read();
	btn4.Read();
	toggle.Read();
	AdvancedSerial.loop();

	/*digitalWrite(ledPin, led ? HIGH : LOW);
	digitalWrite(ledPin, toggle.IsOn() ? HIGH : LOW);*/
}
