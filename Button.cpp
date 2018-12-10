#include "Arduino.h"
#include "Button.h"

Button::Button(int pin, void(*onClick)(Button* sender))
{
	pinMode(pin, INPUT_PULLUP);
	_pin = pin;
	this->onClick = onClick;
}

void Button::Read()
{
	_pinState = digitalRead(_pin);

	if (_pinState == HIGH)
	{
		if (_state != _nextState)
		{
			_state = _nextState;
			if (this->onClick != NULL)
				this->onClick(this);
		}
	}
	else
	{
		_nextState = !_state;
	}
}
