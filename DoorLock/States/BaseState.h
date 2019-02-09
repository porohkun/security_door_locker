#pragma once
#include "Arduino.h"
//#include "StateManager.h"

class BaseState
{
public:
	inline BaseState() { };
	inline virtual void Init() { Serial.println("!!! BaseState Init();"); };
	inline virtual void Loop() { Serial.println("!!! BaseState Loop();"); };
	inline virtual void Exit() { Serial.println("!!! BaseState Exit();"); };

private:
	//StateManager* _stateManager;
};

