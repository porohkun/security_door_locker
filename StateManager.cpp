#include "StateManager.h"
#include "Defines.h"
#include "Button.h"

void StateManagerClass::Init()
{
	byte buttons[] = { BTN_INTERNAL, BTN_EXTERNAL, BTN_NFC, BTN_LOCKED, REED_SWITCH };
	Button.SetButtons(buttons);
	this->SwitchStateTo(_currentState);
}

void StateManagerClass::Loop()
{
	if (_isTimeouting)
		_isTimeout = !_isTimeout && _timeout < millis();

	switch (_currentState)
	{
	case STATE_LOCKED:
		lockedStateLoop();
		break;
	case STATE_LISTEN_FOR_UNLOCK:
		listenForUnlockStateLoop();
		break;
	case STATE_UNLOCKING:
		unlockingStateLoop();
		break;
	case STATE_UNLOCKED:
		unlockedStateLoop();
		break;
	case STATE_OPENED:
		openedStateLoop();
		break;
	case STATE_CLOSED:
		closedStateLoop();
		break;
	case STATE_LOCKING:
		lockingStateLoop();
		break;
	case STATE_LISTEN_FOR_MASTER:
		listenForMasterStateLoop();
		break;
	case STATE_LISTEN_FOR_EMPTY:
		listenForEmptyStateLoop();
		break;
	case STATE_SAVE_NEW_TAG:
		saveNewTagStateLoop();
		break;
	default:
		break;
	}
	//digitalWrite(LED_08, HIGH);
}

void StateManagerClass::SwitchStateTo(byte state)
{
	switch (_currentState)
	{
	case STATE_LOCKED:
		lockedStateExit();
		break;
	case STATE_LISTEN_FOR_UNLOCK:
		listenForUnlockStateExit();
		break;
	case STATE_UNLOCKING:
		unlockingStateExit();
		break;
	case STATE_UNLOCKED:
		unlockedStateExit();
		break;
	case STATE_OPENED:
		openedStateExit();
		break;
	case STATE_CLOSED:
		closedStateExit();
		break;
	case STATE_LOCKING:
		lockingStateExit();
		break;
	case STATE_LISTEN_FOR_MASTER:
		listenForMasterStateExit();
		break;
	case STATE_LISTEN_FOR_EMPTY:
		listenForEmptyStateExit();
		break;
	case STATE_SAVE_NEW_TAG:
		saveNewTagStateExit();
		break;
	default:
		break;
	}

	_currentState = state;
	_isTimeouting = false;

	switch (_currentState)
	{
	case STATE_LOCKED:
		lockedState();
		break;
	case STATE_LISTEN_FOR_UNLOCK:
		listenForUnlockState();
		break;
	case STATE_UNLOCKING:
		unlockingState();
		break;
	case STATE_UNLOCKED:
		unlockedState();
		break;
	case STATE_OPENED:
		openedState();
		break;
	case STATE_CLOSED:
		closedState();
		break;
	case STATE_LOCKING:
		lockingState();
		break;
	case STATE_LISTEN_FOR_MASTER:
		listenForMasterState();
		break;
	case STATE_LISTEN_FOR_EMPTY:
		listenForEmptyState();
		break;
	case STATE_SAVE_NEW_TAG:
		saveNewTagState();
		break;
	default:
		break;
	}
}

void StateManagerClass::StartTimeout(unsigned long length)
{
	_timeout = millis() + length;
	_isTimeouting = true;
}

bool StateManagerClass::IsTimeout()
{
	return _isTimeouting && _isTimeout;
}

StateManagerClass StateManager;
