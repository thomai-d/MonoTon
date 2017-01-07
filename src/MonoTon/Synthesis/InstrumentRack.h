#ifndef INSTRUMENTRACK_H
#define INSTRUMENTRACK_H
#include "IInstrument.h"
#include "../Configuration.h"

enum Instrument
{
	SINE,
	SQUARE,
	NOISE,
	KICK,
	HAT,
	SNARE,
	SHAKER
};

class InstrumentRack
{
public:
	InstrumentRack();

	void Trigger(uint8_t channel);
	void SetParameter(uint8_t channel, uint8_t parameter, uint8_t value);

	void ProcessSamples();
	void ControlTick();

	uint8_t GetDefaultParameterValue(uint8_t channel, uint8_t parameter) { return this->instrument[channel]->GetDefaultParameterValue(parameter); }
	uint32_t GetParameterDisplayValue(uint8_t channel, uint8_t parameter, uint8_t value) { return this->instrument[channel]->GetDisplayValue(parameter, value); }

private:
	IInstrument* instrument[MAX_CHANNELS];

};

extern InstrumentRack instrumentRack;

#endif