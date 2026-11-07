#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// Wiring for ESP32 DevKit V1
static constexpr uint8_t DHT_PIN = 4;
static constexpr uint8_t DHT_TYPE = DHT22;
static constexpr int OLED_SDA = 21;
static constexpr int OLED_SCL = 22;

// Common 0.91-inch OLED: 128x32 pixels, I2C address 0x3C
static constexpr int SCREEN_WIDTH = 128;
static constexpr int SCREEN_HEIGHT = 32;
static constexpr uint8_t OLED_ADDRESS = 0x3C;

DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

unsigned long lastRead = 0;
const unsigned long READ_INTERVAL_MS = 2000;

void showMessage(const char *line1, const char *line2 = "") {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 4);
  display.println(line1);
  display.setCursor(0, 18);
  display.println(line2);
  display.display();
}

void readAndDisplaySensor() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // Celsius

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  if (isnan(humidity) || isnan(temperature)) {
    display.setTextSize(1);
    display.setCursor(0, 3);
    display.println("ROOM MONITOR");
    display.setCursor(0, 17);
    display.println("DHT22 read error");
    display.display();

    Serial.println("DHT22 read failed. Check VCC, GND, signal wire and sensor.");
    return;
  }

  // Top row: small heading
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("TEMPERATURE");

  // Main row: temperature in large text
  display.setTextSize(2);
  display.setCursor(0, 9);
  display.print(temperature, 1);
  display.print(" C");

  // Bottom row: humidity
  display.setTextSize(1);
  display.setCursor(0, 26);
  display.print("HUMIDITY ");
  display.print(humidity, 0);
  display.print("%");

  display.display();

  Serial.print("Temperature: ");
  Serial.print(temperature, 1);
  Serial.print(" C | Humidity: ");
  Serial.print(humidity, 1);
  Serial.println(" %");
}

void setup() {
  Serial.begin(115200);
  delay(200);

  Wire.begin(OLED_SDA, OLED_SCL);
  dht.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("OLED not found. Check wiring and I2C address (0x3C).");
    while (true) {
      delay(1000);
    }
  }

  display.clearDisplay();
  showMessage("ESP32 ROOM MONITOR", "Starting sensor...");
  Serial.println("ESP32 temperature and humidity monitor started.");
  delay(1500);

  readAndDisplaySensor();
  lastRead = millis();
}

void loop() {
  if (millis() - lastRead >= READ_INTERVAL_MS) {
    lastRead = millis();
    readAndDisplaySensor();
  }
}
