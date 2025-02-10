**Bitbang mit dem LGT8FX und FastIO für schnelle GPIO-Steuerung**

In diesem Projekt steuern wir WS2812B RGB-LEDs mithilfe von **Bitbanging** und einem LGT8FX-Mikrocontroller, der mit **FastIO**-Funktionen optimiert wird, um eine schnelle und effiziente Steuerung der LEDs zu ermöglichen. 

### Was ist Bitbanging?

Bitbanging ist eine Technik, bei der Bits manuell über GPIO-Pins gesendet werden, anstatt SPI, UART oder I2C zurückzugreifen. Diese Methode erfordert eine präzise Zeitsteuerung (Nanosekunden), um Bits korrekt zu übertragen.

### Warum der LGT8FX mit FastIO perfekt ist

Der LGT8FX-Mikrocontroller bietet eine besonders schnelle GPIO-Steuerung, die perfekt für Bitbanging-Anwendungen geeignet ist. Mit der Verwendung von **FastIO**-Funktionen erreichen wir sehr schnelle Impulsfolgen, die für die Steuerung der WS2812B LEDs notwendig sind. Diese Funktionen ermöglichen es uns, die Pins direkt zu steuern und die Reaktionszeit zu minimieren.

### Warum benötigen wir Nanosekunden für Bitbanging?

Das WS2812B-Protokoll verwendet eine serielle Datenübertragung, bei der jedes Bit als eine Änderung des Signalpegels interpretiert wird. Ein einzelnes Pixel (LED) wird durch 24 Bits definiert, wobei 8 Bits für jede der Farben Rot, Grün und Blau (RGB) verwendet werden. 

Das Timing für die Bitübertragung ist entscheidend:  
- Ein **High-Puls** von ca. 0,8 µs und ein **Low-Puls** von 0,45 µs repräsentieren ein logisches "1"-Bit.
- Ein **High-Puls** von 0,4 µs und ein **Low-Puls** von 0,85 µs repräsentieren ein logisches "0"-Bit.

Um diese präzise Taktung zu erreichen, sind **Nanosekunden** notwendig, da die Steuerung der LEDs auf sehr kurze Zeiträume angewiesen ist. In der Praxis wird die Funktion **`delayMicroseconds`** verwendet, um diese Millisekunden- und Mikrosekundenabstände zu erzeugen, was eine korrekte Übertragung und die Interpretation der Bitfolgen durch den WS2812B ermöglicht.

### Kurze Codebeschreibung

Im folgenden Code definieren wir die **`sendBit`** Funktion, um ein einzelnes Bit zu senden. Je nach Zustand des Bits (`HIGH` oder `LOW`) setzen wir den Pin auf den entsprechenden Wert und erzeugen mit der **`delayMicroseconds`** Funktion die richtigen Zeitabstände.

```cpp
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
```

Die **`sendRGB`** Funktion sendet die RGB-Werte für die WS2812B LEDs, wobei die Reihenfolge der Farben von **GRB** erwartet wird. Der Code geht bitweise durch die Farbinformationen und überträgt sie mit der **`sendBit`** Funktion.

```cpp
void sendRGB(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 7; i >= 0; i--) {
    sendBit((g >> i) & 1); // G-Wert senden
  }
  for (int i = 7; i >= 0; i--) {
    sendBit((r >> i) & 1); // R-Wert senden
  }
  for (int i = 7; i >= 0; i--) {
    sendBit((b >> i) & 1); // B-Wert senden
  }
}
```

In der **`rainbow`** Funktion wird ein Regenbogenfarbverlauf berechnet, der für jede LED individuell angepasst wird.

```cpp
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
```

Dieser Code demonstriert, wie mit Bitbanging und FastIO die WS2812B LEDs ohne Bibliotheken effizient gesteuert werden können, während eine hohe Präzision und Geschwindigkeit gewährleistet bleibt.

## Links

- WS2812b Datasheet: <https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf>
- fastio LGT8FX: <https://github.com/dbuezas/lgt8fx/blob/master/lgt8f/libraries/lgt328p/examples/lgt8fx8p_fastio/lgt8fx8p_fastio.ino>
