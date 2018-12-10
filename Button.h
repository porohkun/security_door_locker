#pragma once
#include "Arduino.h"

class Button
{
public:
	Button(int pin, void(*onClick)(Button* sender));
	void Read();

private:
	int _pin;
	int _pinState;
	bool _state;
	bool _nextState;
	void(*onClick)(Button* sender);
};
