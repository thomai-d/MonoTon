#pragma once
#include "../Hardware/HAL.h"
#include "../Configuration.h"
#include "../Synthesis/IInstrument.h"
#include "../Synthesis/ITriggerable.h"

class REnvelope : public ITriggerable
{
public:
	void SetOutput(IInstrument* output, uint8_t parameter);
	void Check();
	virtual void Trigger() override;
	void SetR(uint16_t rMs);

private:
	IInstrument* output = 0;
	uint32_t current = 0;
	uint32_t step = 0;
	uint8_t parameter = 0;
};

