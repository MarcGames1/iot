#include <SoftwareSerial.h>

SoftwareSerial espSerial(2, 3); // RX, TX

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600); // Initialize communication with ESP8266 module
}

void loop() {
  int sensorValue1 = analogRead(A0);
  int sensorValue2 = analogRead(A1);
  int sensorValue3 = analogRead(A2);

  float voltage1 = sensorValue1 * (5.0 / 1023.0);
  float voltage2 = sensorValue2 * (5.0 / 1023.0);
  float voltage3 = sensorValue3 * (5.0 / 1023.0);

  String data = String(voltage1, 2) + ";" + String(voltage2, 2) + ";" + String(voltage3, 2);
  
  espSerial.println(data); // Sending data to ESP8266 module
  
  delay(1000); // Wait for a second before the next reading
}
