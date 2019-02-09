#pragma once
#include "BaseState.h";

class UnlockingState : public BaseState
{
	void Init();
	void Loop();
	void Exit();
};