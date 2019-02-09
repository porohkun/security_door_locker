#pragma once
#include "BaseState.h";

class ListenForMasterState : public BaseState
{
	void Init();
	void Loop();
	void Exit();

private:
	bool _activated = false;
};