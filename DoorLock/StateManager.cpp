#include "StateManager.h"
#include "Defines.h"
#include "Button.h"

void StateManagerClass::Init()
{
	Serial.println("StateManagerClass::Init()");
	byte buttons[] = { BTN_INTERNAL, OPENED_STATUS, 0xff, 0xff, 0xff };
	Button.SetButtons(buttons);
	this->StartState(_currentState);
}

void StateManagerClass::Loop()
{
	if (_isTimeouting)
		_isTimeout = !_isTimeout && _timeout < millis();

	if (_currentState > _statesCount)
		Serial.println("!!! Wrong state");
	else
		_states[_currentState]->Loop();
}

void StateManagerClass::SwitchStateTo(byte state)
{
	//Serial.print("StateManagerClass::SwitchStateTo ");
	//Serial.println(state);

	this->StopState();
	this->StartState(state);
}


void StateManagerClass::StopState()
{
	//Serial.println("StateManagerClass::StopState()");

	if (_currentState > _statesCount)
		Serial.println("!!! Wrong state");
	else
		_states[_currentState]->Exit();
}

void StateManagerClass::StartState(byte state)
{
	//Serial.print("StateManagerClass::StartState ");
	//Serial.println(state);

	_currentState = state;
	_isTimeouting = false;

	if (_currentState > _statesCount)
		Serial.println("!!! Wrong state");
	else
		_states[_currentState]->Init();
}

void StateManagerClass::StartTimeout(unsigned long length)
{
	_timeout = millis() + length;
	_isTimeouting = true;
	_isTimeout = false;
}

bool StateManagerClass::IsTimeout()
{
	return _isTimeouting && _isTimeout;
}

StateManagerClass StateManager;
