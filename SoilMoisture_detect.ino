#include <WiFi.h>
#include <Wire.h>    
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>            
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

const char *ssid =  "vivo 1920";     // Enter your WiFi Name
const char *pass =  "shafi1234"; // Enter your WiFi Password

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64  

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

WiFiClient client;

#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "hussshafiul" // Your Adafruit IO Username
#define MQTT_PASS "aio_kijZ91qBa41Pq4UleAd90EYQLnyV" //  Your Adafruit IO AIO key

const int Sensor_pin = 36;             
int Sensor_value;        
    
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);
Adafruit_MQTT_Publish Sensor_data = Adafruit_MQTT_Publish(&mqtt, MQTT_NAME "/f/ESP32_Soil_Moisture_Monitor");  

void setup()
{
  Serial.begin(115200);
  delay(10);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");            
  }
  Serial.println("");
  Serial.println("WiFi connected");

display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}
 void loop()
{
    MQTT_connect();
  Sensor_value = ( 100 - ( (analogRead(Sensor_pin) / 1023.00) * 100 ) );

  Serial.print("Soil Moisture is  = ");
  Serial.print(Sensor_value);
  Serial.println("%");

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("Soil");
  display.setCursor(0, 30);
  display.println("Moisture");
  display.setCursor(0, 50);
  display.println(Sensor_value);
  display.setCursor(40, 50);
  display.println("%");
  display.display(); 
 
       if (! Sensor_data.publish(Sensor_value)) 
       {                     
         delay(5000);   
          }
 delay(6000);
}

void MQTT_connect() {

  int8_t again;

  if (mqtt.connected()) {

    return;

  }

 Serial.print("Connecting to Adafruit IO");

uint8_t retry = 5;

  while ((again = mqtt.connect()) != 0) { 

       Serial.println(mqtt.connectErrorString(again));

       Serial.println("Retrying Adafruit connection in 5 seconds...");

       mqtt.disconnect();

       delay(5000);  

       retry--;

       if (retry == 0) {

         while (1);

       }

  }
  Serial.println("");

  Serial.println("Adafruit IO is Connected!");

}
