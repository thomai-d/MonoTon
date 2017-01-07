#pragma once
#include "../Hardware/HAL.h"

class Stopwatch
{
public:
	void Start();
	void Event(char* str);

private:
	uint32_t lastTime;
};
