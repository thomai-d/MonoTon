#pragma once

#include "Arduino.h"
#include <inttypes.h>

typedef signed char sample_t;

#define DISABLE_INTERRUPTS auto sreg = SREG; cli();
#define ENABLE_INTERRUPTS SREG = sreg;
#define DEBUG(msg, value) Serial.print(msg); Serial.println(value);
#define FLASH_READ_BYTE(base, offset) pgm_read_byte_near(base + offset)

int freeRam();