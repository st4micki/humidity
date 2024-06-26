//ESP-NOW_begin
#include <esp_now.h>
#include <WiFi.h>
//ESP-NOW_end
//DHT-SENSOR_begin
#include <DHT_U.h>
#define DHT_PIN 15
#define SENSOR DHT11
//DHT-SENSOR_end
//ESP-NOW_begin
uint8_t broadcastAddress[] = {0x40, 0x91, 0x51, 0xFC, 0xCA, 0xA4};

typedef struct struct_message{
  int id;
  float data;
} struct_message;

struct_message messageData;
esp_now_peer_info_t peerInfo;
//ESP-NOW_end
//DHT-SENSOR_begin
float hum_read = 0.0;
float temp_read = 0.0;
DHT dht(DHT_PIN, SENSOR);
//DHT-SENSOR_end
void setup() {
//ESP-NOW_begin
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if(esp_now_init() != ESP_OK){
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  else{
    Serial.println("ESP-NOW initialized");
  }

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if(esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  else{
    Serial.print("Peer added. MAC:");
    for(int i  =  0; i < sizeof(broadcastAddress)/sizeof(broadcastAddress[0]); i++){
      Serial.print(broadcastAddress[i]);
    }
    Serial.print("\n");
  }
//ESP-NOW_end
//DHT-SENSOR_begin
  dht.begin();
//DHT-SENSOR_end
}

void loop() {
//DHT-SENSOR_begin
  hum_read = dht.readHumidity();
  temp_read = dht.readTemperature();
  Serial.print("HUM: ");
  Serial.println(hum_read);
//DHT-SENSOR_end
  messageData.id = 1;
  messageData.data = hum_read;
//ESP-NOW_begin
  esp_now_send(broadcastAddress, (uint8_t *) &messageData, sizeof(messageData));
  delay(10000);

}
