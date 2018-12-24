#include "Arduino.h"
#include "Flags.h"

bool HaveFlag(byte value, byte flag)
{
	return (value & flag) == flag;
}

byte AddFlag(byte value, byte flag)
{
	return value | flag;
}

byte RemFlag(byte value, byte flag)
{
	return value & ~flag;
}