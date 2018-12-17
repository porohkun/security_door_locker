#include "Arduino.h"
#include "Toggle.h"

Toggle::Toggle(byte pin, void(*onSwitch)(Toggle* sender, bool on))
{
	pinMode(pin, INPUT_PULLUP);
	_pin = pin;
	this->onSwitch = onSwitch;
}

void Toggle::Read()
{
	_pinState = digitalRead(_pin);

	_nextState = _pinState == LOW;
	if (_state != _nextState)
	{
		_state = _nextState;
		if (this->onSwitch != NULL)
			this->onSwitch(this, _state);
	}
}
