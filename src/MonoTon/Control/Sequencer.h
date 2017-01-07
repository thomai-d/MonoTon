#pragma once
#include "../Configuration.h"
#include "../Hardware/HAL.h"

class Sequencer
{
public:
	void Initialize();

	void Tick16();

	void SetSteps(uint8_t channel, uint8_t bank, uint16_t steps) { this->banks[channel][bank] = steps; }
	uint16_t GetSteps(uint8_t channel, uint8_t bank) { return this->banks[channel][bank]; }

	void SetCurrentBank(uint8_t channel, uint8_t bank) { this->currentBank[channel] = bank; }
	uint8_t GetCurrentBank(uint8_t channel) { return this->currentBank[channel]; }

	uint8_t GetNextStep() { return this->step; }
	void SetParameter(uint8_t channel, uint8_t bank, uint8_t step, uint8_t parameter, uint8_t value) { this->parameters[channel][bank][step][parameter] = value; }

	uint8_t GetMuteMask() { return this->muteMask; }
	void SetMuteMask(uint8_t mask) { this->muteMask = mask; }

private:

	uint8_t step = 0;
	uint8_t muteMask = 0xff;
	uint16_t banks[MAX_CHANNELS][MAX_BANKS];
	uint8_t parameters[MAX_CHANNELS][MAX_BANKS][MAX_STEPS][MAX_PARAMETERS];
	uint8_t currentBank[MAX_CHANNELS];
};

