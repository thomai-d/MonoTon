#include "SyncTimer.h"

void SyncTimer::Start(uint16_t intervalMs, void(*callback)())
{
	this->callback = callback;
	this->Reset(intervalMs);
}

void SyncTimer::Reset(uint16_t intervalMs)
{
	this->lastCheck = millis();
	this->interval = intervalMs;
}

void SyncTimer::Check()
{
	auto now = millis();
	uint16_t delta = TIME_DELTA_MS(now, this->lastCheck);

	if (delta >= this->interval)
	{
		this->lastCheck = this->lastCheck + this->interval;
		this->callback();
	}
}
