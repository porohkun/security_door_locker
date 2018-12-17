#pragma once
#include "Arduino.h"

class StateManagerClass
{
public:
	void Init();
	void Loop();
	void SwitchStateTo(byte state);
	void StartTimeout(unsigned long length);
	bool IsTimeout();

private:
	byte _currentState;
	unsigned long _timeout;
	bool _isTimeouting;
	bool _isTimeout;
};

extern StateManagerClass StateManager;
