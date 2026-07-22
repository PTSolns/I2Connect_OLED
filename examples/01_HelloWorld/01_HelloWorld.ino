// Example: Hello World for I2Connect: OLED
// Last Update: July 22, 2026
// Support: http://ptsolns.com/contact-us
//
// WHAT THIS EXAMPLE DOES
// Initializes the display, clears the internal memory buffer, and prints 
// basic text to the screen. 
//
// SKILLS LEARNED
// Initializing the OLED, setting the cursor position, printing strings, 
// and pushing the memory buffer to the physical display.
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
    while (!Serial) { }

    Wire.begin();
    
    // Initialize the display
    if (!display.begin(Wire, I2C_ADDRESS, I2C_CLOCK_HZ)) {
        Serial.println("OLED not found. Please check wiring.");
        while (true) { } // Halt execution
    }

    // Clear the memory buffer before drawing
    display.clearDisplay();

    // Set cursor to X=0, Y=0 (Top-left corner)
    display.setCursor(0, 0);
    display.println("Hello, World!");
    
    // Move cursor down two lines (each line is 8 pixels tall)
    display.setCursor(0, 16);
    display.println("https://ptsolns.com/products/i2connect-oled");

    // The screen will remain blank until the buffer is explicitly pushed
    display.display();
}

void loop() {
    // The screen retains its memory, so nothing is needed in the loop
}
