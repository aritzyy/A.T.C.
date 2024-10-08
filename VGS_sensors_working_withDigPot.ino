//voltage sensor, current sensor, & digital pot
#include <SPI.h>

static const int spiClk = 1000000;
byte address = 0x00;
SPIClass *hspi = NULL;

const int gatePin = A0;
const int sourcePin = A2;
const int drainPin = A1;

void setup() 
{
  //for digital pot
  pinMode(53, OUTPUT);
  hspi = new SPIClass(SPI);
  hspi->begin();

  //for voltage sensor
  Serial.begin(9600);
}

void loop() {
  for (int i = 0; i <= 255; i++)
  {
    digitalPotWrite(i);
    delay(10);
  }
  delay(5000);
  for (int i = 255; i >= 0; i--)
  {
    digitalPotWrite(i);
    delay(10);
  }
}

float digitalPotWrite(int value)
{
  hspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  //select digital pot
  digitalWrite(53, LOW); 
  hspi->transfer(address);
  hspi->transfer(value);

  //for voltage & current sensor
  //for gate pin
  int rawValue = analogRead(gatePin);
  float voltageGate = (float)rawValue * (30.0 / 1024.0); // 30V is the maximum voltage

  //for drain pin
  int rawValue2 = analogRead(drainPin);
  float voltageDrain = (float)rawValue2 * (30.0 / 1024.0);

  //for source pin
  int rawValue3 = analogRead(sourcePin);
  float voltageSource = (float)rawValue3 * (30.0 / 1024.0);

  //calculate current
  float current = voltageSource / 99.64;

  //calculate vgs 
  float VGS = voltageGate - voltageSource;

  Serial.print("GVoltage: "); 
  Serial.print(voltageGate, 3); //1.1725 v raw value: 282
  Serial.print("        DVoltage: "); 
  Serial.print(voltageDrain, 3);
  Serial.print("        SVoltage: "); 
  Serial.print(voltageSource, 3);
  Serial.print("          VGS: ");
  Serial.print(VGS, 3);
  Serial.print("         Current: ");
  Serial.print(current, 4);
  Serial.println();

  //deselect digital pot
  digitalWrite(53, HIGH); 
  hspi->endTransaction();
}