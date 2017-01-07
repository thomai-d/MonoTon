#include "Utility.h"
#include "../Hardware/Seg7.h"

Utility::Utility()
{
}

uint16_t Utility::NoteToFrequency(uint8_t note)
{
	const uint16_t noteToFreq[] = { 33, 37, 41, 44, 49, 55, 62,				// C1
								 65, 73, 82, 87, 98, 110, 124,				// C2
								 131, 147, 165, 175, 196, 220, 247,			// C3
								 262, 294, 330, 349, 392, 440, 494,			// C4
								 523, 587, 659, 699, 784, 880, 988,			// C5
								 1047, 1175, 1319, 1397, 1568, 1760, 1976,	// C6
								 2093, 2349, 2637, 2794, 3136, 3520, 3951,	// C7
								 4186, 4699, 5274, 5588, 6272, 7040, 7902 };// C8

	return noteToFreq[note];
}

uint32_t Utility::NoteToDisplayValue(uint8_t note)
{
	const uint8_t notes[] = { DIGIT_C, DIGIT_D, DIGIT_E, DIGIT_F, DIGIT_G, DIGIT_A, DIGIT_H };

	return MAKE_7SEG(notes[(note % 7)], DIGIT_DASH, note / 7 + 1);
}
