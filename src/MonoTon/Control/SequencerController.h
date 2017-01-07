#ifndef SEQUENCER_CONTROLLER_H
#define SEQUENCER_CONTROLLER_H

#include "Sequencer.h"
#include "../Hardware/SeqCtrlDriver.h"
#include "../Hardware/SequencerDriver.h"
#include "../System/SyncTimer.h"
#include "../Synthesis/InstrumentRack.h"
#include "../Configuration.h"

enum SelectionType
{
	InstrumentSel = 0,
	BankSel = 1,
	ParameterSel = 2,
	InstrParameter = 3
};

class SequencerController
{
public:
	void Initialize();
	
	void CheckInputs();
	void CheckAnalogInputs();
	void UpdateOutputs();

	void Tick16();

private:

	void ToggleBank(uint8_t channel);
	void ToggleMute(uint8_t channel);
	void BankSwitch(uint8_t channel, bool bankToggleState);
	void RefreshStatusAndSteps();

	Sequencer sequencer;
	SeqCtrlDriver controlHw;
	SequencerDriver sequencerHw;

	uint8_t		parameterValues[16];
	uint8_t		lastParamValue[16];

	// Selection
	uint8_t		currentInstr = 0;
	uint8_t		currentParam = 0;
	uint8_t		currentBank = 0;
	SelectionType currentSelection = SelectionType::InstrumentSel;
};

extern SyncTimer timer16;

#endif
