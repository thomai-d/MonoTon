#pragma once
#include "../Hardware/HAL.h"

enum TraceObject
{
	SampleCalculation,
	Envelopes,
	Inputs,
	Sequencing,
	Feedback,
	TRACE_COUNT
};

class PerformanceTrace
{
public:
	void Initialize();
	void BeginMeasure(TraceObject traceObj);
	void EndMeasure();

	void StartCycle();
	void EndCycle();

private:

	void DumpIfElapsed(uint32_t now);

	uint32_t    traceStart;
	uint32_t    measureStart;
	uint32_t    cycleStart;
	TraceObject currentTrace;
	uint16_t    timeSpent[TRACE_COUNT];

	byte        cycleTimeMin;
	byte        cycleTimeMax;
	uint16_t	cycles;
};
