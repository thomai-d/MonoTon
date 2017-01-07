#include "SeqCtrlDriver.h"
#include "../Hardware/Seg7.h"

#define POINT_BIT		0B1000

SeqCtrlDriver::SeqCtrlDriver() 
	: portExt0(SEQCTRL_PIN_CS, 0x0)
{
}

void SeqCtrlDriver::Initialize()
{
	pinMode(SEQCTRL_PIN_DS, OUTPUT);
	pinMode(SEQCTRL_PIN_STC, OUTPUT);
	pinMode(SEQCTRL_PIN_SHC, OUTPUT);
	
	portExt0.Initialize();
	portExt0.WriteByteChecked(IODIRA, 0B00111111);
	portExt0.WriteByteChecked(GPPUA, 0B00111111);
	portExt0.WriteByteChecked(IPOLA, 0B00111111);
	portExt0.WriteByteChecked(IODIRB, 0B01111111);
	portExt0.WriteByteChecked(GPPUB, 0B01111111);
	portExt0.WriteByteChecked(IPOLB, 0B01111111);
	portExt0.WriteByteChecked(OLATA, 0x0);
	portExt0.WriteByteChecked(OLATB, 0x0);

	this->ClearDisplay();
	this->SetHiLoLed(0, 0);
	this->SetTickLed(0);
	this->UpdateOutputs();

	this->lastButtonUpdate = millis();
}

void SeqCtrlDriver::Test()
{
	const int shortDelayTime = 50;
	const int longDelayTime = 150;

	this->SetTickLed(1);
	this->UpdateOutputs();
	delay(longDelayTime);
	this->SetTickLed(0);

	this->SetHiLoLed(0, 1);
	this->UpdateOutputs();
	delay(longDelayTime);

	this->SetHiLoLed(1, 0);
	this->UpdateOutputs();
	delay(longDelayTime);
	this->SetHiLoLed(0, 0);
	
	for (uint8_t i = 0; i < 9; i++)
	{
		this->SetDisplayRaw(_7segTest[i], 0, 0);
		this->UpdateOutputs();
		delay(shortDelayTime);
	}
	for (uint8_t i = 0; i < 9; i++)
	{
		this->SetDisplayRaw(0, _7segTest[i], 0);
		this->UpdateOutputs();
		delay(shortDelayTime);
	}
	for (uint8_t i = 0; i < 9; i++)
	{
		this->SetDisplayRaw(0, 0, _7segTest[i]);
		this->UpdateOutputs();
		delay(shortDelayTime);
	}
	this->SetDisplayRaw(0, 0, 0);
	this->UpdateOutputs();
}

void SeqCtrlDriver::ClearDisplay()
{
	this->SetDisplayRaw(0, 0, 0);
}

void SeqCtrlDriver::SetNumber(uint8_t number, bool pointA, bool pointB, bool pointC, uint16_t timeoutMs)
{
	uint8_t d1 = number % 10;
	number /= 10;
	uint8_t d2 = number % 10;
	number /= 10;
	uint8_t d3 = number % 10;
	this->SetDigits(d3, d2, d1, pointA, pointB, pointC, timeoutMs);
}

void SeqCtrlDriver::SetDigits(uint8_t d1, uint8_t d2, uint8_t d3, bool pointA, bool pointB, bool pointC, uint16_t timeoutMs)
{
	d1 = d1 < sizeof(_7segLookup) ? _7segLookup[d1] : 0;
	d2 = d2 < sizeof(_7segLookup) ? _7segLookup[d2] : 0;
	d3 = d3 < sizeof(_7segLookup) ? _7segLookup[d3] : 0;
	if (pointA) d1 |= POINT_BIT;
	if (pointB) d2 |= POINT_BIT;
	if (pointC) d3 |= POINT_BIT;
	this->SetDisplayRaw(d1, d2, d3);

	if (timeoutMs != 0)
	{
		this->SetDisplayTimeout(timeoutMs);
	}
	else
	{
		this->defSegA = d1;
		this->defSegB = d2;
		this->defSegC = d3;
	}
}

void SeqCtrlDriver::SetDisplay(uint32_t value, uint16_t timeoutMs)
{
	uint8_t d1 = GETA_SEG7(value);
	uint8_t d2 = GETB_SEG7(value);
	uint8_t d3 = GETC_SEG7(value);
	this->SetDisplayRaw(d1, d2, d3);

	if (timeoutMs != 0)
	{
		this->SetDisplayTimeout(timeoutMs);
	}
	else
	{
		this->defSegA = d1;
		this->defSegB = d2;
		this->defSegC = d3;
	}
}

void SeqCtrlDriver::SetDisplayTimeout(uint16_t timeoutMs)
{
	this->lastOutputUpdate = millis();
	this->displayTimeout = timeoutMs;
}

void SeqCtrlDriver::SetTickLed(bool state)
{
	this->tickState = state;
	this->ext0RefreshNeeded = true;
}

void SeqCtrlDriver::SetHiLoLed(bool lo, bool hi)
{
	this->hiState = hi;
	this->loState = lo;
	this->ext0RefreshNeeded = true;
}

void SeqCtrlDriver::CheckInputs()
{
	uint8_t gpioA = portExt0.ReadByte(GPIOA) & 0B00111111;
	uint8_t gpioB = portExt0.ReadByte(GPIOB) & 0B01111111;

	// Check rotary buttons
	int8_t rot1Delta = rot1.Process(bitRead(gpioA, EXT0A_ROT1_A), bitRead(gpioA, EXT0A_ROT1_B));
	int8_t rot2Delta = rot2.Process(bitRead(gpioA, EXT0A_ROT2_A), bitRead(gpioA, EXT0A_ROT2_B));
	this->rot1Value += rot1Delta;
	this->rot2Value += rot2Delta;

	uint32_t now = millis();
	uint16_t delta = TIME_DELTA_MS(now, this->lastButtonUpdate);
	this->lastButtonUpdate = now;
	if (this->buttonCheckDelay > delta)
	{
		this->buttonCheckDelay -= delta;
	}
	else
	{
		this->buttonCheckDelay = BUTTON_DEBOUNCE_MILLIS;

		// GPIOA
		for (uint8_t i = 4; i <= 5; i++)
		{
			bool state = bitRead(gpioA, i);
			bool lastState = bitRead(this->stateA, i);
			if (!lastState && state)
			{
				bitSet(this->buttonPressedA, i);
			}
		}

		// GPIOB
		for (uint8_t i = 0; i <= 6; i++)
		{
			bool state = bitRead(gpioB, i);
			bool lastState = bitRead(this->stateB, i);
			if (!lastState && state)
			{
				bitSet(this->buttonPressedB, i);
			}
		}

		this->stateA = gpioA;
		this->stateB = gpioB;
	}

	// DEBUG
	if (rot1Delta != 0)
	{
		this->SetNumber(this->rot1Value, false, true, false, 500);
		this->bpmChanged = true;
	}

	if (rot2Delta != 0)
	{
		this->SetNumber(this->rot2Value, false, false, true, 500);
	}
}

void SeqCtrlDriver::UpdateOutputs()
{
	if (this->ext0RefreshNeeded)
	{
		this->portExt0.WriteByte(OLATB, this->tickState << EXT0B_LED_TICK);
		this->portExt0.WriteByte(OLATA, (this->hiState << EXT0A_LED_HI) | (this->loState << EXT0A_LED_LO));
		this->ext0RefreshNeeded = false;
	}

	if (this->seg8RefreshNeeded)
	{
		shiftOut(SEQCTRL_PIN_DS, SEQCTRL_PIN_SHC, MSBFIRST, this->segC);
		shiftOut(SEQCTRL_PIN_DS, SEQCTRL_PIN_SHC, MSBFIRST, this->segB);
		shiftOut(SEQCTRL_PIN_DS, SEQCTRL_PIN_SHC, MSBFIRST, this->segA);
		digitalWrite(SEQCTRL_PIN_STC, LOW);
		digitalWrite(SEQCTRL_PIN_STC, HIGH);
		this->seg8RefreshNeeded = false;
	}

	if (this->displayTimeout > 0)
	{
		uint32_t now = millis();
		uint16_t delta = TIME_DELTA_MS(this->lastOutputUpdate, now);

		if (this->displayTimeout <= delta)
		{
			this->displayTimeout = 0;
			this->RestoreDisplay();
		}
		else
		{
			this->lastOutputUpdate = now;
			this->displayTimeout -= delta;
		}
	}
}

void SeqCtrlDriver::SetDisplayRaw(uint8_t d1, uint8_t d2, uint8_t d3)
{
	this->segA = d1;
	this->segB = d2;
	this->segC = d3;
	this->seg8RefreshNeeded = true;
}

void SeqCtrlDriver::RestoreDisplay()
{
	this->segA = this->defSegA;
	this->segB = this->defSegB;
	this->segC = this->defSegC;
	this->seg8RefreshNeeded = true;
}
