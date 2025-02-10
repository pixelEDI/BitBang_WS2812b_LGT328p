/*
        _          _          _ _
  _ __ (_)_  _____| | ___  __| (_)
 | '_ \| \ \/ / _ \ |/ _ \/ _` | |
 | |_) | |>  <  __/ |  __/ (_| | |
 | .__/|_/_/\_\___|_|\___|\__,_|_|
 |_|

https://links.pixeledi.eu
LGT8F328P | 01.2025

*/

#include <Arduino.h>
#define NUM_LEDS 12

void sendBit(bool bit) {
  if (bit) {
    fastioWrite(12, HIGH);
    delayMicroseconds(0.8); // T1H
    fastioWrite(12, LOW);
    delayMicroseconds(0.45); // T1L
  } else {
    fastioWrite(12, HIGH);
    delayMicroseconds(0.4); // T0H
    fastioWrite(12, LOW);
    delayMicroseconds(0.85); // T0L
  }
}

void sendRGB(uint8_t r, uint8_t g, uint8_t b) {
  // WS2812B erwartet die Reihenfolge: GRB
  for (int i = 7; i >= 0; i--) {
    sendBit((g >> i) & 1);
  }
  for (int i = 7; i >= 0; i--) {
    sendBit((r >> i) & 1);
  }
  for (int i = 7; i >= 0; i--) {
    sendBit((b >> i) & 1);
  }
}

// Funktion zum Berechnen eines Regenbogen-Farbwertes
void rainbow(uint8_t position, uint8_t &r, uint8_t &g, uint8_t &b) {
  position = 255 - position;
  if (position < 85) {
    r = 255 - position * 3;
    g = 0;
    b = position * 3;
  } else if (position < 170) {
    position -= 85;
    r = 0;
    g = position * 3;
    b = 255 - position * 3;
  } else {
    position -= 170;
    r = position * 3;
    g = 255 - position * 3;
    b = 0;
  }
}

void setup() { fastioMode(12, OUTPUT); }

void loop() {
  // 1 Szenario
  // sendRGB(255, 0, 0);
  // sendRGB(0, 255, 0);
  // sendRGB(0, 0, 255);
  // delay(1000);
  // 2. Szenario rainbow

  static uint8_t pos = 0;
  uint8_t r, g, b;

  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t ledPos = pos + (i * 256 / NUM_LEDS);
    rainbow(ledPos, r, g, b);
    sendRGB(r, g, b);
  }

  pos++;
  delay(50);
}
