#include <iostream>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

void choose_number(uint16_t* tmp, int num);
void connect(uint16_t *bitmap, int hours, int minutes, int alarm, bool wifi);
void pris(uint16_t* tmp, uint16_t* num);
