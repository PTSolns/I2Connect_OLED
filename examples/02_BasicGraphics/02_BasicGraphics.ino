// Example: Basic Graphics for I2Connect: OLED
// Last Update: July 22, 2026
// Support: http://ptsolns.com/contact-us
//
// WHAT THIS EXAMPLE DOES
// Demonstrates how to draw outlines and filled geometric shapes using 
// the module's native, lightweight graphics engine.
//
// SKILLS LEARNED
// Utilizing mathematical primitives to draw lines, rectangles, rounded 
// rectangles, and circles without external library dependencies.
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

// User Settings
const uint32_t SERIAL_BAUD  = 115200; // Serial baud setting
const uint32_t I2C_CLOCK_HZ = 400000; // I2C clock speed. 400kHz is recommended for fast screen updates.
const uint8_t  I2C_ADDRESS  = 0x3C;   // Fixed address for the OLED module

PTSolns_I2Connect_OLED display;

void setup() {
    Serial.begin(SERIAL_BAUD);
    Wire.begin();
    
    if (!display.begin(Wire, I2C_ADDRESS, I2C_CLOCK_HZ)) {
        Serial.println("OLED not found. Please check wiring.");
        while (true) { }
    }
}

void loop() {
    display.clearDisplay();

    // 1. Draw a Line (Diagonal across the top)
    display.drawLine(0, 0, 127, 16);

    // 2. Draw a Rectangle Outline (X, Y, Width, Height)
    display.drawRect(0, 24, 40, 20);

    // 3. Draw a Filled Rounded Rectangle (X, Y, Width, Height, Corner Radius)
    display.fillRoundRect(48, 24, 40, 20, 4);

    // 4. Draw a Filled Circle (Center X, Center Y, Radius)
    display.fillCircle(110, 34, 10);

    display.display();
    delay(2000);
    
    display.clearDisplay();
    display.setCursor(20, 28);
    display.print("Test Complete");
    display.display();
    delay(2000);
}
