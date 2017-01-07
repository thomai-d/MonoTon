#pragma once
#include "../Hardware/HAL.h"
#include "../Configuration.h"
#include "../System/Macros.h"
#include "Rotary.h"
#include "MCP23S17.h"
#include "Seg7.h"

#define SEQCTRL_PIN_CS			3		// SPI chip select pin
#define SEQCTRL_SPI_ADDR		0		// SPI chip address
#define SEQCTRL_PIN_DS			29		// Shift data
#define SEQCTRL_PIN_STC			30		// Shift storage clock
#define SEQCTRL_PIN_SHC			31		// Shift clock

/* PORTEXT0 - Bits GPIOA */
#define EXT0A_ROT1_A		0
#define EXT0A_ROT1_B		1
#define EXT0A_ROT2_A		2
#define EXT0A_ROT2_B		3
#define EXT0A_BTN_A			4
#define EXT0A_BTN_B			5
#define EXT0A_LED_LO		6
#define EXT0A_LED_HI		7

/* PORTEXT0 - Bits GPIOB */
#define EXT0B_BTN_0			0
#define EXT0B_BTN_1			1
#define EXT0B_BTN_2			2
#define EXT0B_BTN_INSTR_PRM	3
#define EXT0B_BTN_SEL_INST	4
#define EXT0B_BTN_SEL_BNK	5
#define EXT0B_BTN_SEL_PARA	6
#define EXT0B_LED_TICK		7

class SeqCtrlDriver
{
public:
	SeqCtrlDriver();

	void Initialize();
	void Test();

	void ClearDisplay();
	void SetNumber(uint8_t number, bool pointA = false, bool pointB = false, bool pointC = false, uint16_t timeoutMs = 0);
	void SetDigits(uint8_t d1, uint8_t d2, uint8_t d3, bool pointA = false, bool pointB = false, bool pointC = false, uint16_t timeoutMs = 0);
	void SetDisplay(uint32_t value, uint16_t timeoutMs = 0);
	void SetTickLed(bool state);
	void SetHiLoLed(bool lo, bool hi);
	bool WasPressedA(uint8_t button) { bool b = bitRead(this->buttonPressedA, button); bitClear(this->buttonPressedA, button); return b; }
	bool WasPressedB(uint8_t button) { bool b = bitRead(this->buttonPressedB, button); bitClear(this->buttonPressedB, button); return b; }

	bool BpmChanged() { bool v = this->bpmChanged; this->bpmChanged = false; return v; }
	uint8_t GetBpm() { return this->rot1Value; }
	void SetBpm(uint8_t bpm) { this->rot1Value = bpm; }

	void CheckInputs();
	void UpdateOutputs();

private:
	void SetDisplayTimeout(uint16_t timeoutMs);
	void SetDisplayRaw(uint8_t d1, uint8_t d2, uint8_t d3);
	void RestoreDisplay();

	// Hardware helpers
	MCP23S17 portExt0;
	Rotary rot1;
	Rotary rot2;

	// BPM
	bool		bpmChanged;

	// Button pressed buffer
	uint32_t	lastButtonUpdate;
	uint16_t	buttonCheckDelay;
	uint8_t		stateA;
	uint8_t		stateB;
	uint8_t		buttonPressedA;
	uint8_t		buttonPressedB;

	// Rotary buffer
	uint8_t		rot1Value;
	uint8_t		rot2Value;

	// Members for display timeout
	uint16_t	displayTimeout;
	uint32_t	lastOutputUpdate;
	uint8_t		defSegA, defSegB, defSegC;

	// PortExtender buffer
	bool		ext0RefreshNeeded;
	bool		tickState, hiState, loState;

	// 8-Segment buffer
	bool		seg8RefreshNeeded;
	bool		pointA, pointB, pointC;
	uint8_t		segA, segB, segC;
};

