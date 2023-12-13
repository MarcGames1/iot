// #include <ESP8266WiFi.h>
// #include <WiFiClientSecure.h>
// #include <NTPClient.h>
// #include <WiFiUdp.h>
// #define LED D1

// const char* ssid = "IoTHotspot"; //--> Your wifi name or SSID.
// const char* password = "27122023"; //--> Your wifi password.


// //----------------------------------------Host & httpsPort
// const char* host = "script.google.com";
// const int httpsPort = 443;
// //----------------------------------------

// WiFiClientSecure client; //--> Create a WiFiClientSecure object.

// String GAS_ID = "1MSPP8nVVDIIkAQtCLP7TDcQKnP7DU1ez3IVWxoOKg9U"; //--> spreadsheet script ID

// const long utcOffsetInSeconds = 3600;
// WiFiUDP ntpUDP;
// NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

// enum WiFiState {
//   WiFi_Idle,
//   WiFi_Connecting,
//   WiFi_Connected
// };

// WiFiState wifiState = WiFi_Idle;

// void setup() {
//   // put your setup code here, to run once:
//   Serial.begin(115200);
//   delay(500);

 
//   delay(500);
//   pinMode(LED, OUTPUT);
//   WiFi.begin(ssid, password); //--> Connect to your WiFi router
//   Serial.println("");
    
//   //----------------------------------------Wait for connection
//   Serial.print("Connecting");


//   client.setInsecure();
// wifiState = WiFi_Connecting;
// }

// void loop() {
//   if (wifiState == WiFi_Connecting) {
//     if (WiFi.status() == WL_CONNECTED) {
//       Serial.println("Connected to WiFi!");
//       wifiState = WiFi_Connected;

//   timeClient.begin();
//    Serial.print("IP address: ");
//       Serial.println(WiFi.localIP());
//     } else if (WiFi.status() != WL_CONNECT_FAILED && WiFi.status() != WL_IDLE_STATUS && WiFi.status() != WL_NO_SSID_AVAIL) {
//       Serial.println("Connection failed.");
//       delay(5000); // Wait before retrying connection
//       WiFi.begin(ssid, password); // Retry connection
//       Serial.println("Retrying connection...");
//     }
//   }
//   int sensorValue = analogRead(A0);   // read the input on analog pin 0

// 	float voltage = sensorValue * (5.0 / 1023.0);   

// 	Serial.println(voltage);   // print out the value you read
  
//   // Check if any reads failed and exit early (to try again).
//   Serial.print(timeClient.getHours());
//   Serial.print(":");
//   Serial.print(timeClient.getMinutes());
//   Serial.print(":");
//   Serial.println(timeClient.getSeconds());

//   if((timeClient.getHours()==8||timeClient.getHours()==14||timeClient.getHours()==18)&&timeClient.getMinutes()==0)
//   {
//   sendData(voltage); //--> Calls the sendData Subroutine
//    digitalWrite(LED, HIGH);
//    delay(1000);          // wait for 1 second.
//    digitalWrite(LED, LOW); // turn the LED on.
//    delay(1000); 
//   }
// }

// // Subroutine for sending data to Google Sheets
// void sendData(float voltage) {
//   Serial.println("==========");
//   Serial.print("connecting to ");
//   Serial.println(host);
  
//   //----------------------------------------Connect to Google host
//   if (!client.connect(host, httpsPort)) {
//     Serial.println("connection failed");
//     return;
//   }
//   //----------------------------------------

//   //----------------------------------------Processing data and sending data
//   String string_voltage =  String(voltage);
//   Serial.println(string_voltage);

//   String url = "/macros/s/" + GAS_ID + "/exec?light=" + string_voltage;
//   Serial.print("requesting URL: ");
//   Serial.println(url);

//   client.print(String("GET ") + url + " HTTP/1.1\r\n" +
//          "Host: " + host + "\r\n" +
//          "User-Agent: BuildFailureDetectorESP8266\r\n" +
//          "Connection: close\r\n\r\n");

//   Serial.println("request sent");
//   //----------------------------------------

//   //----------------------------------------Checking whether the data was sent successfully or not
//   while (client.connected()) {
//     String line = client.readStringUntil('\n');
//     if (line == "\r") {
//       Serial.println("headers received");
//       break;
//     }
//   }
//   String line = client.readStringUntil('\n');
//   if (line.startsWith("{\"state\":\"success\"")) {
//     Serial.println("esp8266/Arduino CI successfull!");
//   } else {
//     Serial.println("esp8266/Arduino CI has failed");
//   }
//   Serial.print("reply was : ");
//   Serial.println(line);
//   Serial.println("closing connection");
//   Serial.println("==========");
//   Serial.println();
//   //----------------------------------------
// }











///////////////////////////////////////////////////////////////////////////////////





// #include <ESP8266WiFi.h>
// #include <WiFiClientSecure.h>
// #include <NTPClient.h>
// #include <WiFiUdp.h>

// #define LED D1

// const char* ssid = "IoTHotspot"; // Your WiFi name or SSID.
// const char* password = "27122023"; // Your WiFi password.

// const char* host = "script.google.com";
// const int httpsPort = 443;

// WiFiClientSecure client; // Create a WiFiClientSecure object.

// String GAS_ID = "YourScriptID"; // Spreadsheet script ID

// const long utcOffsetInSeconds = 3600;
// WiFiUDP ntpUDP;
// NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

// bool isConnected = false;



// void setup() {
//   Serial.begin(115200);
//   delay(500);

//   pinMode(LED, OUTPUT);

//   WiFi.begin(ssid, password); // Connect to your WiFi router
//   Serial.println("Connecting to WiFi...");

  
// }


// // Subroutine for sending data to Google Sheets
// void sendData(float voltage) {
//   Serial.println("==========");
//   Serial.print("connecting to ");
//   Serial.println(host);
  
//   //----------------------------------------Connect to Google host
//   if (!client.connect(host, httpsPort)) {
//     Serial.println("connection failed");
//     return;
//   }
//   //----------------------------------------

//   //----------------------------------------Processing data and sending data
//   String string_voltage =  String(voltage);
//   Serial.println(string_voltage);

//   String url = "/macros/s/" + GAS_ID + "/exec?light=" + string_voltage;
//   Serial.print("requesting URL: ");
//   Serial.println(url);

//   client.print(String("GET ") + url + " HTTP/1.1\r\n" +
//          "Host: " + host + "\r\n" +
//          "User-Agent: BuildFailureDetectorESP8266\r\n" +
//          "Connection: close\r\n\r\n");

//   Serial.println("request sent");
//   //----------------------------------------

//   //----------------------------------------Checking whether the data was sent successfully or not
//   while (client.connected()) {
//     String line = client.readStringUntil('\n');
//     if (line == "\r") {
//       Serial.println("headers received");
//       break;
//     }
//   }
//   String line = client.readStringUntil('\n');
//   if (line.startsWith("{\"state\":\"success\"")) {
//     Serial.println("esp8266/Arduino CI successfull!");
//   } else {
//     Serial.println("esp8266/Arduino CI has failed");
//   }
//   Serial.print("reply was : ");
//   Serial.println(line);
//   Serial.println("closing connection");
//   Serial.println("==========");
//   Serial.println();
//   //----------------------------------------
// }

// void loop() {
//      Serial.println("IS Connected is " );
//      Serial.println(isConnected );
//   if (!isConnected) {
//     if (WiFi.status() == WL_CONNECTED) {
//       Serial.println("Connected to WiFi!");
//      isConnected = true;

//       timeClient.begin();
//       Serial.print("IP address: ");
//       Serial.println(WiFi.localIP());
//     } else if (WiFi.status() != WL_CONNECT_FAILED && WiFi.status() != WL_IDLE_STATUS && WiFi.status() != WL_NO_SSID_AVAIL) 
//     {
//       Serial.println("Connection failed.");
//       delay(5000); // Wait before retrying connection
//       WiFi.begin(ssid, password); // Retry connection
//       Serial.println("Retrying connection...");
//     }
//   }

//   if (isConnected) {
//     Serial.print("YEEEEEE!!!!!");
//     timeClient.update();
//     int sensorValue = analogRead(A0);
//     float voltage = sensorValue * (5.0 / 1023.0);
//     Serial.println(voltage); 

//     // Check if any reads failed and exit early (to try again).
//   Serial.print(timeClient.getHours());
//   Serial.print(":");
//   Serial.print(timeClient.getMinutes());
//   Serial.print(":");
//   Serial.println(timeClient.getSeconds());
//     // ... rest of your code ...
//       if((timeClient.getHours()==8||timeClient.getHours()==14||timeClient.getHours()==18)&&timeClient.getMinutes()==0)
//   {
//   sendData(voltage); //--> Calls the sendData Subroutine
//    digitalWrite(LED, HIGH);
//    delay(1000);          // wait for 1 second.
//    digitalWrite(LED, LOW); // turn the LED on.
//    delay(1000); 
//   }
// }
// }
   


/////////////////////////////////////////////////////////////////////////



#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define LED D1

const char* ssid = "IoTHotspot"; // Nazwa twojej sieci WiFi.
const char* password = "27122023"; // Hasło do twojej sieci WiFi.

const char* host = "script.google.com";
const int httpsPort = 443;

WiFiClientSecure client; // Utworzenie obiektu WiFiClientSecure.

String GAS_ID = "YourScriptID"; // ID skryptu w Google Sheets

const long utcOffsetInSeconds = 3600;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

bool isConnected = false;



void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(LED, OUTPUT);

  WiFi.begin(ssid, password); // Connect to your WiFi router
  Serial.println("Connecting to WiFi...");

  
}

void sendData(float voltage1, float voltage2, float voltage3) {
  Serial.println("==========");
  Serial.print("Connecting to ");
  Serial.println(host);

//----------------------------------------Connect to Google host
  if (!client.connect(host, httpsPort)) {
    Serial.println("Connection failed");
    return;
  }
//----------------------------------------

  //----------------------------------------Processing data and sending data
  String string_voltage =  String(voltage);
  Serial.println(string_voltage);

  String url = "/macros/s/" + GAS_ID + "/exec?light1=" + String(voltage1) +
               "&light2=" + String(voltage2) + "&light3=" + String(voltage3);
  Serial.print("Requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("Request sent");

//----------------------------------------Checking whether the data was sent successfully or not
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("Headers received");
      break;
    }
  }

  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("ESP8266/Arduino CI successful!");
  } else {
    Serial.println("ESP8266/Arduino CI has failed");
  }

  Serial.print("Reply was: ");
  Serial.println(line);
  Serial.println("Closing connection");
  Serial.println("==========");
  Serial.println();

}

void loop() {
  Serial.println("Is connected: ");
  Serial.println(isConnected);

  if (!isConnected) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Connected to WiFi!");
      isConnected = true;

      timeClient.begin();
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
    } else if (WiFi.status() != WL_CONNECT_FAILED && WiFi.status() != WL_IDLE_STATUS && WiFi.status() != WL_NO_SSID_AVAIL) {
      Serial.println("Connection failed.");
      delay(5000);
      WiFi.begin(ssid, password);
      Serial.println("Retrying connection...");
    }
  }

  if (isConnected) {
    Serial.print("YEEEEEE!!!!!");
    timeClient.update();
    int sensorValue1 = analogRead(A0);  //analog A0
    int sensorValue2 = analogRead(2);   //GPIO2
    int sensorValue3 = analogRead(4);   //gpio4 

    float voltage1 = sensorValue1 * (5.0 / 1023.0);
    float voltage2 = sensorValue2 * (5.0 / 1023.0);
    float voltage3 = sensorValue3 * (5.0 / 1023.0);

    Serial.print("Voltage 1: ");
    Serial.println(voltage1);
    Serial.print("Voltage 2: ");
    Serial.println(voltage2);
    Serial.print("Voltage 3: ");
    Serial.println(voltage3);

    Serial.print(timeClient.getHours());
    Serial.print(":");
    Serial.print(timeClient.getMinutes());
    Serial.print(":");
    Serial.println(timeClient.getSeconds());

    if ((timeClient.getHours() == 8 || timeClient.getHours() == 14 || timeClient.getHours() == 18) && timeClient.getMinutes() == 0) {
      sendData(voltage1, voltage2, voltage3);
      digitalWrite(LED, HIGH);
      delay(1000);
      digitalWrite(LED, LOW);
      delay(1000);
    }
  }
}




