#include "Arduino.h"
#include "Sound.h"
#include "Defines.h"


void SoundClass::Initial(int pin)
{
	pinMode(BEEEPER, OUTPUT);
}

void SoundClass::PlayClosed()
{
	const unsigned int melody[2] = { NOTE_A7, NOTE_E7 };
	const unsigned int tempo[2] = { 8, 8 };
	Play(melody, tempo, 2);
}

void SoundClass::PlayOpened()
{
	const unsigned int melody[2] = { NOTE_E7, NOTE_A7 };
	const unsigned int tempo[2] = { 8, 8 };
	Play(melody, tempo, 2);
}

void SoundClass::PlayError()
{
	const unsigned int melody[3] = { NOTE_E7, 0, NOTE_E7 };
	const unsigned int tempo[3] = { 8, 8, 12 };
	Play(melody, tempo, 3);
}

void SoundClass::PlayOK()
{
	const unsigned int melody[3] = { NOTE_D7, NOTE_E7, NOTE_A7 };
	const unsigned int tempo[3] = { 12, 8, 8 };
	Play(melody, tempo, 3);
}

void SoundClass::PlayBeep()
{
	const unsigned int melody[1] = { NOTE_D7 };
	const unsigned int tempo[1] = { 12 };
	Play(melody, tempo, 1);
}

void SoundClass::Loop()
{
	if (_play)
	{
		unsigned long time = millis();
		if (time >= _nextStepTime)
			this->NextNote(_note + 1);
	}
}

void SoundClass::Play(const unsigned int* melody, const unsigned int* tempo, byte length)
{
	length = length < SOUND_LENGTH ? length : SOUND_LENGTH;
	for (byte i = 0; i < length; i++)
	{
		_melody[i] = melody[i];
		_tempo[i] = tempo[i];
	}
	_length = length;
	_nextStepTime = millis();
	this->NextNote(0);
}

void SoundClass::NextNote(byte index)
{
	_note = index;
	_play = _note < _length;
	if (_play)
	{
		unsigned int note = _melody[_note];
		_nextStepTime += _tempo[_note] * SOUND_SPEED;
		if (note == 0 )
			noTone(BEEEPER);
		else
			tone(BEEEPER, note);
	}
	else
		noTone(BEEEPER);
}

SoundClass Sound;