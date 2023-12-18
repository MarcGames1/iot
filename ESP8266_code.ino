#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define LED D1

float light1 = 0.0;
float light2 = 0.0;
float light3 = 0.0;

bool newData = false;
String inputString = "";

const char* ssid = "Robert Kubica"; 
const char* password = "rafarafa1"; 

const char* host = "script.google.com";
const int httpsPort = 443;

WiFiClientSecure client; // Create a WiFiClientSecure object.

String GAS_ID = "AKfycbzZFXzscDUDQ-Uw2IqU4zk_exYnQm0PhVfzS_5uZ1hHIFe6TGc2LngrvLaQhRG5g15AQg"; // Spreadsheet script ID

const long utcOffsetInSeconds = 3600;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

bool isConnected = false;
unsigned long lastConnectionTime = 0;
const unsigned long connectionInterval = 20 * 1000; // Interval for sending data (milliseconds)

void setup() {
  Serial.begin(115200);
  delay(500);
  pinMode(LED, OUTPUT);
  WiFi.begin(ssid, password); // Connect to your WiFi router
  Serial.println("Connecting to WiFi...");
  client.setInsecure();
}

void sendData(float light1, float light2, float light3) {
  Serial.println("==========");
  Serial.print("connecting to ");
  Serial.println(host);

  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed on google ");
    return;
  }

  String string_light1 = String(light1);
  String string_light2 = String(light2);
  String string_light3 = String(light3);

  String url = "/macros/s/" + GAS_ID + "/exec?light1=" + string_light1 + "&light2=" + string_light2 + "&light3=" + string_light3;
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }

  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successful!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.print("reply was : ");
  Serial.println(line);
  Serial.println("closing connection");
  Serial.println("==========");
  Serial.println();
  client.stop(); // Close the connection
}

void parseData() {
  int comma1Index = inputString.indexOf(',');
  int comma2Index = inputString.lastIndexOf(',');

  if (comma1Index != -1 && comma2Index != -1 && comma1Index != comma2Index) {
    String light1Str = inputString.substring(0, comma1Index);
    String light2Str = inputString.substring(comma1Index + 1, comma2Index);
    String light3Str = inputString.substring(comma2Index + 1);

    light1 = light1Str.toFloat();
    light2 = light2Str.toFloat();
    light3 = light3Str.toFloat();

    Serial.print("light1: ");
    Serial.print(light1);
    Serial.print(" light2: ");
    Serial.print(light2);
    Serial.print(" light3: ");
    Serial.println(light3);

    newData = true; // Set flag for new data received
  }

  inputString = ""; // Clear the inputString
}

void loop() {
  unsigned long currentMillis = millis();

  while (Serial.available() > 0) {
    char receivedChar = Serial.read(); // Read incoming serial data

    if (receivedChar != '\n') {
      inputString += receivedChar;
    } else {
      parseData(); // Parse received data
    }
  }

  if (newData && isConnected) {
    sendData(light1, light2, light3); // Send data if new data available and connected
    newData = false; // Reset new data flag
  }

  if (!isConnected) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Connected to WiFi!");
      isConnected = true;

      timeClient.begin();
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
    } else if (currentMillis - lastConnectionTime >= connectionInterval) {
      lastConnectionTime = currentMillis;
      WiFi.begin(ssid, password); // Retry WiFi connection
      Serial.println("Retrying WiFi connection...");
    }
  }

  if (isConnected && timeClient.update()) {
    digitalWrite(LED, HIGH); // Turn on LED
    delay(1000);
    digitalWrite(LED, LOW); // Turn off LED
    delay(1000);
  }
}
