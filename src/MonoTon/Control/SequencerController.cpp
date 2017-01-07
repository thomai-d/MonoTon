#include "SequencerController.h"

void SequencerController::Initialize()
{
	this->sequencer.Initialize();
	this->sequencerHw.Initialize();
	this->controlHw.Initialize();

	if (this->sequencerHw.IsDebugButtonPressed())
	{
		this->sequencerHw.Test();
		this->controlHw.Test();
	}

	this->RefreshStatusAndSteps();
	this->controlHw.SetBpm(125);
}

void SequencerController::CheckInputs()
{
	this->controlHw.CheckInputs();
	this->sequencerHw.CheckInputs();

	uint8_t selectionId = this->sequencerHw.GetCurrentSelection();
	uint8_t nextStep = sequencer.GetNextStep();

	if (this->sequencerHw.HasSelectionChanged())
	{
		if (this->currentSelection == SelectionType::InstrumentSel)
		{
			this->currentInstr = selectionId;
			this->currentBank = this->sequencer.GetCurrentBank(selectionId);
			this->sequencerHw.SetSteps(this->sequencer.GetSteps(selectionId, this->currentBank));
		}

		if (this->currentSelection == SelectionType::BankSel)
		{
			if (selectionId >= MAX_BANKS)
			{
				selectionId = MAX_BANKS - 1;
				this->sequencerHw.SetCurrentSelection(selectionId);
			}

			this->currentBank = selectionId;
			this->sequencer.SetCurrentBank(this->currentInstr, this->currentBank);
			this->sequencerHw.SetSteps(this->sequencer.GetSteps(this->currentInstr, this->currentBank));
		}

		if (this->currentSelection == SelectionType::ParameterSel)
		{
			if (selectionId >= MAX_PARAMETERS)
			{
				selectionId = MAX_PARAMETERS - 1;
				this->sequencerHw.SetCurrentSelection(selectionId);
			}
			this->currentParam = selectionId;
		}

		if (this->currentSelection == SelectionType::InstrParameter)
		{
			this->currentSelection = SelectionType::InstrumentSel;
			this->currentInstr = selectionId;
			this->currentBank = this->sequencer.GetCurrentBank(selectionId);
			this->sequencerHw.SetSteps(this->sequencer.GetSteps(selectionId, this->currentBank));
		}

		this->RefreshStatusAndSteps();
	}

	if (this->sequencerHw.HaveSequencerStepsChanged())
	{
		switch (this->currentSelection)
		{
		case SelectionType::InstrumentSel:
		case SelectionType::BankSel:
		case SelectionType::ParameterSel:
			this->sequencer.SetSteps(this->currentInstr, this->currentBank, this->sequencerHw.GetSteps());
			break;

		case SelectionType::InstrParameter:
			// set channel mute
			uint16_t bank = this->sequencerHw.GetSteps();
			uint8_t muteMask = bank & 0xff;
			this->sequencer.SetMuteMask(muteMask);

			// set channel banks
			uint8_t bankBits = bank >> 8;
			for (uint8_t c = 0; c < MAX_CHANNELS; c++)
			{
				uint8_t requestedBankBit = bitRead(bankBits, c);
				this->BankSwitch(c, requestedBankBit);
			}
			break;
		}
	}

	if (this->controlHw.WasPressedB(EXT0B_BTN_INSTR_PRM))
	{
		this->currentSelection = SelectionType::InstrParameter;
		this->RefreshStatusAndSteps();
	}

	if (this->controlHw.WasPressedB(EXT0B_BTN_SEL_INST))
	{
		this->currentSelection = SelectionType::InstrumentSel;
		this->RefreshStatusAndSteps();
	}

	if (this->controlHw.WasPressedB(EXT0B_BTN_SEL_BNK))
	{
		this->currentSelection = SelectionType::BankSel;
		this->RefreshStatusAndSteps();
	}

	if (this->controlHw.WasPressedB(EXT0B_BTN_SEL_PARA))
	{
		this->currentSelection = SelectionType::ParameterSel;
		this->RefreshStatusAndSteps();
	}

	if (this->controlHw.WasPressedA(EXT0A_BTN_A))
	{
		this->ToggleBank(this->currentInstr);
		this->RefreshStatusAndSteps();
	}

	if (this->controlHw.WasPressedA(EXT0A_BTN_B))
	{
		this->sequencer.GetMuteMask();

		this->ToggleMute(this->currentInstr);
		this->RefreshStatusAndSteps();
	}

	if (this->controlHw.BpmChanged())
	{
		uint8_t bpm = this->controlHw.GetBpm();
		uint16_t interval = (uint16_t)60000 / bpm / 4;
		timer16.SetInterval(interval);
	}
}

void SequencerController::CheckAnalogInputs()
{
	for (uint8_t step = 0; step < MAX_STEPS; step++)
	{
		uint8_t value = this->sequencerHw.GetValue(step);
		uint8_t oldValue = this->parameterValues[step];
		uint8_t lastValue = this->lastParamValue[step];

		if (value != oldValue && value != lastValue)
		{
			this->controlHw.SetNumber(value, false, false, false, 1000);
			this->lastParamValue[step] = this->parameterValues[step];
			this->parameterValues[step] = value;

			this->controlHw.SetDisplay(instrumentRack.GetParameterDisplayValue(this->currentInstr, this->currentParam, value), PARAMETER_DISPLAY_TIME_MS);
			this->sequencer.SetParameter(this->currentInstr, this->currentBank, step, this->currentParam, value);
		}
	}
}

void SequencerController::UpdateOutputs()
{
	this->controlHw.UpdateOutputs();
}

void SequencerController::Tick16()
{
	switch (this->currentSelection)
	{
	case SelectionType::InstrumentSel:
	case SelectionType::BankSel:
	case SelectionType::ParameterSel:
	{
		uint8_t currentStep = this->sequencer.GetNextStep();
		this->sequencerHw.SetStep(currentStep);
		break;
	}

	case SelectionType::InstrParameter:
		break;
	}

	this->sequencer.Tick16();
}

void SequencerController::ToggleMute(uint8_t channel)
{
	uint8_t muteMask = this->sequencer.GetMuteMask();
	bool isMute = bitRead(muteMask, this->currentInstr);
	bitWrite(muteMask, this->currentInstr, !isMute);
	this->sequencer.SetMuteMask(muteMask);
}

void SequencerController::ToggleBank(uint8_t channel)
{
	uint8_t currentBank = this->sequencer.GetCurrentBank(channel);
	bool currentBankBit = currentBank & 1;
	bitWrite(currentBank, 0, !currentBankBit);
	this->sequencer.SetCurrentBank(channel, currentBank);
}

// Toggles the current bank's last bit (0->1, 2->3, ...)
void SequencerController::BankSwitch(uint8_t channel, bool bankToggleState)
{
	uint8_t currentBank = this->sequencer.GetCurrentBank(channel);
	uint8_t currentBankBit = currentBank & 1;
	if (bankToggleState != currentBankBit)
	{
		bitWrite(currentBank, 0, bankToggleState);
		this->sequencer.SetCurrentBank(channel, currentBank);
	}
}

void SequencerController::RefreshStatusAndSteps()
{
	// Refresh bank, may have changed by bank switch.
	this->currentBank = this->sequencer.GetCurrentBank(this->currentInstr);

	uint8_t muteMask = this->sequencer.GetMuteMask();
	bool isMute = bitRead(muteMask, this->currentInstr);
	this->controlHw.SetTickLed(isMute);

	switch (this->currentSelection)
	{
	case SelectionType::InstrumentSel:
		this->controlHw.SetDigits(this->currentInstr, this->currentBank, this->currentParam, true, false, false);
		this->sequencerHw.SetInstrumentLEDs(1 << this->currentInstr);
		this->sequencerHw.SetSteps(this->sequencer.GetSteps(this->currentInstr, this->currentBank));
		break;

	case SelectionType::BankSel:
		this->controlHw.SetDigits(this->currentInstr, this->currentBank, this->currentParam, false, true, false);
		this->sequencerHw.SetInstrumentLEDs(1 << this->currentBank);
		this->sequencerHw.SetSteps(this->sequencer.GetSteps(this->currentInstr, this->currentBank));
		break;

	case SelectionType::ParameterSel:
		this->controlHw.SetDigits(this->currentInstr, this->currentBank, this->currentParam, false, false, true);
		this->sequencerHw.SetInstrumentLEDs(1 << this->currentParam);
		this->sequencerHw.SetSteps(this->sequencer.GetSteps(this->currentInstr, this->currentBank));
		break;

	case SelectionType::InstrParameter:
		this->controlHw.SetDigits(DIGIT_OFF, DIGIT_I, DIGIT_P);
		this->sequencerHw.SetInstrumentLEDs(0);

		uint16_t display = this->sequencer.GetMuteMask();
		for (uint8_t c = 0; c < MAX_CHANNELS; c++)
		{
			bitWrite(display, 8 + c, this->sequencer.GetCurrentBank(c) & 1);
		}
		this->sequencerHw.SetSteps(display);
		break;
	}
}
