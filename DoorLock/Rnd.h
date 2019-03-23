#pragma once
#include "Arduino.h"

class RndClass
{
public:
	void SetSeed(int seed)
	{
		srand(seed);
		_seed = seed;
	}

	void SetSeed(byte index, unsigned long seed)
	{
		_contexts[index] = seed;
	}

	int GetSeed()
	{
		return _seed;
	}

	unsigned long GetSeed(byte index)
	{
		return _contexts[index];
	}

	int Next()
	{
		return rand();
	}

	int Next(int min, int max)
	{
		if (min >= max - 1)
			return min;
		return rand() % (max - min) + min;
	}

	int Next(byte index)
	{
		return rand_r(_contexts_ptr + index);
	}

	int Next(byte index, int min, int max)
	{
		if (min >= max - 1)
			return min;
		return rand_r(_contexts_ptr + index) % (max - min) + min;
	}

private:
	int _seed;
	unsigned long _contexts[2] = { 0, 0 };
	unsigned long* _contexts_ptr = _contexts;
};

extern RndClass Rnd;