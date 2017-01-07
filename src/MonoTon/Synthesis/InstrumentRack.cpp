#include "InstrumentRack.h"
#include "..\Hardware\HAL.h"
#include "..\Synthesis\Sampler.h"
#include "..\Synthesis\WaveTableSynth.h"
#include "..\Synthesis\Square.h"
#include "..\Hardware\AudioOut.h"

InstrumentRack::InstrumentRack()
{
	this->instrument[0] = new Sampler(0);
	this->instrument[1] = new Sampler(1);
	this->instrument[2] = new Sampler(2);
	this->instrument[3] = new Sampler(3);
	this->instrument[4] = new Sampler(4);
	this->instrument[5] = new WaveTableSynth(WaveTable::Noise);
	this->instrument[6] = new WaveTableSynth(WaveTable::Sine);
	this->instrument[7] = new Square();
}

void InstrumentRack::Trigger(uint8_t channel)
{
	this->instrument[channel]->Trigger();
}

void InstrumentRack::SetParameter(uint8_t channel, uint8_t parameter, uint8_t value)
{
	this->instrument[channel]->SetParameter(parameter, value);
}

void InstrumentRack::ProcessSamples()
{
	uint8_t samples = audioOut.SpaceLeft();
	if (samples != 255 && samples > 200) Serial.println("BUFFER LOW");
	for (uint8_t sample = 0; sample < samples; sample++)
	{
		audioOut.Enqueue(
		(this->instrument[0]->NextSample() + this->instrument[2]->NextSample()) / 8 +
		(this->instrument[3]->NextSample() + this->instrument[1]->NextSample()) / 4 +
		(this->instrument[4]->NextSample() + this->instrument[5]->NextSample()) / 16 +
		(this->instrument[6]->NextSample() + this->instrument[7]->NextSample()) / 8
		);
	}
}

void InstrumentRack::ControlTick()
{
	for (uint8_t i = 0; i < 8; i++)
		this->instrument[i]->ControlTick();
}

InstrumentRack instrumentRack;