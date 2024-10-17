#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeMonoOblique9pt7b.h>
#include <DHT.h>
#include "MQ135.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define MQ135PIN A0
#define DHTPIN 2          // Digital pin 2
#define DHTTYPE DHT11     // DHT 11

DHT dht(DHTPIN, DHTTYPE);
MQ135 mq135_sensor(MQ135);
String quality = "";

void sendSensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setFont();
  display.setCursor(0, 43);
  display.println("Temp  :");
  display.setCursor(80, 43);
  display.println(t);
  display.setCursor(114, 43);
  display.println("C");
  display.setCursor(0, 56);
  display.println("RH    :");
  display.setCursor(80, 56);
  display.println(h);
  display.setCursor(114, 56);
  display.println("%");
}

void airSensor() {
  float ppm = mq135_sensor.getPPM();
  if (ppm <= 50.0) {
    quality = "Good!";
  } else if (ppm > 50.0 && ppm <= 100.0) {
    quality = "Moderate!";
  } else if (ppm > 100.0 && ppm <= 150.0) {
    quality = "Sensitive!";
  } else if (ppm > 150.0 && ppm <= 200.0) {
    quality = "Unhealthy!";
  } else if (ppm > 200.0 && ppm <= 250.0) {
    quality = "Very Unhealthy!";
  } else {
    quality = "Hazardous!";
  }

  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(1, 5);
  display.setFont();
  display.println("Air Quality:");
  display.setTextSize(1);
  display.setCursor(20, 23);
  display.setFont(&FreeMonoOblique9pt7b);
  display.println(quality);
}

void setup() {
  Serial.begin(9600);
  pinMode(SENSOR_PIN, INPUT);
  dht.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
  }

  display.clearDisplay();
  display.setTextColor(WHITE);

  display.setTextSize(2);
  display.setCursor(50, 0);
  display.println("Air");
  display.setTextSize(1);
  display.setCursor(23, 20);
  display.println("Quality monitor");
  display.display();
  delay(1200);
  display.clearDisplay();
}

void loop() {
  display.clearDisplay();
  airSensor();
  sendSensor();
  display.display();
  delay(2000); 
}