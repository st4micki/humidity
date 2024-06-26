#include <esp_now.h>
#include <WiFi.h>
#include <DHT_U.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#define DHT_PIN 15
#define SENSOR DHT11
#define LED_PIN 2
//ESP-NOW_begin
typedef struct struct_message{
  int id;
  float data;
} struct_message;
struct_message messageData;
struct_message board1;
float hum_rec1 = 0.0;
//tutaj dodac kolejne plytki
struct_message boardsStruct[] = {board1};


void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len){
  Serial.print("Packet received from: ");
  memcpy(&messageData, incomingData, sizeof(messageData));
  Serial.printf("Board ID %u: %u bytes\n", messageData.id, len);
  boardsStruct[messageData.id-1].data = messageData.data;
  Serial.printf("x value: %f \n", boardsStruct[messageData.id-1].data);
  Serial.println();
  digitalWrite(LED_PIN,HIGH);
  delay(10);
  digitalWrite(LED_PIN,LOW);
  


}
//ESP-NOW_end
//DHT-SENSOR_begin
float hum_read = 0.0;
float temp_read = 0.0;
DHT dht(DHT_PIN, SENSOR);
//DHT-SENSOR_end
//SERVER_begin
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
    <meta http-equiv='refresh' content='4'/>\
</head>\
<body>\
    <h1>Pokoj 0</h1>\
    <p>%.2f</p>\
    <h1>Pokoj 1</h1>\
    <p>%.2f</p>\
</body>\
</html>",read_humidity(), receive_humidity());
  server.send(200, "text/html", msg);
//SERVER_end
}

void setup() {
  pinMode(LED_PIN,OUTPUT);
//SERVER_begin
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(490);
    Serial.print(".");
    digitalWrite(LED_PIN,HIGH);
    delay(10);
    digitalWrite(LED_PIN,LOW);
  }
  Serial.print("\nIP adress: ");
  Serial.println(WiFi.localIP());
  if (MDNS.begin("wilgoc")){
    Serial.println("MDNS responder started");
  }
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");


//SERVER_end
//ESP-NOW_begin
  if(esp_now_init() != ESP_OK){
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  else{
    Serial.println("ESP-NOW initialized");
  }
  esp_now_register_recv_cb(OnDataRecv);
//ESP-NOW_end
//DHT-SENSOR_begin
  dht.begin();
//DHT-SENSOR_end
}

void loop() {
  server.handleClient();
  delay(1000);

}

float receive_humidity(){
  float h = boardsStruct[0].data;
  return h;
}

float read_humidity(){
  float h = dht.readHumidity();
  return h;
}
