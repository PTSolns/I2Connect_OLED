// Example: Sensor Dashboard for I2Connect: OLED
// Last Update: July 22, 2026
// Support: http://ptsolns.com/contact-us
//
// WHAT THIS EXAMPLE DOES
// Pairs the OLED display with the PTSolns I2Connect: AHT20 module 
// to create a live, auto-updating temperature and humidity dashboard.
//
// SKILLS LEARNED
// Integrating multiple I2Connect modules on the same I2C bus and 
// formatting live sensor data cleanly onto a screen.
//
// DESCRIPTION
// The I2Connect: OLED is a compact, high-contrast 0.96-inch display module.
// It provides a simple and reliable way to add a rich visual user interface 
// and data readouts to your embedded systems.
// The module communicates over the standard I2C interface with a fixed address 
// of 0x3C. If multiple OLED modules are required in the same system, an I2C 
// multiplexer must be used.
// Like all I2Connect modules, it features dual Qwiic compatible connectors for 
// easy daisy-chaining and an angled male header for breadboard use. It is 
// fully compatible with both 3.3V and 5V microcontroller systems.

#include <Wire.h>
#include <PTSolns_I2Connect_OLED.h>
#include <PTSOLNS_I2CONNECT_AHT20.h>

// User Settings
const uint32_t SERIAL_BAUD    = 115200; // Serial baud setting
const uint32_t I2C_CLOCK_HZ   = 100000; // Default to 100kHz when using mixed modules
const uint8_t  OLED_ADDRESS   = 0x3C;   // Fixed address for the OLED
const uint8_t  AHT20_ADDRESS  = 0x38;   // Fixed address for the AHT20

PTSolns_I2Connect_OLED display;
PTSOLNS_I2CONNECT_AHT20 aht20;

void setup() {
    Serial.begin(SERIAL_BAUD);
    Wire.begin();

    if (!display.begin(Wire, OLED_ADDRESS, I2C_CLOCK_HZ)) {
        Serial.println("OLED missing!");
        while(true) { }
    }
    
    if (!aht20.begin(Wire, AHT20_ADDRESS, I2C_CLOCK_HZ)) {
        Serial.println("AHT20 missing!");
        display.clearDisplay();
        display.setCursor(0,0);
        display.print("AHT20 Sensor Error");
        display.display();
        while(true) { }
    }
}

void loop() {
    // Read new data from the AHT20 sensor
    if (aht20.readData()) {
        float tempC = aht20.getTemperature();
        float humidity = aht20.getHumidity();

        display.clearDisplay();

        // UI Header Outline
        display.drawRect(0, 0, 128, 14);
        display.setCursor(14, 3);
        display.print("Enviro Dashboard");

        // Temperature Output
        display.setCursor(0, 24);
        display.print("Temp: ");
        display.print(tempC);
        display.print(" C");

        // Humidity Output
        display.setCursor(0, 40);
        display.print("Hum:  ");
        display.print(humidity);
        display.print(" %");

        display.display();
    }

    // Refresh the screen every 2 seconds
    delay(2000);
}
