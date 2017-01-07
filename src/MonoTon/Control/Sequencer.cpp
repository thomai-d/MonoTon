#include "Sequencer.h"
#include "../Synthesis/InstrumentRack.h"

void Sequencer::Initialize()
{
	for (uint8_t instr = 0; instr < MAX_CHANNELS; instr++)
	{
		for (uint8_t bank = 0; bank < MAX_BANKS; bank++)
		{
			for (uint8_t param = 0; param < MAX_PARAMETERS; param++)
			{
				uint8_t def = instrumentRack.GetDefaultParameterValue(instr, param);
				for (uint8_t step = 0; step < MAX_STEPS; step++)
				{
					this->parameters[instr][bank][step][param] = def;
				}
			}
		}
	}
}

void Sequencer::Tick16()
{
	for (uint8_t channel = 0; channel < MAX_CHANNELS; channel++)
	{
		if (bitRead(this->banks[channel][this->currentBank[channel]], this->step))
		{
			for (uint8_t param = 0; param < MAX_PARAMETERS; param ++)
				instrumentRack.SetParameter(channel, param, this->parameters[channel][this->currentBank[channel]][this->step][param]);

			if (bitRead(this->muteMask, channel))
			{
				instrumentRack.Trigger(channel);
			}
		}
	}

	this->step++;

	if (this->step >= MAX_STEPS)
		this->step = 0;
}