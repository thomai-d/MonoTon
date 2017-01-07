#include "REnvelope.h"
#include "../System/Macros.h"

void REnvelope::SetOutput(IInstrument * output, uint8_t parameter)
{
	this->output = output;
	this->parameter = parameter;
}

void REnvelope::Check()
{
	if (this->output == 0 || this->current == 0) return;

	uint8_t value = (uint8_t)(this->current >> 16);
	this->output->SetParameter(this->parameter, value);

	this->current -= this->step;
	if (this->current < this->step)
	{
		this->output->SetParameter(this->parameter, 0);
		this->current = 0;
	}
}

void REnvelope::Trigger()
{
	this->current = (uint32_t)255 << 16;
}

void REnvelope::SetR(uint16_t rMs)
{
	if (rMs < CONTROL_RATE_MS * 4)
	{
		rMs = CONTROL_RATE_MS * 4;
	}

	this->step = (((uint32_t)255 << 16) * CONTROL_RATE_MS) / rMs;
}
