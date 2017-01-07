#pragma once
#include "../Hardware/HAL.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define RESOLUTION 65536    // Timer3 is 16 bit

class TimerThree
{
  public:
  
    // properties
    unsigned int pwmPeriod;
    unsigned char clockSelectBits;

    // methods
    void initialize(long microseconds=1000000);
    void start();
    void stop();
    void restart();
    void attachInterrupt(void (*isr)(), long microseconds=-1);
    void detachInterrupt();
    void setPeriod(long microseconds);
    void (*isrCallback)();
};

extern TimerThree Timer3;

