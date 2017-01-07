#pragma once
#include "../Hardware/HAL.h"
class ITriggerable
{
public:
	virtual void Trigger() = 0;
};

