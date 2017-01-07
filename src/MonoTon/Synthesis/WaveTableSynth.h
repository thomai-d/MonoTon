#pragma once
#include "../Configuration.h"
#include "../Hardware/HAL.h"
#include "ISampleProvider.h"
#include "IInstrument.h"
#include "../Control/REnvelope.h"

enum WaveTable
{
	Sine,
	Noise
};

class WaveTableSynth : public IInstrument
{
public:
	WaveTableSynth(WaveTable waveTableType);

	virtual sample_t NextSample() override;

	virtual void SetParameter(uint8_t parameter, uint8_t value) override;
	virtual void Trigger() override;
	virtual void ControlTick() override;
	virtual uint8_t GetDefaultParameterValue(uint8_t param) { return param == 1 ? 10 : 0; }

	virtual uint32_t GetDisplayValue(uint8_t param, uint8_t value) override;

private:
	uint16_t lambda = 0;
	uint32_t step = 0;
	uint32_t current = 0;
	uint8_t amplitude = 0;

	sample_t* wavetable;
	REnvelope envelope;
};