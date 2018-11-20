# arduino-nano-bpm

## Arduino Nano x MAX30102 x SSD1306

### Hardware

These are Amazon affiliate links; a small part of your purchase goes to support me without any extra cost to you.

* [Arduino Nano](https://amzn.to/2FxQPF4)
* [SSD1306 OLED Display](https://amzn.to/2S3pb42)
* [MAX30102 Heart Rate/SpO2 Sensor](https://amzn.to/2KjaOGf)

### Software

* CH340 driver (may be handled automatically for you, otherwise check documentation)
* [Sparkfun MAX3010x Sensor Library](https://github.com/sparkfun/SparkFun_MAX3010x_Sensor_Library)

### Setup

1. Connect the OLED display and heart rate sensor to the Nano's A4, A5, 5V, and GND pins.
2. Connect Nano to host, install drivers if necessary (Win10 will say USB-SERIAL CH340 instead of Arduino Nano, but it works all the same), install libraries, and upload code
3. ???
4. Profit

### Bugs/Features

* Incoming information is a little noisy, but it's being managed by the `delta < 1000` check. If `delta > 1000`, the information associated with it doesn't look great, so it's thrown out (usually, this is a <60bpm).
    * Time to register finger touch slowed as a result
    * Can appear as if not registering anything - just be patient.

### Displayed Information

* Delta (time between heartbeats)
* BPM: current BPM through some math (taken from Sparkfun's examples)
* Average BPM: average BPM across 4 latest measurements (taken/modified from examples)
* Cute little chart showing BPM on Y axis and time on X. No labels on axes though, feel free to submit a pull request if you'd like :)

### How it Works

The sensor measures some quantities about the finger interacting with it through IR and a couple LEDs. It gives this information to the Nano, which measures time between certain quantities returned by the sensor to calculate heartbeat. This information is then formatted, added to the queue, and displayed on screen.
