# ESP32 Temperature & Humidity Monitor — Web Flasher

This project is for:
- ESP32 DevKit V1 (ESP32-WROOM-32 / common 30- or 32-pin DevKit)
- DHT22 temperature/humidity sensor module
- Common 0.91-inch, 128×32 SSD1306 I²C OLED

## Wiring

### OLED (4 pins)
| OLED pin | ESP32 |
|---|---|
| GND | GND |
| VCC | 3V3 |
| SCL | GPIO 22 |
| SDA | GPIO 21 |

### DHT22 module (3 pins)
Connect by the **labels printed on your module**, not by guessing the left-to-right order:
| DHT22 module label | ESP32 |
|---|---|
| S / DATA / OUT | GPIO 4 |
| + / VCC | 3V3 |
| - / GND | GND |

The module shown in the project discussion appears to have an onboard resistor. For a typical 3-pin DHT22 breakout, an extra pull-up resistor is usually not needed.

## Publish your own browser flasher

The source code is not itself a flashable `.bin` file. The included GitHub Actions workflow builds the firmware and publishes a browser flasher page.

1. Sign in to GitHub and create a **new public repository**.
2. Upload the contents of this ZIP into the repository's root. Make sure `.github/workflows/build-and-publish.yml` is included.
3. Name the repository's default branch `main` (or change the workflow branch setting).
4. Open **Settings → Pages** and set the source to **GitHub Actions** if GitHub asks.
5. Open the **Actions** tab and wait for “Build and publish ESP32 web flasher” to finish successfully.
6. Open **Settings → Pages** to find the published page URL. Open it in Chrome or Edge on a computer.
7. Connect the ESP32 by USB, click **Install**, choose its serial port, and follow the prompts.

The first build may take a few minutes. The workflow rebuilds and republishes the flasher whenever you push changes to `main`.

## What the firmware does

- Reads DHT22 temperature in Celsius and relative humidity.
- Updates the OLED every 2 seconds.
- Shows a sensor error if the DHT22 reading fails.
- Prints readings and troubleshooting messages to the USB serial monitor at 115200 baud.

## Important compatibility notes

- This code assumes the OLED is **SSD1306, 128×32**, usually at I²C address `0x3C`. Some 0.91-inch displays use a different controller or address; if the screen stays blank, verify the display controller and address before changing wiring.
- Use USB power for initial testing.
- Do not connect a bare 3.7 V lithium cell directly to the ESP32 3V3 pin. A TP4056 charging board is a charger, not a 3.3 V regulator. Add a suitable regulated power circuit before using a battery.
- Disconnect power before changing wiring.
