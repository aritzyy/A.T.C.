// This is omar's Notes for the pins
//for the SPI connections: for the (NMOS pot the CS:is pin 53,the dip switch is pin 1 that connects to pin 5 on the arduino)
// for the SPI connections: for the (PMOS pot the CS:is pin 42,the dip switch is pin 2 that connects to pin 7 on the arduino)
// for the SPI connections: for both SCK and SDI are pins (sck:52,sdi:51)
// for the button is pin 33

#include <SPI.h>

static const int spiClk = 1000000;
byte address = 0x00;
SPIClass *hspi = NULL;

//nmos
const int gatePin = A0;
const int drainPin = A1;
const int sourcePin = A2;


//pmos 
const int pgatePin = A3; //change all a0 to a3 for pmos
const int psourcePin = A4; //change all a2 to a4 for pmos
const int pdrainPin = A5; //change all a1 to a5 for pmos

void setup() 
{
  // button to turn on the device
  pinMode(33, INPUT);

  // switch for selection of pmos on the dip switch 2
  pinMode(7, INPUT);

  // switch for selecting nmos on the dip switch 1
  pinMode(5, INPUT);

  //for digital pot for pmos
  pinMode(42, OUTPUT);

  //second dig pot for nmos
  pinMode(53, OUTPUT);

  hspi = new SPIClass(SPI);
  hspi->begin();

  //for voltage sensor
  Serial.begin(9600);
}

void loop() 
{
  int nswitch = digitalRead(5);

  int pswitch = digitalRead(7);

  int button = digitalRead(33);


 if(nswitch == HIGH && pswitch == LOW){ //when button pressed, nmos and pmos activating (not anymore)
    
    if(button == HIGH){
      Serial.println("0");
      //digitalPotWrite(0);
      for (int i = 0; i <= 255; i+=10){
        digitalPotWrite2(i);
        //digitalPotWrite(0);
        delay(100);
      }
    digitalPotWrite2(0);
    //digitalPotWrite(0);
    }
    
  }

  else if(pswitch == HIGH && nswitch == LOW){ //only pmos activating with this code
    
    if(button == HIGH){
      Serial.println("1");
      digitalWrite(53, HIGH);
      for (int i = 0; i <= 255; i+=5){
        digitalPotWrite(i);
        delay(100);
      }
    digitalPotWrite(0);
    }
  }
  // else if(nswitch == HIGH && pswitch == LOW){ //when button pressed, nmos and pmos activating
  //   if(button == HIGH){
  //     for (int i = 0; i <= 255; i+=10){
  //       digitalPotWrite2(i);
  //       delay(100);
  //     }
  //   digitalPotWrite2(0);
  //   }
  // }
}

float digitalPotWrite(int value) //pmos
{
  hspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  //select digital pot
  digitalWrite(42, LOW); 
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
  
  Serial.print("PGVoltage:"); 
  Serial.print(pvoltageGate, 3); //1.1725 v raw value: 282
  Serial.print("        PDVoltage:"); 
  Serial.print(pvoltageDrain, 3);
  Serial.print("        PSVoltage:"); 
  Serial.print(pvoltageSource, 3);
  Serial.print("          PVGS:");
  Serial.print(pVGS, 3);
  Serial.print("         PCurrent:");
  Serial.print(pcurrent, 4); // Adjusted to make it negative
  Serial.println();
  
  //deselect digital pot
  digitalWrite(42, HIGH); 
  hspi->endTransaction();
}

float digitalPotWrite2(int value) //nmos
{
  hspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  //select digital pot
  digitalWrite(53, LOW); 
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
  digitalWrite(53, HIGH); 
  hspi->endTransaction();
}