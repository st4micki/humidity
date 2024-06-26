#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char *ssid = "NaszaSiec.NET_4C8FA69F";
const char *password = "kahsh5ee"; 


WebServer server(80);

void handleRoot() {
  char msg[1500];

  snprintf(msg, 1500,
           "<!DOCTYPE html>\
<html>\
<head>\
    <title>Humidity Server</title>\
</head>\
<body>\
    <h1>Pokoj 0</h1>\
    <p>%d</p>\
</body>\
</html>",readDHTHumidity());
  server.send(200, "text/html", msg);
}


void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.print("IP adress: ");
  Serial.println(WiFi.localIP());
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");

}

void loop() {
  server.handleClient();
  delay(100);

}

int readDHTTemperature(){
  int variable;
  variable = 90;
  return variable;
}

int readDHTHumidity(){
  int variable;
  variable = 90;
  return variable;
}