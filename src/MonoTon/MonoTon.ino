#include "SPI.h"
#include "Configuration.h"
#include "System/SyncTimer.h"
#include "System/PerformanceTrace.h"
#include "Control/SequencerController.h"
#include "Hardware/AudioOut.h"
#include "Synthesis/InstrumentRack.h"
#include "System/TimerThree.h"
#include "Control/Sequencer.h"
#include "Hardware/SequencerDriver.h"

SequencerController seqCtrl;

SyncTimer timer16;
SyncTimer analogTimer;
SyncTimer controlTimer;

void NextSample();
void AnalogTimer();

void setup() {

	Serial.begin(115200);
	Serial.println("MonoTon 1.0");

	timer16.Start(60000 / 125 / 4, Timer16th);
	controlTimer.Start(CONTROL_RATE_MS, ControlTimer);
	analogTimer.Start(300, AnalogTimer);

	Timer3.initialize();
	Timer3.attachInterrupt(NextSample, 1000000 / SAMPLE_RATE);

	// Faster analog read
	bitSet(ADCSRA, ADPS2);
	bitClear(ADCSRA, ADPS1);
	bitClear(ADCSRA, ADPS0);

	seqCtrl.Initialize();
}

void Timer16th()
{
	seqCtrl.Tick16();
}

void ControlTimer()
{
	instrumentRack.ControlTick();
}

void AnalogTimer()
{
	seqCtrl.CheckAnalogInputs();
}

void loop() 
{
	timer16.Check();
	controlTimer.Check();
	analogTimer.Check();

	seqCtrl.CheckInputs();
	seqCtrl.UpdateOutputs();
	instrumentRack.ProcessSamples();
}

void NextSample()
{
	audioOut.SampleOut();
}
