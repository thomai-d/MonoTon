#include "WaveTableSynth.h"
#include "../System/Utility.h"
#include "../Hardware/Seg7.h"

#define WAVETABLE_SIZE		256
static sample_t sineWaveTable[WAVETABLE_SIZE] = { 0, 3, 6, 9, 12, 16, 19, 22, 25, 28, 31, 34, 37, 40, 43, 46, 49, 51, 54, 57, 60, 63, 65, 68, 71, 73, 76, 78, 81, 83, 85, 88, 90, 92, 94, 96, 98, 100, 102, 104, 106, 107, 109, 111, 112, 113, 115, 116, 117, 118, 120, 121, 122, 122, 123, 124, 125, 125, 126, 126, 126, 127, 127, 127, 127, 127, 127, 127, 126, 126, 126, 125, 125, 124, 123, 122, 122, 121, 120, 118, 117, 116, 115, 113, 112, 111, 109, 107, 106, 104, 102, 100, 98, 96, 94, 92, 90, 88, 85, 83, 81, 78, 76, 73, 71, 68, 65, 63, 60, 57, 54, 51, 49, 46, 43, 40, 37, 34, 31, 28, 25, 22, 19, 16, 12, 9, 6, 3, 0, -3, -6, -9, -12, -16, -19, -22, -25, -28, -31, -34, -37, -40, -43, -46, -49, -51, -54, -57, -60, -63, -65, -68, -71, -73, -76, -78, -81, -83, -85, -88, -90, -92, -94, -96, -98, -100, -102, -104, -106, -107, -109, -111, -112, -113, -115, -116, -117, -118, -120, -121, -122, -122, -123, -124, -125, -125, -126, -126, -126, -127, -127, -127, -127, -127, -127, -127, -126, -126, -126, -125, -125, -124, -123, -122, -122, -121, -120, -118, -117, -116, -115, -113, -112, -111, -109, -107, -106, -104, -102, -100, -98, -96, -94, -92, -90, -88, -85, -83, -81, -78, -76, -73, -71, -68, -65, -63, -60, -57, -54, -51, -49, -46, -43, -40, -37, -34, -31, -28, -25, -22, -19, -16, -12, -9, -6, -3, };
static sample_t noiseWaveTable[WAVETABLE_SIZE] = { 9, 106, -43, 122, 72, 47, -84, 102, -120, -87, 36, 10, -63, 50, 99, -82, -73, 0, 34, 16, 121, -110, 96, -39, -84, -29, -99, 61, 31, -74, 78, 53, 82, -50, -67, 122, 123, -96, 109, 81, 74, 120, 20, 93, -24, 26, -24, -105, -52, 0, 84, -36, -74, -78, 121, -9, -119, 123, 78, -27, 18, -19, -58, 81, -39, 110, 55, 113, -12, 103, -79, -105, 17, 79, 69, -128, -3, -125, 111, 17, 121, -123, -16, -97, -32, -14, -2, 31, -45, 69, 112, 126, -48, 8, -65, -36, 49, 65, 3, -7, 98, -11, 33, -29, -50, 61, 73, -27, -20, 120, 121, -122, -116, -68, -22, 12, 124, -89, -16, 102, -17, -105, 30, 46, 118, -78, 69, -119, 16, -24, -50, 58, -2, -10, 46, 4, -30, -116, 80, 34, 40, -104, -77, -38, 74, 106, 114, -109, -99, 49, -33, 10, -47, 28, -119, 74, 86, 114, -20, -101, 118, 64, 83, -124, 42, -65, 7, 21, 13, 100, -87, 111, -42, 77, -66, -42, 99, -61, 98, -117, -66, 49, 106, 94, -120, 66, -23, 96, -20, 98, 45, -17, 30, 52, 43, 102, -60, -82, 46, 11, -30, -35, 5, -72, 7, -17, -76, 2, -34, 50, 101, 18, -113, -31, 92, -60, 85, 116, 122, 102, 84, 38, -88, 41, -73, 57, -47, 32, -95, -42, -110, 52, 20, -41, 40, 96, -84, 50, -41, 24, 14, 101, 1, 57, 123, -101, -33, -67, 87, -25, -111, -49, -78, 71, 53, 59, }; 

WaveTableSynth::WaveTableSynth(WaveTable waveTableType)
{
	this->SetParameter(0, 50);
	this->envelope.SetOutput(this, 10);
	
	switch (waveTableType)
	{
	case WaveTable::Sine:
		this->wavetable = sineWaveTable;
		break;

	case WaveTable::Noise:
		this->wavetable = noiseWaveTable;
		break;
	}
}

sample_t WaveTableSynth::NextSample()
{
	if (this->amplitude == 0)
		return 0;

	this->current += this->step;
	if (this->current >= ((uint32_t)WAVETABLE_SIZE << 16))
	{
		this->current = this->current - ((uint32_t)WAVETABLE_SIZE << 16);
	}

	uint8_t index = this->current >> 16;
	
	return (uint16_t)this->wavetable[index] * (this->amplitude + 1) / 256;
}

void WaveTableSynth::SetParameter(uint8_t parameter, uint8_t value)
{
	switch (parameter)
	{
	case 0:
		this->step = ((uint32_t)WAVETABLE_SIZE << 16) / SAMPLE_RATE * Utility::NoteToFrequency(value);
		break;

	case 1:
		this->envelope.SetR(20 + value * 10);
		break;

	case 10:
		this->amplitude = value;
		if (this->amplitude == 0)
			this->current = 0;
		break;
	}
}

void WaveTableSynth::Trigger()
{
	envelope.Trigger();
	envelope.Check();
}

void WaveTableSynth::ControlTick()
{
	envelope.Check();
}

uint32_t WaveTableSynth::GetDisplayValue(uint8_t param, uint8_t value)
{
	switch (param)
	{
	case 0:
		return Utility::NoteToDisplayValue(value);

	default: // TODO DECAY
		return MAKE_7SEG(DIGIT_DASH, DIGIT_DASH, DIGIT_DASH);
	}
}


