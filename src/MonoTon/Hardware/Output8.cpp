#include "Output8.h"

void Output8::Initialize(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7)
{
	this->pins[0] = b0;
	this->pins[1] = b1;
	this->pins[2] = b2;
	this->pins[3] = b3;
	this->pins[4] = b4;
	this->pins[5] = b5;
	this->pins[6] = b6;
	this->pins[7] = b7;
	
	for (uint8_t pinIndex = 0; pinIndex < 8; pinIndex++)
		pinMode(this->pins[pinIndex], OUTPUT);
}

void Output8::Set(uint8_t value)
{
	for (uint8_t pinIndex = 0; pinIndex < 8; pinIndex++)
	{
		digitalWrite(this->pins[pinIndex], bitRead(value, pinIndex));
	}
}

void Output8::SetProgress(uint8_t value)
{
	for (int n = 0; n < 8; n++)
	{
		digitalWrite(this->pins[n], value > (32 * n));
	}
}

void Output8::Animate()
{
	this->Set(0x00);
	delay(100);

	uint8_t value = 1;
	for (uint8_t i = 0; i < 7; i++)
	{
		this->Set(value);
		delay(100);
		value <<= 1;
	}
	
	for (uint8_t i = 0; i < 7; i++)
	{
		this->Set(value);
		delay(100);
		value >>= 1;
	}
	
	this->Set(0x01);
	delay(100);
	this->Set(0x00);
}
