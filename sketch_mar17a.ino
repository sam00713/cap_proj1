#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFiMulti.h>

#define ONE_WIRE_BUS D2  

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
#define server "api.thingspeak.com"
String apiKey = "W65FL8EO8XU4QKED";
#ifndef STASSID
#define STASSID  "AndroidHotspot8939";
#define STAPSK "12344321"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "192.168.1.1";
const uint16_t port = 80;
ESP8266WiFiMulti WiFiMulti;
int sent=0;
void setup() {
  Serial.begin(115200);
  connectWifi();
}

void loop() {
  float temp;
  //char buffer[10];
  sensors.requestTemperatures(); 
  temp = sensors.getTempCByIndex(0);
  Serial.print(String(sent)+" Temperature: ");
  Serial.println(temp);
  
  
  sendTeperatureTS(temp);
  int count =20;
  while(count--)
  delay(1000);
}

void connectWifi()
{
  Serial.print("Connecting to "+ *ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("Connected");
  Serial.println("");  
}

void sendTeperatureTS(float temp)
{  
   WiFiClient client;
  
   if (client.connect(server, 80)) { // use ip 184.106.153.149 or api.thingspeak.com
   Serial.println("WiFi Client connected ");
   
   String postStr = apiKey;
   postStr += "&field1=";
   postStr += String(temp);
   postStr += "\r\n\r\n";
   
   client.print("POST /update HTTP/1.1\n");
   client.print("Host: api.thingspeak.com\n");
   client.print("Connection: close\n");
   client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
   client.print("Content-Type: application/x-www-form-urlencoded\n");
   client.print("Content-Length: ");
   client.print(postStr.length());
   client.print("\n\n");
   client.print(postStr);
   delay(1000);
   
   }//end if
   sent++;
 client.stop();
}
