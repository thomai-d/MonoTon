#pragma once
#include "../Hardware/HAL.h"
#include "IInstrument.h"
#include "ISampleProvider.h"

class Sampler : public IInstrument
{
public:
	Sampler(uint8_t sample);
	virtual sample_t NextSample() override;
	virtual void SetParameter(uint8_t parameter, uint8_t value) override;
	virtual void Trigger() override;
	virtual void ControlTick() override;
	virtual uint8_t GetDefaultParameterValue(uint8_t param) override { return 63; };

	virtual uint32_t GetDisplayValue(uint8_t param, uint8_t value) override;

private:
	uint16_t currentSample = (uint16_t)-1;

	uint8_t velocity = 127;

	const sample_t *sampleAddr;
	uint16_t sampleCount;
};