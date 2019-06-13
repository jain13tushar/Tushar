// Adafruit IO Temperature & Humidity Example
// Tutorial Link: https://learn.adafruit.com/adafruit-io-basics-temperature-and-humidity
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016-2017 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"
#include "GoogleAssistant.h"
/************************ Example Starts Here *******************************/
#include <Adafruit_Sensor.h>
#include "DHT.h"
#include "SPI.h"

// pin connected to DH22 data line
#define DHTPIN D1
#define DHTTYPE DHT11
// create DHT22 instance
DHT dht(DHTPIN, DHTTYPE);

const int ldrPin = A0;
int lr,in;
int r;

// set up the 'temperature' and 'humidity' feeds
AdafruitIO_Feed *temperature = io.feed("Temperature"); //Send Temperature
AdafruitIO_Feed *humidity = io.feed("Humidity"); //Send Humidity
AdafruitIO_Feed *fs = io.feed("Fs"); //for controlling the speed of the fan
AdafruitIO_Feed *lights = io.feed("lights"); //ON or OFF the light
AdafruitIO_Feed *li = io.feed("li"); //Send LUX
AdafruitIO_Feed *fns = io.feed("Fns"); //status of the fan
AdafruitIO_Feed *lts = io.feed("Lts"); //status of the light

void setup() {

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while (! Serial);

  // initialize dht22
  dht.begin();

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();
  fs->onMessage(handleMessage);
  lights->onMessage(lightscheck);
  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  pinMode(ldrPin, INPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
}


void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
  float c = dht.readTemperature();
  Serial.print("celsius: ");
  Serial.print(c);
  Serial.println("C");
  temperature->save(c);
  float h = dht.readHumidity();
  Serial.print("humidity: ");
  Serial.print(h);
  Serial.println("%");

  // save humidity to Adafruit IO
  humidity->save(h);
  Serial.print("Light Intensity:");
  in = analogRead(ldrPin);
  Serial.print(in);
  Serial.println(" Lux");
  li->save(in);
  fs->onMessage(handleMessage);
  lights->onMessage(lightscheck);
  fan();
  bulb();
  delay(10000);
}

void handleMessage( AdafruitIO_Data *data)
{
  r = data-> toInt();

}
void lightscheck( AdafruitIO_Data *data)
{ lr = data-> toInt();
}

void fan()
{
  if (r == 0)
  { digitalWrite(D2, HIGH);
    digitalWrite(D3, HIGH);
    digitalWrite(D4, HIGH);
    digitalWrite(D5, HIGH);
    Serial.println(r);
  }
  else if (r == 1)
  { digitalWrite(D2, LOW);
    digitalWrite(D3, HIGH);
    digitalWrite(D4, HIGH);
    digitalWrite(D5, HIGH);
    Serial.println(r);
  }
  else if (r == 2)
  { digitalWrite(D3, LOW);
    digitalWrite(D2, HIGH);
    digitalWrite(D4, HIGH);
    digitalWrite(D5, HIGH);
    Serial.println(r);
  }
  else if (r == 3)
  { digitalWrite(D4, LOW);
    digitalWrite(D2, HIGH);
    digitalWrite(D3, HIGH);
    digitalWrite(D5, HIGH);
    Serial.println(r);
  }
  else if (r == 4)
  { digitalWrite(D5, LOW);
    digitalWrite(D2, HIGH);
    digitalWrite(D4, HIGH);
    digitalWrite(D3, HIGH);
    Serial.println(r);
  }
  fns->save(r);
}

void bulb()
{ if(lr==1)
  {
    digitalWrite(D6, LOW);
    Serial.println("ON");
    lts->save(lr);
  }
  else
  {
  if (lr == 1)
  { digitalWrite(D6, LOW);
    Serial.println("ON");
  }
  else if (lr == 0)
  { digitalWrite(D6, HIGH);
    Serial.println("OFF");
  }
  lts->save(lr);
  }
   
}
