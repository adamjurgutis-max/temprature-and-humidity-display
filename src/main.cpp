

#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <DHT.h>

// OLED: First test with SSD1306 128x64
U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(
  U8G2_R0,
  U8X8_PIN_NONE
);

// DHT11
#define DHT_PIN 4
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(115200);

  Wire.begin(21, 22);
  dht.begin();

  display.begin();
  display.clearBuffer();

  display.setFont(u8g2_font_ncenB14_tr);
  display.drawStr(10, 30, "OLED TEST");

  display.sendBuffer();

  Serial.println("U8g2 OLED test started");
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (!isnan(humidity) && !isnan(temperature)) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" C | Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    display.clearBuffer();

    display.setFont(u8g2_font_6x12_tr);
    display.drawStr(0, 12, "TEMPERATURE");

    char tempText[20];
    snprintf(tempText, sizeof(tempText), "%.1f C", temperature);

    display.setFont(u8g2_font_ncenB14_tr);
    display.drawStr(0, 34, tempText);

    display.setFont(u8g2_font_6x12_tr);
    char humidityText[20];
    snprintf(humidityText, sizeof(humidityText), "HUMIDITY: %.0f%%", humidity);
    display.drawStr(0, 55, humidityText);

    display.sendBuffer();
  } else {
    Serial.println("DHT11 reading failed!");
  }

  delay(2000);
}

