#include "Stopwatch.h"

void Stopwatch::Start()
{
	this->lastTime = micros();
}

void Stopwatch::Event(char * str)
{
	auto now = micros();
	auto offset = now - this->lastTime;
	this->lastTime = now;
	Serial.print(str);
	Serial.println(offset);
}
