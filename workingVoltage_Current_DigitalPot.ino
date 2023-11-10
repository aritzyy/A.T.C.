//voltage sensor, current sensor, & digital pot
#include <SPI.h>

static const int spiClk = 1000000;
byte address = 0x00;
SPIClass *hspi = NULL;

const int analogInputPin = A0; // Use the appropriate analog input pin (for voltage)

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
  // Read the analog value
  //int rawValue = analogRead(analogInputPin);
  
  // Calculate the voltage using the voltage divider formula
  // Vout = Vin * (R2 / (R1 + R2))
  //float voltage = (float)rawValue * (30.0 / 1024.0); // 30V is the maximum voltage
  //voltage = voltage * 5.96471;
  // Print the voltage to the serial monitor
  //float current = voltage / 99.64;
  //Serial.print("Voltage: "); 
  //Serial.print(voltage, 3);
  //Serial.print("         Current: ");
  //Serial.print(current, 4);
  //Serial.print("Average Current: "); // print the current with 3 decimal places
  //Serial.print(robojax.getCurrentAverage(300), 3);
  //Serial.println();
  
  //delay(500); // Delay for readability (adjust as needed)


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

float digitalPotWrite(int value)
{
  hspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(53, LOW); // Select the MCP4131
  hspi->transfer(address);
  hspi->transfer(value);
  //for voltage & current sensor
  int rawValue = analogRead(analogInputPin);
  float voltage = (float)rawValue * (30.0 / 1024.0); // 30V is the maximum voltage
  float current = voltage / 99.64;
  Serial.print("Voltage: "); 
  Serial.print(voltage, 3);
  Serial.print("         Current: ");
  Serial.print(current, 4);
  Serial.println();
  //deselect digital pot
  digitalWrite(53, HIGH); // Deselect the MCP4131
  hspi->endTransaction();
}