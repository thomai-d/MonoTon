#pragma once
#include "../Hardware/HAL.h"
#include "../Configuration.h"
#include "../System/Macros.h"
#include "../Hardware/MCP23S17.h"

#define SEQ_PIN_POTI_A			24		// Potis A
#define SEQ_PIN_POTI_B			25		// Potis B
#define SEQ_PIN_POTI_C			26		// Potis C
#define SEQ_PIN_POTI_D			27		// Potis D
#define SEQ_PIN_LEDGN_D			10		// Green LEDs data
#define SEQ_PIN_LEDGN_CLK		11		// Green LEDs clock	
#define SEQ_PIN_LEDGN_LAT		12		// Green LEDs latch
#define SEQ_PIN_LEDBL_D			13		// Blue LEDs data
#define SEQ_PIN_LEDBL_CLK		14		// Blue LEDs clock
#define SEQ_PIN_LEDBL_LAT		15		// Blue LEDs latch
#define SEQ_PIN_LEDINSTR_D		02		// Instrument LEDs data	
#define SEQ_PIN_LEDINSTR_CLK	01		// Instrument LEDs clock
#define SEQ_PIN_LEDINSTR_LAT	00		// Instrument LEDs latch
#define SEQ_PIN_BTN_CS			04		// Chip select 23S17 / 23S08

class SequencerDriver
{
public:
	SequencerDriver();

	void Initialize();
	void Test();

	void CheckInputs();
	void SetStep(uint8_t step);
	void SetSteps(uint16_t bank);
	uint16_t GetSteps() { return this->sequencerSteps; }

	void SetInstrumentLEDs(uint8_t value);

	uint8_t GetValue(uint8_t step);

	bool IsDebugButtonPressed();

	uint8_t GetCurrentSelection() { return this->selection; }
	void SetCurrentSelection(uint8_t sel) { this->selection = sel; }

	bool	HasSelectionChanged() { bool v = this->selectionChanged; this->selectionChanged = false; return v; }
	bool	HaveSequencerStepsChanged() { bool v = this->sequencerStepsChanged; this->sequencerStepsChanged = false; return v; }

private:

	void SetBlueLEDs(uint16_t value);
	void SetGreenLEDs(uint16_t value);

	bool		selectionChanged;
	bool		sequencerStepsChanged;
	uint8_t		selection = 0;
	uint16_t	sequencerSteps = 0x00;

	// Button pressed buffer
	uint8_t		instrBtnStates = 0;
	uint16_t	seqBtnStates = 0;

	// Hardware helpers
	// todo mcp umbenennen
	MCP23S17 portExtInstr;
	MCP23S17 portExtSeq;
};

