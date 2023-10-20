#include <SPI.h>

static const int spiClk = 1000000;
byte address = 0x00;
SPIClass *hspi = NULL;

void setup()
{
  pinMode(53, OUTPUT); // Set SS (Slave Select) pin to 53 on Mega
  hspi = new SPIClass(SPI);
  hspi->begin();
}

void loop()
{
  for (int i = 0; i <= 255; i++)
  {
    digitalPotWrite(i);
    delay(10);
  }
  delay(500);
  for (int i = 255; i >= 0; i--)
  {
    digitalPotWrite(i);
    delay(10);
  }
}

int digitalPotWrite(int value)
{
  hspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(53, LOW); // Select the MCP4131
  hspi->transfer(address);
  hspi->transfer(value);
  digitalWrite(53, HIGH); // Deselect the MCP4131
  hspi->endTransaction();
}
