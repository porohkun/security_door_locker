#pragma once
#include "Arduino.h"
#include "States\BaseState.h"
#include "States\LockedState.h"
#include "States\UnlockingState.h"
#include "States\OpenedState.h"
#include "States\ListenForMasterState.h"
#include "States\ListenForEmptyState.h"

class StateManagerClass
{
public:
	void Init();
	void Loop();
	void SwitchStateTo(byte state);
	void StartTimeout(unsigned long length);
	bool IsTimeout();

private:
	void StopState();
	void StartState(byte state);

	BaseState* _states[5] = {/*nullptr, nullptr, nullptr, nullptr, nullptr };//*/ new LockedState(), new UnlockingState(), new OpenedState(), new ListenForMasterState(), new ListenForEmptyState()};
	byte _statesCount = 5;
	byte _currentState;
	unsigned long _timeout;
	bool _isTimeouting;
	bool _isTimeout;
};

extern StateManagerClass StateManager;
