#include <Arduino.h>
#include <ArduinoJson.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266WiFiMulti.h>

#include <Time.h>
#include <TimeLib.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#include <PubSubClient.h>
#include <ESP8266HTTPClient.h>

#define MAX_MSG_LEN (256)

// MQTT Configuration
// if you have a hostname set for the MQTT server, you can use it here
const char *serverHostname = "your_server_external_ip";
// otherwise you can use an IP address like this
//const IPAddress serverIPAddress(192.168.0.14);
// the topic we want to use
const char *helloTopic = "client/connected";
const char *topic = "get/gpsdata";
const char *outTopic = "send/gpsdata";
const char *outTopicDataErr = "data/notready";

static const int RXPin = 13, TXPin = 15;
static const uint32_t GPSBaud = 9600;

double lati;
double longi;
double latiOld;
double longiOld;
String data;
boolean dataEncoded = false;
double distance;

const double offset = 5.5;

// The TinyGPS++ object
TinyGPSPlus gps;
TinyGPSPlus gpsOld;
TinyGPSPlus dis;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);
ESP8266WiFiMulti WiFiMulti;
WiFiClient client;
PubSubClient pubClient(client);
HTTPClient http;
 void connection () {
  
  Serial.begin(9600);
  //  Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    //delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("Tech_D0065336", "01efcae9f2aeafe2f3b4613f0d");
  ss.begin(GPSBaud);

  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("[HTTP] begin...\n");

  pubClient.setServer(serverHostname, 1883);
  pubClient.setCallback(callback);
  Serial.print("PubSub client ready...\n");
}

void connectMQTT() {
  // Wait until we're connected
  while (!pubClient.connected()) {
    // Create a random client ID
    String clientId = "ESP8266-A9001";
    Serial.printf("MQTT connecting as client %s...\n", clientId.c_str());
    // Attempt to connect
    if (pubClient.connect(clientId.c_str())) {
      Serial.println("MQTT connected");
      // Once connected, publish an announcement...
      pubClient.publish(helloTopic, "hello from ESP8266");
      // ... and resubscribe
      pubClient.subscribe(topic);
    } else {
      Serial.printf("MQTT failed, state %s, retrying...\n", pubClient.state());
      // Wait before retrying
      delay(2500);
    }
  }
}

void callback(char *msgTopic, byte *msgPayload, unsigned int msgLength) {
  // copy payload to a static string
  static char message[MAX_MSG_LEN + 1];
  if (msgLength > MAX_MSG_LEN) {
    msgLength = MAX_MSG_LEN;
  }
  strncpy(message, (char *)msgPayload, msgLength);
  message[msgLength] = '\0';

  Serial.printf("topic %s, message received: %s\n", topic, message);

  // decode message
  if (strcmp(message, "send") == 0) {
    Serial.println("topic correct \n");
    if (dataEncoded) {
      Serial.println(data);
      char charBuff[data.length()];
      data.toCharArray(charBuff, data.length());
      pubClient.publish(outTopic, charBuff);
      Serial.printf("gps data published \n");
      dataEncoded = false;
    }
    else {
      pubClient.publish(outTopicDataErr, "null");
      Serial.printf("gps data not ready \n");
    }
  }
}

#define ENA   14          // Enable/speed motors Right        GPIO14(D5)
#define ENB   12          // Enable/speed motors Left         GPIO12(D6)
#define IN_1  15          // L298N in1 motors Right           GPIO15(D8)
#define IN_2  13          // L298N in2 motors Right           GPIO13(D7)
#define IN_3  2           // L298N in3 motors Left            GPIO2(D4)
#define IN_4  0           // L298N in4 motors Left            GPIO0(D3)

String command;             
int speedCar = 800;         // 400 - 1023.
int speed_Coeff = 3;

const char* ssid = "NodeMCU Car";
ESP8266WebServer server(80);

void setup() {
 
 pinMode(ENA, OUTPUT);
 pinMode(ENB, OUTPUT);  
 pinMode(IN_1, OUTPUT);
 pinMode(IN_2, OUTPUT);
 pinMode(IN_3, OUTPUT);
 pinMode(IN_4, OUTPUT); 
  
  Serial.begin(115200);
  
// Connecting WiFi

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
 
 // Starting WEB-server 
     server.on ( "/", HTTP_handleRoot );
     server.onNotFound ( HTTP_handleRoot );
     server.begin();    
}

void goAhead(){ 

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar);
  }

void goBack(){ 

      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
  }

void goRight(){ 

      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar);
  }

void goLeft(){

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
  }

void goAheadRight(){
      
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar/speed_Coeff);
 
      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar);
   }

void goAheadLeft(){
      
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar/speed_Coeff);
  }

void goBackRight(){ 

      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar/speed_Coeff);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
  }

void goBackLeft(){ 

      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar/speed_Coeff);
  }

void stopRobot(){  

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
 }

void loop() {
    server.handleClient();
    
      command = server.arg("State");
      if (command == "F") goAhead();
      else if (command == "B") goBack();
      else if (command == "L") goLeft();
      else if (command == "R") goRight();
      else if (command == "I") goAheadRight();
      else if (command == "G") goAheadLeft();
      else if (command == "J") goBackRight();
      else if (command == "H") goBackLeft();
      else if (command == "0") speedCar = 400;
      else if (command == "1") speedCar = 470;
      else if (command == "2") speedCar = 540;
      else if (command == "3") speedCar = 610;
      else if (command == "4") speedCar = 680;
      else if (command == "5") speedCar = 750;
      else if (command == "6") speedCar = 820;
      else if (command == "7") speedCar = 890;
      else if (command == "8") speedCar = 960;
      else if (command == "9") speedCar = 1023;
      else if (command == "S") stopRobot();
}

void HTTP_handleRoot(void) {

if( server.hasArg("State") ){
       Serial.println(server.arg("State"));
  }
  server.send ( 200, "text/html", "" );
  delay(1);
}
