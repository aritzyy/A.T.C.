
//voltage sensor code
//const int VIN = A1; // define the Arduino pin A0 as voltage input (V in)
//const float VCC   = 5.04;// supply voltage
//const int MODEL = 2;   // enter the model (see above list)
//#include <Robojax_AllegroACS_Current_Sensor.h>

const int analogInputPin = A0; // Use the appropriate analog input pin (for voltage)

//Robojax_AllegroACS_Current_Sensor robojax(MODEL,VIN);
void setup() {
  Serial.begin(9600);
}

void loop() {
  // Read the analog value
  int rawValue = analogRead(analogInputPin);
  
  // Calculate the voltage using the voltage divider formula
  // Vout = Vin * (R2 / (R1 + R2))
  float voltage = (float)rawValue * (30.0 / 1024.0); // 30V is the maximum voltage
  //voltage = voltage * 5.96471;
  // Print the voltage to the serial monitor
  float current = voltage / 99.64;
  Serial.print("Voltage: "); 
  Serial.print(voltage, 3);
  Serial.print("         Current: ");
  Serial.print(current, 4);
  //Serial.plot(voltage);
  //Serial.print(robojax.getCurrent(),3);
  //Serial.print("Average Current: "); // print the current with 3 decimal places
  //Serial.print(robojax.getCurrentAverage(300), 3);
  Serial.println();
  
  delay(500); // Delay for readability (adjust as needed)
}