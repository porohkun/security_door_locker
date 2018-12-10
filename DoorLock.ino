/*
    Name:       DoorLock.ino
    Created:	11.12.2018 0:34:21
    Author:     POROH-PC\P
*/


//#include <EEPROM.h>
#include "Toggle.h"
#include "Button.h"

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
	//Serial.begin(115200);
}

void btnClick(Button* sender)
{
	//Serial.println(count, DEC);
	led = !led;
}

void loop()
{
	btn1.Read();
	btn2.Read();
	btn3.Read();
	btn4.Read();
	toggle.Read();

	digitalWrite(ledPin, led ? HIGH : LOW);
	digitalWrite(ledPin, toggle.IsOn() ? HIGH : LOW);
}
