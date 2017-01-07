#pragma once
// todo includes in .cpp verschieben
#include "../Hardware/HAL.h"
#include "ITriggerable.h"
#include "ISampleProvider.h"

class IInstrument : public ISampleProvider, public ITriggerable
{
public:
	virtual void SetParameter(uint8_t parameter, uint8_t value) = 0;
	virtual void ControlTick() = 0;
	virtual uint8_t GetDefaultParameterValue(uint8_t param) { return 64; }

	virtual uint32_t GetDisplayValue(uint8_t param, uint8_t value) = 0;
};
