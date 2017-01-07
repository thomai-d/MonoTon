#include "ProgressBar.h"

void ProgressBar::Initialize(byte pin0, byte pin1, byte pin2, byte pin3, byte pin4, byte pin5, byte pin6, byte pin7)
{
	this->pins[0] = pin0;
	this->pins[1] = pin1;
	this->pins[2] = pin2;
	this->pins[3] = pin3;
	this->pins[4] = pin4;
	this->pins[5] = pin5;
	this->pins[6] = pin6;
	this->pins[7] = pin7;
	
	for (int n = 0; n < 8; n++)
		pinMode(this->pins[n], OUTPUT);
}

void ProgressBar::SetRaw(byte value)
{
	for (int n = 0; n < 8; n++)
		digitalWrite(this->pins[n], bitRead(value, n));
}


void ProgressBar::Animate()
{
	for (int n = 0; n < 8; n++)
	{
		this->SetRaw(1 << n);
		delay(100);
	}

	this->SetRaw(0);
}
