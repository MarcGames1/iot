#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

const char* ssid = "IoTHotspot"; 
const char* password = "27122023";

const char* host = "script.google.com";
const int httpsPort = 443;

WiFiClientSecure client;

String GAS_ID = "AKfycbz8FT8zGOOZXssnwzSj5RxaC6OlZMfuVLWrPNGtmyCDRwoZLi4_BUSEXOjFetY-7Bc"; // Your Google Sheets script ID

void setup() {
  Serial.begin(9600);
  delay(500);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

  client.setInsecure();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    if (Serial.available() > 0) {
      String data = Serial.readStringUntil('\n');

      sendDataToGoogleSheet(data);
    }
  } else {
    Serial.println("WiFi Disconnected. Reconnecting...");
    WiFi.begin(ssid, password);
  }

  delay(1000); // Wait a second before checking for data from Arduino again
}

void sendDataToGoogleSheet(String data) {
  Serial.println("Connecting to Google Sheets...");

  if (!client.connect(host, httpsPort)) {
    Serial.println("Connection to Google Sheets failed.");
    return;
  }

  String url = "/macros/s/" + GAS_ID + "/exec?data=" + data;
  Serial.print("Requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: ESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("Request sent.");

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("Headers received.");
      break;
    }
  }

  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("Data sent successfully to Google Sheets!");
  } else {
    Serial.println("Failed to send data to Google Sheets.");
  }

  client.stop();
}