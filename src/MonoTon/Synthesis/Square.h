#pragma once
#include "../Hardware/HAL.h"
#include "../Configuration.h"
#include "ISampleProvider.h"
#include "IInstrument.h"
#include "../Control/REnvelope.h"

enum SquareParam
{
	Frequency = 0,
	Decay = 1,

	AmplitudeInternal = 9 
};

class Square : public IInstrument
{
public:
	Square();

	virtual sample_t NextSample() override;

	virtual void SetParameter(uint8_t parameter, uint8_t value) override;
	virtual void Trigger() override;
	virtual void ControlTick() override;

	virtual uint8_t GetDefaultParameterValue(uint8_t param) { return param == 1 ? 10 : 0; }

	virtual uint32_t GetDisplayValue(uint8_t param, uint8_t value) override;

private:
	uint16_t positivePulse = 0;
	uint16_t negativePulse = 0;
	uint16_t currentSample = 0;
	bool currentValue = false;
	sample_t halfAmplitude = 0;

	REnvelope envelope;
};
