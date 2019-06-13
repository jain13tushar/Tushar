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

// pin connected to DH22 data line
#define DHTPIN D1
#define DHTTYPE DHT11
// create DHT11 instance
DHT dht(DHTPIN, DHTTYPE);

const int ldrPin = A0;
int lr;
int in, r, t = 0, a, l = 0, d = 0, e = 0;
int Sd[5] = {0, 1, 2, 3, 4};
float Td[5][2] = {{ -20, 17}, {18, 24}, {25, 30}, {31, 35}, {36, 60}};
float Tn[10][2], Hn[10][2], Tml[100][2], Hml[100][2];
int Sn[10], Sml[100], respd[2] = {0, 1}, i = 0, j = 0, y =0;
int Ld[2][2] = {{200, 1000}, {0, 199}};
float c, h;

// set up the 'temperature' and 'humidity' feeds
AdafruitIO_Feed *temperature = io.feed("Temperature");
AdafruitIO_Feed *humidity = io.feed("Humidity");
AdafruitIO_Feed *fs = io.feed("Fs");
AdafruitIO_Feed *lights = io.feed("lights");
AdafruitIO_Feed *li = io.feed("li");
AdafruitIO_Feed *fns = io.feed("Fns");
AdafruitIO_Feed *lts = io.feed("Lts");

void setup() {

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  //while (! Serial);

  // initialize dht11
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
  c = dht.readTemperature();
  Serial.print("celsius: ");
  Serial.print(c);
  Serial.println("C");
  temperature->save(c);
  h = dht.readHumidity();
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
   deffan();
  deflight();
  fs->onMessage(handleMessage);
  lights->onMessage(lightscheck);
  FanCheck();
  fan();
  bulb();
  // wait 5 seconds (5000 milliseconds == 5 seconds)
  delay(10000);
y=0;
e=0;
}

void handleMessage( AdafruitIO_Data *data)
{
  r = data-> toInt();
  
}

void lightscheck( AdafruitIO_Data *data)
{
    lr = data-> toInt();
  
}

void fan()
{
  if (t == 0)
  { digitalWrite(D2, HIGH);
    digitalWrite(D3, HIGH);
    digitalWrite(D4, HIGH);
    digitalWrite(D5, HIGH);
    Serial.println(t);
  }
  else if (t == 1)
  { digitalWrite(D2, LOW);
    digitalWrite(D3, HIGH);
    digitalWrite(D4, HIGH);
    digitalWrite(D5, HIGH);
    Serial.println(r);
  }
  else if (t == 2)
  { digitalWrite(D3, LOW);
    digitalWrite(D2, HIGH);
    digitalWrite(D4, HIGH);
    digitalWrite(D5, HIGH);
    Serial.println(t);
  }
  else if (t == 3)
  { digitalWrite(D4, LOW);
    digitalWrite(D2, HIGH);
    digitalWrite(D3, HIGH);
    digitalWrite(D5, HIGH);
    Serial.println(t);
  }
  else if (t == 4)
  { digitalWrite(D5, LOW);
    digitalWrite(D2, HIGH);
    digitalWrite(D4, HIGH);
    digitalWrite(D3, HIGH);
    Serial.println(t);
  }
  fns->save(t);
}

void bulb()
{ if(d==1)
  {
    digitalWrite(D6, LOW);
    Serial.println("ON");
    lts->save(d);
  }
  else
  {
  if (a == 1)
  { digitalWrite(D6, LOW);
    Serial.println("ON");
  }
  else if (a == 0)
  { digitalWrite(D6, HIGH);
    Serial.println("OFF");
  }
  lts->save(a);
  }
   
}
void macheinlearnFan()
{
  int k, b, ti, tf, hi, hf, ri;
  for (k = 0; k < i; k++)
  { ti = Tn[k][0];
    tf = Tn[k][1];
    hi = Hn[k][0];
    hf = Hn[k][1];
    ri = Sn[k];
    for (b = 0;b<l; b++)
    {
      if ((ti == (Tn[b][0])) && (tf == (Tn[b][1])) && (hi == (Hn[b][0])) && (hf == (Hn[b][1])) && (ri == Sn[b]))
      { Tml[l][0] = Tn[k][0];
        Tml[l][1] = Tn[k][1];
        Hml[l][0] = Hn[k][0];
        Hml[l][1] = Hn[k][1];
        Sml[l] = Sn[k];
        l++;
      }
    }
  }
}
void FanCheck()
{ int o;
    for (o = 0; o < l; o++)
    {
      if ((c >= (Tml[o][0])) && (c <= (Tml[o][1])) && (h >= (Hml[o][0])) && (h <= (Hml[o][1])))
      { t == Sml[o];
      }
    }
    fns->save(t);
     lts->save(a);    
}
void deflight()
{
  int u;
  lights->onMessage(lightscheck);
  if(e==0){
  for (u = 0; u < 2; u++)
  { if ((in >= (Ld[u][0])) && (in <= (Ld[u][1])))
    { a = respd[u];
    }
  }
    
}
  lts->save(a);
}
void deffan()
{ int u;
fs->onMessage(handleMessage);
if(y==0){
  for (u = 0; u < 5; u++)
  { if ((c >= (Td[u][0])) && (c <= (Td[u][1])))
    {
      t = Sd[u];
    }
  }
}
  fns->save(t);
}
