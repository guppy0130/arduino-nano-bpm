#include <SPI.h>
#include <Wire.h>
#include "MAX30105.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h> // using the 128x32 display

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#include "heartRate.h"

MAX30105 particleSensor;

const byte RATE_SIZE = 4; // Increase this for more averaging. 4 is good.
const byte SAMP_SIZE = 127;
const byte HEIGHT = 8;
byte rates[SAMP_SIZE]; // Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; // Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;
long delta;

int maxx = -1;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  reset();
  display.println("Starting up...");
  display.display();
  delay(500);

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    display.println("No sensor. Please check wiring/power.");
    display.display();
    while (1);
  }

  reset();
  display.println("Put finger on sensor.");
  display.display();

  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x0A);
}

void loop() {
  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue)) {
    // We sensed a beat!
    long now = millis();
    delta = now - lastBeat;
    lastBeat = now;

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20 && delta < 1000) {
      int measure = (byte) beatsPerMinute;
      rates[rateSpot++] = measure;
      rateSpot %= SAMP_SIZE; // Cyclic buffer

      // Take average of last 4 readings
      beatAvg = 0;
      for (byte x = 0; x < RATE_SIZE ; x++) {
        beatAvg += rates[(rateSpot - x) % SAMP_SIZE];
      }
      beatAvg /= RATE_SIZE;

      // calc height
      if (measure > maxx) {
        maxx = measure;
      }
      int tmp = maxx / HEIGHT;

      reset();
      display.print("Delta: ");
      display.print(delta);
      display.println("ms");
      display.print("BPM: ");
      display.println(beatsPerMinute);
      display.print("Avg BPM: ");
      display.println(beatAvg);

      for (int i = 0; i < SAMP_SIZE; i++) {
        display.drawFastVLine(i, 31 - (rates[(i + rateSpot) % SAMP_SIZE] / tmp), 31, WHITE);
      }

      display.display();
    }
  }

  if (irValue < 50000) {
    reset();
    zero();
    display.print("No finger on sensor.");
    display.display();
  }
}

void reset() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
}

void zero() {
  for (int i = 0; i < SAMP_SIZE; i++) {
    rates[i] = 0;
  }
}
