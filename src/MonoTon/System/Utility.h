#ifndef UTILITY_H
#define UTILITY_H
#include "../Hardware/HAL.h"

class Utility
{
public:

	static uint16_t NoteToFrequency(uint8_t note);
	static uint32_t NoteToDisplayValue(uint8_t note);

private:
	Utility();
};

#endif