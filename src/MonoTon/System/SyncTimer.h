#pragma once
#include "../Configuration.h"
#include "../Hardware/HAL.h"
#include "../System/Macros.h"

class SyncTimer
{
public:
	void Start(uint16_t intervalMs, void (*callback)());
	void Reset(uint16_t intervalMs);
	void SetInterval(uint16_t intervalMs) { this->interval = intervalMs; }
	void Check();

private:
	void(*callback)();
	uint32_t lastCheck;
	uint16_t interval;
};

