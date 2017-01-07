#ifndef AUDIOOUT_H
#define AUDIOOUT_H
#include "../Hardware/HAL.h"
#include "../Configuration.h"

class AudioOut
{
public:
	AudioOut();

	void SampleOut();
	void Enqueue(sample_t sample);
	uint8_t SpaceLeft();

private:
	volatile sample_t samples[256];
	volatile uint8_t currentReadIndex = 0;
	volatile uint8_t count = 0;
};

extern AudioOut audioOut;

#endif