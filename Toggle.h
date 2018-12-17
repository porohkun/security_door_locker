#pragma once
#include "Arduino.h"

class Toggle
{
public:
	Toggle(byte pin, void(*onSwitch)(Toggle* sender, bool on));
	void Read();
	bool IsOn() { return _state; }

private:
	byte _pin;
	byte _pinState;
	bool _state;
	bool _nextState;
	void(*onSwitch)(Toggle* sender, bool on);
};
