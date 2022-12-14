#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <Servo.h> // servo library  
#define SensorPin A0  // used for Arduino and ESP8266
//#define SensorPin 4  // used for ESP32
#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701

long duration;
int distanceCm;
float distanceInch;
Servo s1;
Servo s2;
const int trigPin = 12;
const int echoPin = 14;
String jsondata = ""; 
String li = "";
int status = WL_IDLE_STATUS;
WiFiClient client;

 StaticJsonBuffer<200> jsonBuffer;
 JsonObject& root = jsonBuffer.createObject();
   int re = client.connect("192.168.133.227", 80);

void sonarValue(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_VELOCITY/2;
  
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;
  
  // Prints the distance on the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);
}

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  s1.attach(0);
  s2.attach(2);
  WiFi.begin("간절한 임베디드", "1234abcd");
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}


void loop() {
  Serial.println(analogRead(SensorPin));
  sonarValue();
  if(analogRead(SensorPin)>400){
    s1.write(180); 
    s2.write(180);
    root["openStatus"] = true;
  }
  else{
    s1.write(0);
    s2.write(0); 
    root["openStatus"] = false;
  }
  jsondata = "";
  root["garbageAmount"] = distanceCm;
  root.printTo(jsondata);
  re = client.connect("192.168.133.227", 80);
  if (re) {
    client.println("POST /api/sewer/updateSewerInfo/1 HTTP/1.1");
    client.println("Host: 192.168.133.227");
    client.println("Cache-Control: no-cache");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(jsondata.length());
    client.println();
    client.print(jsondata);
    li = client.readString();
    Serial.println(li);
  }
  Serial.println("good");
} 
