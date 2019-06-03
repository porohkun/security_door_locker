#include "StateManager.h"
#include "Defines.h"
#include "Button.h"
#include "Logger.h"

void StateManagerClass::Init()
{
	Logger.TextMessage(MESSAGE_LEVEL_DEBUG, "StateManagerClass::Init()");
	this->StartState(_currentState);
}

void StateManagerClass::Loop()
{
	if (_isTimeouting)
		_isTimeout = !_isTimeout && _timeout < millis();

	if (!_isWrongState)
		if (_currentState > _statesCount)
		{
			_isWrongState = true;
			Logger.TextMessage(MESSAGE_LEVEL_ERROR, "StateManagerClass::StopState() : Wrong state " + _currentState);
		}
		else
			_states[_currentState]->Loop();
}

void StateManagerClass::SwitchStateTo(byte state)
{
	byte oldState = _currentState;
	this->StopState();
	this->StartState(state);
	Logger.StateChangedMessage(oldState, state);
}


void StateManagerClass::StopState()
{
	if (_currentState > _statesCount)
	{
		_isWrongState = true;
		Logger.TextMessage(MESSAGE_LEVEL_ERROR, "StateManagerClass::StopState() : Wrong state " + _currentState);
	}
	else
	{
		_isWrongState = false;
		_states[_currentState]->Exit();
		Logger.ExitStateMessage(_currentState);
	}
}

void StateManagerClass::StartState(byte state)
{
	_currentState = state;
	_isTimeouting = false;

	if (_currentState > _statesCount)
	{
		_isWrongState = true;
		Logger.TextMessage(MESSAGE_LEVEL_ERROR, "StateManagerClass::StartState() : Wrong state " + _currentState);
	}
	else
	{
		_isWrongState = false;
		Logger.InitStateMessage(_currentState);
		_states[_currentState]->Init();
	}
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
