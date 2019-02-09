#pragma once
#include "BaseState.h";

class OpenedState : public BaseState
{
	void Init();
	void Loop();
	void Exit();
};