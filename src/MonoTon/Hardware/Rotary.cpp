/* Original source: http://www.buxtronix.net/2011/10/rotary-encoders-done-properly.html */

#include "Arduino.h"
#include "rotary.h"

#define R_START 0x0
#define R_CW_FINAL 0x1
#define R_CW_BEGIN 0x2
#define R_CW_NEXT 0x3
#define R_CCW_BEGIN 0x4
#define R_CCW_FINAL 0x5
#define R_CCW_NEXT 0x6

const unsigned char ttable[7][4] = {
  {R_START,    R_CW_BEGIN,  R_CCW_BEGIN, R_START},
  {R_CW_NEXT,  R_START,     R_CW_FINAL,  R_START | DIR_CW},
  {R_CW_NEXT,  R_CW_BEGIN,  R_START,     R_START},
  {R_CW_NEXT,  R_CW_BEGIN,  R_CW_FINAL,  R_START},
  {R_CCW_NEXT, R_START,     R_CCW_BEGIN, R_START},
  {R_CCW_NEXT, R_CCW_FINAL, R_START,     R_START | DIR_CCW},
  {R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START},
};

int8_t Rotary::Process(bool pinA, bool pinB) 
{
  unsigned char pinstate = (pinB << 1) | pinA;
  state = ttable[state & 0xf][pinstate];
  switch (state & 0x30)
  {
  case DIR_CW:
	  return -1;
  case DIR_CCW:
	  return 1;
  }

  return 0;
}
