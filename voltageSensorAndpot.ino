const int analogInputPin = A0; // Use the appropriate analog input pin

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
  float current = voltage / 100;
  Serial.print("Voltage: ");
  Serial.print("Current: ");
  Serial.print(voltage, 3);
  Serial.println(" V");
  
  delay(500); // Delay for readability (adjust as needed)
}