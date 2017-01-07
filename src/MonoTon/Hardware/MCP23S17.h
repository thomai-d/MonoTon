#pragma once
#include "../Hardware/HAL.h"

// MCP23S17
#define IODIRA		0x00
#define IODIRB		0x01
#define IPOLA		0x02
#define IPOLB		0x03
#define GPINTENA	0x04
#define GPINTENB	0x05
#define DEFVALA		0x06
#define DEFVALB		0x07
#define INTCONA		0x08
#define INTCONB		0x09
#define IOCON1		0x0a
#define IOCON2		0x0b
#define GPPUA		0x0c
#define GPPUB		0x0d
#define INTFA		0x0e
#define INTFB		0x0f
#define INTCAPA		0x10
#define INTCAPB		0x11
#define GPIOA		0x12
#define GPIOB		0x13
#define OLATA		0x14
#define OLATB		0x15

// IOCON Flags
#define HAEN		0x08		// Hardware address enable

// MCP23S08
#define IODIR		0x00
#define IPOL		0x01
#define GPINTEN		0x02
#define DEFVAL		0x03
#define INTCON		0x04
#define IOCON		0x05
#define GPPU		0x06
#define INTF		0x07
#define INTCAP		0x08
#define GPIO		0x09
#define OLAT		0x0a

class MCP23S17
{
public:
	MCP23S17(uint8_t chipSelectPin, uint8_t chipAddress);

	void Initialize();
	void WriteByte(uint8_t reg, uint8_t value);
	void WriteByteChecked(uint8_t reg, uint8_t value);
	uint8_t ReadByte(uint8_t reg);

private:
	uint8_t readCommand;
	uint8_t writeCommand;
	uint8_t chipSelectPin;
};

