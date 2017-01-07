#pragma once
#include "../Hardware/HAL.h"

class ISampleProvider
{
public:
	virtual sample_t NextSample() = 0;
};
