#include "PerformanceTrace.h"

void PerformanceTrace::Initialize()
{
#ifdef TRACE
	this->traceStart = millis();
	memset(this->timeSpent, 0, sizeof(this->timeSpent));
	this->cycleTimeMin = 0;
	this->cycleTimeMax = 0;
	this->cycles = 0;
#endif
}

void PerformanceTrace::BeginMeasure(TraceObject traceObj)
{
#ifdef TRACE
	this->measureStart = millis();
	this->currentTrace = traceObj;
#endif
}

void PerformanceTrace::EndMeasure()
{
#ifdef TRACE
	uint32_t now = millis();
	uint16_t took = TIME_DELTA_MS(this->measureStart, now);
	this->timeSpent[this->currentTrace] += took;

	this->DumpIfElapsed(now);
#endif
}

void PerformanceTrace::StartCycle()
{
#ifdef TRACE
	this->cycleStart = millis();
#endif
}

void PerformanceTrace::EndCycle()
{
#ifdef TRACE
	uint32_t now = millis();
	uint16_t took = TIME_DELTA_MS(this->measureStart, now);
	if (took > this->cycleTimeMax)
		this->cycleTimeMax = took;
	if (took < this->cycleTimeMax)
		this->cycleTimeMin = took;
	this->cycles++;
#endif
}

void PerformanceTrace::DumpIfElapsed(uint32_t now)
{
#ifdef TRACE
	if (TIME_DELTA_MS(now, this->traceStart) < 5000)
		return;

	Serial.println();
	Serial.println();
	DEBUG("CalcTime:  ", this->timeSpent[SampleCalculation]);
	DEBUG("EnvTime:   ", this->timeSpent[Envelopes]);
	DEBUG("InpTime:   ", this->timeSpent[Inputs]);
	DEBUG("SeqTime:   ", this->timeSpent[Sequencing]);
	DEBUG("FbTime:    ", this->timeSpent[Feedback]);
	DEBUG("CycleMin:  ", this->cycleTimeMin);
	DEBUG("CycleMax:  ", this->cycleTimeMax);
	DEBUG("Cycles:    ", this->cycles);

	DISABLE_INTERRUPTS;
	this->Initialize();
	ENABLE_INTERRUPTS;
#endif
}
