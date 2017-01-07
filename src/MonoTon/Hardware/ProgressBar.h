#pragma once
#include "../Hardware/HAL.h"

#include "Arduino.h"

class ProgressBar
{
public:
	void Initialize(byte pin0, byte pin1, byte pin2, byte pin3, byte pin4, byte pin5, byte pin6, byte pin7);
	void SetRaw(byte value);
	void SetProgress(byte value);
	void Animate();

private:
	byte pins[8];

};
