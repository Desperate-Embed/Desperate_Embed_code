#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

String jsondata = ""; 
String li = "";
int status = WL_IDLE_STATUS;
WiFiClient client;

 StaticJsonBuffer<200> jsonBuffer;
 JsonObject& root = jsonBuffer.createObject();
   int re = client.connect("192.168.133.227", 80);

void setup() {
  Serial.begin(115200);
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
  jsondata = "";
  root["openStatus"] = true;
  root["garbageAmount"] = 30;
  root.printTo(jsondata);
  re = client.connect("192.168.133.227", 80);
  if (re) {
    client.println("POST /sewer/updateSewerInfo/1 HTTP/1.1");
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
