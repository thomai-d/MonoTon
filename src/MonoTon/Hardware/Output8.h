#pragma once
#include "../Hardware/HAL.h"

class Output8
{
public:
	void Initialize(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7);
	void Set(uint8_t value);
	void SetProgress(uint8_t value);
	void Animate();

private:
	uint8_t pins[8];
};
