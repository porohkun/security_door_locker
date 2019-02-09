#pragma once
#include "BaseState.h";

class ListenForEmptyState : public BaseState
{
	void Init();
	void Loop();
	void Exit();

private:
	bool _activated = false;
};