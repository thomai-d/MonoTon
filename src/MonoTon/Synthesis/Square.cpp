#include "Square.h"
#include "../System/Utility.h"
#include "../Hardware/Seg7.h"

Square::Square()
{
	this->SetParameter(SquareParam::Frequency, 1000);
	this->envelope.SetOutput(this, SquareParam::AmplitudeInternal);
}

sample_t Square::NextSample()
{
	if (this->halfAmplitude == 0)
		return 0;

	bool value = this->currentValue;

	this->currentSample++;
	uint16_t threshold = value ? this->positivePulse : this->negativePulse;
	if (this->currentSample >= threshold)
	{
		this->currentSample = 0;
		this->currentValue = !this->currentValue;
	}

	// todo temp
	return (value ? this->halfAmplitude : -this->halfAmplitude) / 2;
}

void Square::SetParameter(uint8_t parameter, uint8_t value)
{
	switch (parameter)
	{
	case SquareParam::Frequency:
		{
			uint16_t freq = Utility::NoteToFrequency(value);
			uint16_t fullCycle = SAMPLE_RATE / freq;
			this->positivePulse = fullCycle / 2;
			this->negativePulse = fullCycle - this->positivePulse;
			break;
		}
	case SquareParam::Decay:
		this->envelope.SetR(20 + value * 10);
		break;

	case SquareParam::AmplitudeInternal:
		this->halfAmplitude = value / 2;
		if (this->halfAmplitude == 0)
		{
			this->currentValue = 0;
			this->currentSample = 0;
		}
		break;
	}
}

void Square::Trigger()
{
	envelope.Trigger();
	envelope.Check();
}

void Square::ControlTick()
{
	envelope.Check();
}

uint32_t Square::GetDisplayValue(uint8_t param, uint8_t value)
{
	switch (param)
	{
	case SquareParam::Frequency:
		return Utility::NoteToDisplayValue(value);

	default: // TODO DECAY
		return MAKE_7SEG(DIGIT_DASH, DIGIT_DASH, DIGIT_DASH);
	}
}
