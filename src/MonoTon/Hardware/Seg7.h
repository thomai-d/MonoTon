#pragma once
#include "..\Hardware\HAL.h"

// 7-Seg lookup stuff
#define	DIGIT_OFF			0xff
#define DIGIT_A				10
#define DIGIT_B				11
#define DIGIT_C				12
#define DIGIT_D				13
#define DIGIT_E				14
#define DIGIT_F				15
#define DIGIT_G				16
#define DIGIT_H				17
#define DIGIT_I				18
#define DIGIT_DASH			19
#define DIGIT_P				20	

static const uint8_t _7segLookup[] = { 0B01110111, 0B00000110, 0B10110011, 0B10010111, 0B11000110, 0B11010101, 0B11110101, 0B00000111, 0B11110111, 0B11010111,  // 0 - 9
									   0B11100111, 0B00101111, 0B01110001, 0B10110110, 0B11110001, 0B11100001, 0B11110101, 0B11100110, 0B00000110, 0B10000000,  // A - I, Dash
									   0B11100011};  // P
static const uint8_t _7segTest[9] = { 0B10000000, 0B00000100, 0B00010000, 0B00100000, 0B10000000, 0B00000010 };


#define MAKE_7SEG(A, B, C) (((uint32_t)_7segLookup[A] << 16) + ((uint32_t)_7segLookup[B] << 8) + (uint32_t)_7segLookup[C])
#define GETA_SEG7(x) ((x >> 16) & 0xff)
#define GETB_SEG7(x) ((x >> 8) & 0xff)
#define GETC_SEG7(x) (x & 0xff)
