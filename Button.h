#include "Arduino.h"

class ButtonClass
{
public:
	void SetButtons(byte *pins, byte length);
	void Loop();
	bool GetDown(byte pin);
	bool GetUp(byte pin);
	bool GetState(byte pin);

private:
	byte GetIndex(byte pin);
	byte _length;
	byte *_pins;
	bool *_states;
	bool *_prevStates;
};

extern ButtonClass Button;
