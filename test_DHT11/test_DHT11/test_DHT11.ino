
#include <DHT_U.h>
#define DHT_PIN 15
#define SENSOR DHT11
float hum_read = 0.0;
float temp_read = 0.0;
DHT dht(DHT_PIN, SENSOR);
void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  hum_read = dht.readHumidity();
  temp_read = dht.readTemperature();
  Serial.print("HUM: ");
  Serial.println(hum_read);

  delay(200);
}