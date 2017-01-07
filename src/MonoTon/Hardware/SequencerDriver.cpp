#include "SequencerDriver.h"

// todo verschieben
void shiftOut16(uint8_t dataPin, uint8_t clockPin, uint8_t storePin, uint16_t data)
{
	digitalWrite(storePin, LOW);
	shiftOut(dataPin, clockPin, MSBFIRST, data >> 8);
	shiftOut(dataPin, clockPin, MSBFIRST, data & 0xff);
	digitalWrite(storePin, HIGH);
}

SequencerDriver::SequencerDriver()
	: portExtInstr(SEQ_PIN_BTN_CS, 0),
	  portExtSeq(SEQ_PIN_BTN_CS, 1)
{
}

void SequencerDriver::Initialize()
{
	pinMode(SEQ_PIN_POTI_A, OUTPUT);
	pinMode(SEQ_PIN_POTI_B, OUTPUT);
	pinMode(SEQ_PIN_POTI_C, OUTPUT);
	pinMode(SEQ_PIN_POTI_D, OUTPUT);
	pinMode(SEQ_PIN_LEDGN_D, OUTPUT);
	pinMode(SEQ_PIN_LEDGN_CLK, OUTPUT);
	pinMode(SEQ_PIN_LEDGN_LAT, OUTPUT);
	pinMode(SEQ_PIN_LEDINSTR_D, OUTPUT);
	pinMode(SEQ_PIN_LEDINSTR_CLK, OUTPUT);
	pinMode(SEQ_PIN_LEDINSTR_LAT, OUTPUT);
	pinMode(SEQ_PIN_LEDBL_D, OUTPUT);
	pinMode(SEQ_PIN_LEDBL_CLK, OUTPUT);
	pinMode(SEQ_PIN_LEDBL_LAT, OUTPUT);

	this->SetBlueLEDs(0);
	this->SetGreenLEDs(0);
	this->SetInstrumentLEDs(0);

	this->portExtSeq.Initialize();
	this->portExtSeq.WriteByte(GPPUA, 0xff);
	this->portExtSeq.WriteByte(GPPUB, 0xff);
	this->portExtSeq.WriteByte(IODIRA, 0xff);
	this->portExtSeq.WriteByte(IODIRB, 0xff);
	this->portExtSeq.WriteByte(IPOLA, 0xff);
	this->portExtSeq.WriteByte(IPOLB, 0xff);
	this->portExtSeq.WriteByte(IOCON1, HAEN);
	this->portExtInstr.Initialize();
	this->portExtInstr.WriteByte(GPPU, 0xff);
	this->portExtInstr.WriteByte(IODIR, 0xff);
	this->portExtInstr.WriteByte(IPOL, 0xff);
	this->portExtInstr.WriteByte(IOCON, HAEN);

	this->SetInstrumentLEDs(1);
}

void SequencerDriver::Test()
{
	uint16_t counter = 1;
	do
	{
		this->SetBlueLEDs(counter);
		this->SetGreenLEDs(counter);
		delay(100);
		counter <<= 1;
	} while (counter != 0);

	this->SetBlueLEDs(0);
	this->SetGreenLEDs(0);
	counter = 0B100000000;
	do
	{
		this->SetInstrumentLEDs(counter);
		delay(100);
		counter >>= 1;
	} while (counter != 0);


	this->SetInstrumentLEDs(0);
}

void SequencerDriver::CheckInputs()
{
	uint8_t instrBtns = this->portExtInstr.ReadByte(GPIO);
	uint8_t seqBtnsA = this->portExtSeq.ReadByte(GPIOA);
	uint8_t seqBtnsB = this->portExtSeq.ReadByte(GPIOB);
	uint16_t seqBtns = ((uint16_t)seqBtnsB << 8) + seqBtnsA;
	bool changed = false;

	if (instrBtns != this->instrBtnStates)
	{
		for (uint8_t i = 0; i < 8; i++)
		{
			bool lastState = bitRead(this->instrBtnStates, i);
			bool currentState = bitRead(instrBtns, i);
			if (!lastState && currentState)
			{
				this->selection = i;
				this->selectionChanged = true;
				break;
			}
		}

		this->instrBtnStates = instrBtns;
	}

	if (seqBtns != this->seqBtnStates)
	{
		for (uint8_t i = 0; i < 16; i++)
		{
			bool lastState = bitRead(this->seqBtnStates, i);
			bool currentState = bitRead(seqBtns, i);
			if (!lastState && currentState)
			{
				// pressed
				bitWrite(this->sequencerSteps, i, !bitRead(this->sequencerSteps, i));
				this->sequencerStepsChanged = true;
				this->SetGreenLEDs(this->sequencerSteps);
			}
		}

		this->seqBtnStates = seqBtns;
	}
}

void SequencerDriver::SetStep(uint8_t step)
{
	this->SetBlueLEDs((uint16_t)1 << step);
	delay(1);
	this->SetBlueLEDs(0);
}

void SequencerDriver::SetSteps(uint16_t bank)
{
	this->sequencerSteps = bank;
	this->SetGreenLEDs(this->sequencerSteps);
}

uint8_t SequencerDriver::GetValue(uint8_t step)
{
	digitalWrite(SEQ_PIN_POTI_A, bitRead(step, 0));
	digitalWrite(SEQ_PIN_POTI_B, bitRead(step, 1));
	digitalWrite(SEQ_PIN_POTI_C, bitRead(step, 2));
	digitalWrite(SEQ_PIN_POTI_D, bitRead(step, 3));
	return analogRead(A4) / 16;
}

bool SequencerDriver::IsDebugButtonPressed()
{
	return (this->portExtInstr.ReadByte(GPIO) > 0);
}

void SequencerDriver::SetBlueLEDs(uint16_t value)
{
	shiftOut16(SEQ_PIN_LEDBL_D, SEQ_PIN_LEDBL_CLK, SEQ_PIN_LEDBL_LAT, value);
}

void SequencerDriver::SetGreenLEDs(uint16_t value)
{
	shiftOut16(SEQ_PIN_LEDGN_D, SEQ_PIN_LEDGN_CLK, SEQ_PIN_LEDGN_LAT, value);
}

void SequencerDriver::SetInstrumentLEDs(uint8_t value)
{
	digitalWrite(SEQ_PIN_LEDINSTR_LAT, LOW);
	shiftOut(SEQ_PIN_LEDINSTR_D, SEQ_PIN_LEDINSTR_CLK, MSBFIRST, value);
	digitalWrite(SEQ_PIN_LEDINSTR_LAT, HIGH);
}
