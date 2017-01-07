#include "AudioOut.h"

AudioOut::AudioOut()
{
	DDRC= 0xff;
}

void AudioOut::SampleOut()
{
	if (this->count == 0)
	{
		PORTC = this->samples[(uint8_t)(this->currentReadIndex - 1)] + 127;
		return;
	}

	sample_t sample = this->samples[this->currentReadIndex];
	this->currentReadIndex++;
	this->count--;

	PORTC = (signed char)sample + 127;
}

void AudioOut::Enqueue(sample_t sample)
{
	if (this->count == 255)
	{
		// Buffer is full.
		return;
	}

	DISABLE_INTERRUPTS;
	uint8_t nextWriteIndex = this->currentReadIndex + this->count;
	this->count++;
	ENABLE_INTERRUPTS;

	this->samples[nextWriteIndex] = sample;
}

uint8_t AudioOut::SpaceLeft()
{
	DISABLE_INTERRUPTS;
	uint8_t currentCount = this->count;
	ENABLE_INTERRUPTS;

	return 255 - currentCount;
}

AudioOut audioOut;