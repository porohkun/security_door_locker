#include "Arduino.h"
#include "Button.h"

void ButtonClass::SetButtons(byte *pins, byte length)
{
	_length = length;
	_pins = pins;
	_states = new bool[_length];
	_prevStates = new bool[_length];
	for (byte i = 0; i < _length; i++)
		pinMode(_pins[i], INPUT_PULLUP);
}

void ButtonClass::Loop()
{
	for (byte i = 0; i < _length; i++)
	{
		_prevStates[i] = _states[i];
		_states[i] = digitalRead(_pins[i]) == LOW;
	}
}

bool ButtonClass::GetDown(byte pin)
{
	byte i = this->GetIndex(pin);
	if (i == 255)
		return false;
	return _states[i] && !_prevStates[i];
}

bool ButtonClass::GetUp(byte pin)
{
	byte i = this->GetIndex(pin);
	if (i == 255)
		return false;
	return !_states[i] && _prevStates[i];
}

bool ButtonClass::GetState(byte pin)
{
	byte i = this->GetIndex(pin);
	if (i == 255)
		return false;
	return _states[i];
}

byte ButtonClass::GetIndex(byte pin)
{
	for (byte i = 0; i < _length; i++)
		if (_pins[i] == pin)
			return i;
	return 255;
}

ButtonClass Button;