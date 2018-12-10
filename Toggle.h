#pragma once
#include "Arduino.h"

class Toggle
{
public:
	Toggle(int pin, void(*onSwitch)(Toggle* sender, bool on));
	void Read();
	bool IsOn() { return _state; }

private:
	int _pin;
	int _pinState;
	bool _state;
	bool _nextState;
	void(*onSwitch)(Toggle* sender, bool on);
};
