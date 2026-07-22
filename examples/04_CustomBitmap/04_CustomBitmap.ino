// Example: Custom Bitmap for I2Connect: OLED
// Last Update: July 22, 2026
// Support: http://ptsolns.com/contact-us
//
// WHAT THIS EXAMPLE DOES
// Demonstrates how to load a custom 1-bit array from PROGMEM and 
// render it as an icon or logo directly on the display.
//
// SKILLS LEARNED
// Storing arrays in flash memory (PROGMEM) to save SRAM and utilizing 
// the drawBitmap function to render XBM-style graphical formats.
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

// A 32x32 pixel gear icon stored in flash memory to preserve SRAM
static const uint8_t PROGMEM gearIcon32x32[] = {
  0x00, 0x07, 0xe0, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x0f, 0xf0, 0x00, 
  0x00, 0x3f, 0xfc, 0x00, 0x03, 0xff, 0xff, 0xc0, 0x07, 0xff, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xf8, 
  0x1f, 0xff, 0xff, 0xf8, 0x3f, 0xc0, 0x03, 0xfc, 0x3f, 0x80, 0x01, 0xfc, 0x7f, 0x00, 0x00, 0xfe, 
  0x7e, 0x00, 0x00, 0x7e, 0x7e, 0x00, 0x00, 0x7e, 0xfc, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x3f, 
  0xfc, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x3f, 0x7e, 0x00, 0x00, 0x7e, 0x7e, 0x00, 0x00, 0x7e, 
  0x7f, 0x00, 0x00, 0xfe, 0x3f, 0x80, 0x01, 0xfc, 0x3f, 0xc0, 0x03, 0xfc, 0x1f, 0xff, 0xff, 0xf8, 
  0x1f, 0xff, 0xff, 0xf8, 0x07, 0xff, 0xff, 0xe0, 0x03, 0xff, 0xff, 0xc0, 0x00, 0x3f, 0xfc, 0x00, 
  0x00, 0x0f, 0xf0, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x07, 0xe0, 0x00
};

void setup() {
    Wire.begin();
    display.begin(Wire, I2C_ADDRESS, I2C_CLOCK_HZ);

    display.clearDisplay();

    // Syntax: drawBitmap(X, Y, Array_Name, Width, Height, Color)
    display.drawBitmap(48, 16, gearIcon32x32, 32, 32, true);

    display.setCursor(30, 52);
    display.print("System Boot");

    display.display();
}

void loop() {
    // Static screen
}
