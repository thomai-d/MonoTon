#ifndef ROTARY_H
#include "../Hardware/HAL.h"
#define ROTARY_H

typedef signed char int8_t;

#define DIR_NONE	0x0
#define DIR_CW		0x10
#define DIR_CCW		0x20

class Rotary
{
public:
	int8_t Process(bool pinA, bool pinB);
private:
	unsigned char state;
};

#endif

