// Example: Full Feature Demo for I2Connect: OLED
// Last Update: July 22, 2026
// Support: http://ptsolns.com/contact-us
//
// WHAT THIS EXAMPLE DOES
// Cycles through animated scenes to exercise every function in the library, 
// including text wrapping, shape rendering, and live math plotting.
//
// SKILLS LEARNED
// Advanced graphics utilization, building animation loops, clearing and 
// redrawing rapidly, and creating dynamic visual elements.
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
#include <math.h>
#include "PTSolns_I2Connect_OLED.h"

// User Settings
const uint32_t SERIAL_BAUD  = 115200; 
const uint32_t I2C_CLOCK_HZ = 400000; 
const uint8_t  I2C_ADDRESS  = 0x3C;   

PTSolns_I2Connect_OLED display;

// A tiny 16x16 "signal bars" icon
static const uint8_t iconSignal16x16[] PROGMEM = {
    0b00000000, 0b00000000,
    0b00000000, 0b00000000,
    0b00000001, 0b10000000,
    0b00000001, 0b10000000,
    0b00000011, 0b11000000,
    0b00000011, 0b11000000,
    0b00000111, 0b11100000,
    0b00000111, 0b11100000,
    0b00001111, 0b11110000,
    0b00001111, 0b11110000,
    0b00011111, 0b11111000,
    0b00011111, 0b11111000,
    0b00111111, 0b11111100,
    0b00111111, 0b11111100,
    0b00000000, 0b00000000,
    0b00000000, 0b00000000
};

const unsigned long SCENE_DURATION_MS = 3500;

void setup() {
    Serial.begin(SERIAL_BAUD);
    Wire.begin();

    if (!display.begin(Wire, I2C_ADDRESS, I2C_CLOCK_HZ)) {
        Serial.println("OLED not found. Check wiring / I2C address.");
        while (true) { }
    }
}

void loop() {
    sceneTextDemo();
    sceneShapesDemo();
    sceneBitmapDemo();
    sceneBouncingBallAnimation();
    sceneBarGraphAnimation();
    sceneSineWavePlotter();
}

void sceneTextDemo() {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("PTSolns OLED");
    display.setCursor(0, 8);
    display.println("Text Demo");
    display.setCursor(0, 24);
    display.println("Line wrap test:");
    display.setCursor(0, 32);
    display.println("abcdefghijklmnopqrstuvwxyz0123456789");
    display.setCursor(0, 56);
    display.println("Symbols: !@#$%^&*()");
    display.display();
    delay(SCENE_DURATION_MS);
}

void sceneShapesDemo() {
    display.clearDisplay();

    // Rounded rect outline + filled rounded rect
    display.drawRoundRect(0, 0, 44, 24, 6, true);
    display.fillRoundRect(48, 0, 44, 24, 6, true);

    // Outline circle + filled circle
    display.drawCircle(14, 44, 10, true);
    display.fillCircle(40, 44, 10, true);

    // Triangle outline + filled triangle
    display.drawTriangle(70, 36, 60, 54, 82, 54, true);
    display.fillTriangle(96, 36, 88, 54, 108, 54, true);

    // A rect + diagonal line, tucked in the remaining corner
    display.drawRect(100, 2, 24, 20, true);
    display.drawLine(100, 2, 124, 22, true);

    display.display();
    delay(SCENE_DURATION_MS);
}

void sceneBitmapDemo() {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Bitmap Blit:");

    // Draw the icon a few times at different positions across the row
    display.drawBitmap(10, 20, iconSignal16x16, 16, 16, true);
    display.drawBitmap(40, 20, iconSignal16x16, 16, 16, true);
    display.drawBitmap(70, 20, iconSignal16x16, 16, 16, true);
    display.drawBitmap(100, 20, iconSignal16x16, 16, 16, true);

    display.setCursor(0, 44);
    display.println("(PROGMEM icon x4)");

    display.display();
    delay(SCENE_DURATION_MS);
}

void sceneBouncingBallAnimation() {
    const int16_t radius = 8;
    int16_t x = radius + 2;
    int16_t y = 32;
    int8_t vx = 3;
    int8_t vy = 2;

    unsigned long start = millis();
    while (millis() - start < SCENE_DURATION_MS) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print("Bounce demo");
        display.drawCircle(x, y, radius, true);
        display.display();

        x += vx;
        y += vy;

        if (x - radius <= 0 || x + radius >= 128) vx = -vx;
        if (y - radius <= 8 || y + radius >= 64) vy = -vy;

        delay(25);
    }
}

void sceneBarGraphAnimation() {
    const uint8_t numBars = 8;
    const uint8_t barWidth = 12;
    const uint8_t barGap = 3;
    const uint8_t baseY = 63;
    uint8_t barHeights[numBars];

    for (uint8_t i = 0; i < numBars; i++) {
        barHeights[i] = random(6, 50);
    }

    unsigned long start = millis();
    while (millis() - start < SCENE_DURATION_MS) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print("Bar graph");

        for (uint8_t i = 0; i < numBars; i++) {
            // Random-walk each bar's height for a "live meter" feel
            int8_t delta = random(-6, 7);
            int16_t newHeight = (int16_t)barHeights[i] + delta;
            if (newHeight < 4) newHeight = 4;
            if (newHeight > 52) newHeight = 52;
            barHeights[i] = (uint8_t)newHeight;

            int16_t x = i * (barWidth + barGap);
            display.fillRect(x, baseY - barHeights[i], barWidth, barHeights[i], true);
        }

        display.display();
        delay(80);
    }
}

void sceneSineWavePlotter() {
    const uint8_t midY = 40;
    const uint8_t amplitude = 16;
    float phase = 0.0;

    unsigned long start = millis();
    while (millis() - start < SCENE_DURATION_MS) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print("Sine plot");

        for (int16_t x = 0; x < 128; x++) {
            float angle = (x * 0.1f) + phase;
            int16_t y = midY + (int16_t)(amplitude * sin(angle));
            display.drawPixel(x, y, true);
        }

        display.display();
        phase += 0.3f;
        delay(30);
    }
}
