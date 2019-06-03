#pragma once
#include "Arduino.h"
//#include "Logger.h"

class BaseState
{
public:
	inline BaseState() { };
	inline virtual void Init() {/* Logger.TextMessage(MESSAGE_LEVEL_WARNING, "!!! BaseState Init();");*/ };
	inline virtual void Loop() { /*Logger.TextMessage(MESSAGE_LEVEL_WARNING, "!!! BaseState Loop();");*/ };
	inline virtual void Exit() { /*Logger.TextMessage(MESSAGE_LEVEL_WARNING, "!!! BaseState Exit();");*/ };

private:
	//StateManager* _stateManager;
};

