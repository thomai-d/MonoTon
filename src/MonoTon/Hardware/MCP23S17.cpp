#include "MCP23S17.h"
#include <SPI.h>

#define SPI_SPEED_8MHZ 8000000
#define SPI_SPEED_10MHZ 10000000
SPISettings defaultSPISettings(SPI_SPEED_8MHZ, MSBFIRST, SPI_MODE0);

MCP23S17::MCP23S17(uint8_t chipSelectPin, uint8_t chipAddress)
{
	this->readCommand = 0b01000001 | (chipAddress << 1);
	this->writeCommand = 0b01000000 | (chipAddress << 1);
	this->chipSelectPin = chipSelectPin;
}

void MCP23S17::Initialize()
{
	SPI.begin();
	pinMode(this->chipSelectPin, OUTPUT);
	digitalWrite(this->chipSelectPin, HIGH);
}

void MCP23S17::WriteByte(uint8_t reg, uint8_t value)
{
	SPI.beginTransaction(defaultSPISettings);
	digitalWrite(this->chipSelectPin, LOW);
	SPI.transfer(this->writeCommand);
	SPI.transfer(reg);
	SPI.transfer(value);
	digitalWrite(this->chipSelectPin, HIGH);
	SPI.endTransaction();
}

void MCP23S17::WriteByteChecked(uint8_t reg, uint8_t value)
{
	while (true)
	{
		this->WriteByte(reg, value);
		uint8_t newValue = this->ReadByte(reg);
		if (value == newValue)
			return;

		Serial.print(reg, 16);
		Serial.print(" should be ");
		Serial.print(value, 16);
		Serial.print(" but found ");
		Serial.println(newValue, 16);
	}
}

uint8_t MCP23S17::ReadByte(uint8_t reg)
{
	SPI.beginTransaction(defaultSPISettings);
	digitalWrite(this->chipSelectPin, LOW);
	SPI.transfer(this->readCommand);
	SPI.transfer(reg);
	auto val = SPI.transfer(0x00);
	digitalWrite(this->chipSelectPin, HIGH);
	SPI.endTransaction();
	return val;
}
