#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

const int SAMP_SIZE = 127;
const int HEIGHT = 8;
int rates[SAMP_SIZE];

int circBuff = 0;
int maxx = 0;

void setup() {
  // put your setup code here, to run once:
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  reset();

  for (int i = 0; i < SAMP_SIZE; i++) {
    int x = random(20, 255);
    rates[i] = x;
    if (x > maxx) {
      maxx = x;
    }
  }

  int tmp = maxx / HEIGHT;

  for (int i = 0; i < SAMP_SIZE; i++) {
    display.drawFastVLine(i, 31 - rates[i] / tmp, 31, WHITE);
    display.display();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  reset();

  int addition = random(20, 255);
  rates[circBuff++] = addition;
  circBuff %= SAMP_SIZE;
  if (addition > maxx) {
    maxx = addition;
  }

  int tmp = maxx / HEIGHT;

  for (int i = 0; i < SAMP_SIZE; i++) {
    display.drawFastVLine(i, 31 - (rates[(i + circBuff) % SAMP_SIZE] / tmp), 31, WHITE);
  }
  display.display();
}

void reset() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
}
