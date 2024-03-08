#include <SPI.h>

static const int spiClk = 1000000;
byte address = 0x00;
SPIClass *hspi = NULL;

//nmos
const int gatePin = A0;
const int sourcePin = A2;
const int drainPin = A1;

//pmos 
const int pgatePin = A3; //change all a0 to a3 for pmos
const int psourcePin = A4; //change all a2 to a4 for pmos
const int pdrainPin = A5; //change all a1 to a5 for pmos

void setup() 
{
  // button to turn on the device
  pinMode(6, INPUT);
  // switch for selection of pmos
  pinMode(7, INPUT);
  // switch for selecting nmos
  pinMode(5, INPUT);
  //for digital pot for nmos
  pinMode(10, OUTPUT);
  //second dig pot for pmos
  pinMode(8, OUTPUT);

  hspi = new SPIClass(SPI);
  hspi->begin();

  //for voltage sensor
  Serial.begin(9600);
}

void loop() 
{
  int nswitch = digitalRead(5);

  int pswitch = digitalRead(7);

  int button = digitalRead(6);
  if(pswitch == HIGH)
  {
  if(button == HIGH)
  {
  for (int i = 0; i <= 255; i++)
  {
    digitalPotWrite(i);
    delay(100);
  }
  digitalPotWrite(0);
  }
  }
  else if(nswitch == HIGH)
  {
    if(button == HIGH)
    {
    for (int i = 0; i <= 255; i++)
    {
      digitalPotWrite2(i);
      delay(100);
    }
    digitalPotWrite2(0);
    }
  }
}

float digitalPotWrite(int value) //pmos
{
  hspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  //select digital pot
  digitalWrite(10, LOW); 
  hspi->transfer(address);
  hspi->transfer(value);

  //for voltage & current sensor
  //for gate pin with inverting op-amp (Assuming it provides a negative voltage)
  int rawValue = analogRead(pgatePin);
  float pvoltageGate = (float)rawValue * (-30.0 / 1024.0); // Adjusted to negative

  //for drain pin with inverting op-amp (Assuming it provides a negative voltage)
  int rawValue2 = analogRead(pdrainPin);
  float pvoltageDrain = (float)rawValue2 * (-30.0 / 1024.0); // Adjusted to negative

  //for source pin with inverting op-amp (Assuming it provides a negative voltage)
  int rawValue3 = analogRead(psourcePin);
  float pvoltageSource = (float)rawValue3 * (-30.0 / 1024.0); // Adjusted to negative

  //calculate current (Note: Current direction is opposite for PMOS)
  float pcurrent = pvoltageSource / 99.09; // Current Measurement

  //calculate VSG (Note: VSG is the negative of what it is for NMOS)
  float pVGS = pvoltageGate - pvoltageSource;

  Serial.print("GVoltage:"); 
  Serial.print(pvoltageGate, 3); //1.1725 v raw value: 282
  Serial.print("        DVoltage:"); 
  Serial.print(pvoltageDrain, 3);
  Serial.print("        SVoltage:"); 
  Serial.print(pvoltageSource, 3);
  Serial.print("          VGS:");
  Serial.print(pVGS, 3);
  Serial.print("         Current:");
  Serial.print(pcurrent, 4); // Adjusted to make it negative
  Serial.println();

  //deselect digital pot
  digitalWrite(10, HIGH); 
  hspi->endTransaction();
}

float digitalPotWrite2(int value) //nmos
{
  hspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  //select digital pot
  digitalWrite(8, LOW); 
  hspi->transfer(address);
  hspi->transfer(value);

  //for voltage & current sensor
  //for gate pin with inverting op-amp (Assuming it provides a negative voltage)
  int rawValue = analogRead(gatePin);
  float voltageGate = (float)rawValue * (30.0 / 1024.0); // Adjusted to negative

  //for drain pin with inverting op-amp (Assuming it provides a negative voltage)
  int rawValue2 = analogRead(drainPin);
  float voltageDrain = (float)rawValue2 * (30.0 / 1024.0); // Adjusted to negative

  //for source pin with inverting op-amp (Assuming it provides a negative voltage)
  int rawValue3 = analogRead(sourcePin);
  float voltageSource = (float)rawValue3 * (30.0 / 1024.0); // Adjusted to negative

  //calculate current (Note: Current direction is opposite for PMOS)
  float current = voltageSource / 99.64; // Current Measurement

  //calculate VSG (Note: VSG is the negative of what it is for NMOS)
  float VGS = voltageGate - voltageSource;

  Serial.print("GVoltage:"); 
  Serial.print(voltageGate, 3); //1.1725 v raw value: 282
  Serial.print("        DVoltage:"); 
  Serial.print(voltageDrain, 3);
  Serial.print("        SVoltage:"); 
  Serial.print(voltageSource, 3);
  Serial.print("          VGS:");
  Serial.print(VGS, 3);
  Serial.print("         Current:");
  Serial.print(current, 4); // Adjusted to make it negative
  Serial.println();

  //deselect digital pot
  digitalWrite(8, HIGH); 
  hspi->endTransaction();
}