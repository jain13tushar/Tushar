#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WIFI_SSID "k8 note"
#define WIFI_PASS "tu@123456"

#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "sahil24"
#define MQTT_PASS "945bc86aab404b51aa565d1cc842e4dc"

int led = D6;

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);

//for LED
Adafruit_MQTT_Subscribe LightSlider = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/lights");
Adafruit_MQTT_Publish LightDisplay = Adafruit_MQTT_Publish(&mqtt, MQTT_NAME "/f/Lts");

//for Fan
//Adafruit_MQTT_Subscribe FanSlider = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/lights");
//Adafruit_MQTT_Publish FanDisplay = Adafruit_MQTT_Publish(&mqtt, MQTT_NAME "/f/Lts");

void setup1()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);


  //Connect to WiFi
  Serial.print("\n\nConnecting Wifi>");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  digitalWrite(LED_BUILTIN, LOW);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(">");
    delay(50);
  }

  Serial.println("OK!");

  //Subscribe to the LightSlider topic
  mqtt.subscribe(&LightSlider);

  pinMode(led, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(led, LOW);

}

void MQTT_connect()
{

  //  // Stop if already connected
  if (mqtt.connected() && mqtt.ping())
  {
    //    mqtt.disconnect();
    return;
  }

  int8_t ret;

  mqtt.disconnect();

  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) // connect will return 0 for connected
  {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0)
    {
      ESP.reset();
    }
  }
  Serial.println("MQTT Connected!");
}

void loop1()
{
  //Connect/Reconnect to MQTT
  MQTT_connect();

  //Read from our subscription queue until we run out, or
  //wait up to 5 seconds for subscription to update
  Adafruit_MQTT_Subscribe * subscription;
  while ((subscription = mqtt.readSubscription(5000)))
  {
    //If we're in here, a subscription updated...
    if (subscription == &LightSlider)
    {
      //Print the new value to the serial monitor
      Serial.print("LightSlider: ");
      Serial.println((char*) LightSlider.lastread);

      //If the new value is  "ON", turn the light on.
      //Otherwise, turn it off.
      if (!strcmp((char*) LightSlider.lastread, "ON"))
      {
        //active low logic
        digitalWrite(led, HIGH);
        LightDisplay.publish("ON");
      }
      else if (!strcmp((char*) LightSlider.lastread, "OFF"))
      {
        digitalWrite(led, LOW);
        LightDisplay.publish("OFF");

      }
      else
      {
        LightDisplay.publish("ERROR");
      }
    }
    else
    {
      //LightDisplay.publish("ERROR");
    }
  }
  //  if (!mqtt.ping())
  //  {
  //    mqtt.disconnect();
  //  }
}
