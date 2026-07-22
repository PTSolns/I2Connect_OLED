# PTSolns *I2Connect: OLED* Library

![Arduino Library](https://img.shields.io/badge/Arduino-Library-blue)
![License](https://img.shields.io/badge/License-MIT-green)
![Display](https://img.shields.io/badge/Display-OLED_128x64-orange)

Library for the PTSolns *I2Connect: OLED* module. Available through both the Arduino Library Manager and the PTSolns IDE Library Manager, this library provides a powerful but easy-to-use graphics engine to draw text, shapes, and bitmaps while automatically managing I2C communication and screen initialization.

---

## Overview

The *I2Connect: OLED* is a compact, high-contrast 0.96-inch display module. Designed as part of the I2Connect Series, the module provides a simple and reliable way to add a rich visual user interface and data readouts to your embedded systems. 

The display communicates over the industry-standard I2C interface and features a 128x64 pixel resolution. The self-illuminating pixels require no backlight, resulting in deep blacks and crisp graphics. The module is fully compatible with both 3.3V and 5V systems out of the box, making it suitable for use with a wide range of popular microcontroller platforms without the need for external level shifters. 

Like all I2Connect modules, it connects using dual Qwiic® compatible connectors to easily integrate into existing I2C systems without soldering, while the preinstalled right-angle header provides direct access to the I2C bus for breadboard-based prototyping.

---

## *I2Connect* Series

The PTSolns *I2Connect* Series is a family of plug and play expansion modules designed to simplify the integration of sensors, displays, user interfaces, and other peripherals into embedded systems. Built around the industry standard I2C communication protocol and featuring convenient Qwiic® compatible connectors (4 pin, 1 mm JST SH), *I2Connect* modules can be quickly connected without soldering or complex wiring.

Each module is fully assembled and includes a preinstalled male header that is compatible with standard solderless breadboards, enabling immediate use and rapid prototyping. Designed to work seamlessly with a wide range of microcontroller platforms, I2Connect modules combine compact hardware, comprehensive documentation, and easy to use software libraries. The _I2Connect_ Series enables makers, students, educators, and professionals to rapidly prototype, evaluate, and develop reliable electronic systems.

The following modules are part of the *I2Connect* Series:

* [**AHT20**](https://ptsolns.com/products/i2connect-aht20) — Temperature & Humidity
* [**VEML7700**](https://ptsolns.com/products/i2connect-veml7700) — Light & Lux
* [**EEPROM**](https://ptsolns.com/products/i2connect-eeprom) — 2 Mbit Memory
* **Buttons** — User Input via 4×4 Matrix of Buttons
* **Digit Display** — Dual 7 Segment Display
* **ICP-20100** — Pressure & Temperature
* **KXTJ3-1057** — 3 Axis Accelerometer
* **MUX** — 4 Channel I2C Multiplexer
* **OLED** — 0.96" 128×64 OLED Display
* **I2C Doctor** — I2C Bus Recovery and Diagnostics
* **...and more!**

---

## Key Features

* High-contrast 0.96-inch monochrome display.
* Crisp 128x64 pixel resolution.
* Fixed I2C address of 0x3C.
* 3.3V and 5V logic and power compatibility.
* Advanced graphics engine supporting text scaling, geometric shapes, and custom bitmaps.
* Dual Qwiic® compatible 4-pin JST SH connectors for daisy-chaining.
* Breadboard-friendly 4-pin 2.54mm angled male header.
* Integrated power LED with a physical disconnect pad on the rear.

---

## I2Connect Ecosystem

The *I2Connect: OLED* is part of the PTSolns I2Connect ecosystem.

All *I2Connect* modules utilize a standardized 4 pin JST SH 1.0 mm connector carrying:

| Pin | Function |
| --- | -------- |
| 1   | GND      |
| 2   | VCC      |
| 3   | SDA      |
| 4   | SCL      |

This connector follows the widely adopted Qwiic® standard, allowing modules and compatible boards to be connected without soldering.

For maximum compatibility, the module also includes a preinstalled angled male header, allowing use with virtually any 3.3 V or 5 V microcontroller platform even when a Qwiic compatible connector is unavailable.

---

## OLED Specifications

| Parameter               | Value                          |
| ----------------------- | ------------------------------ |
| Display Size            | 0.96"                          |
| Resolution              | 128 x 64 Pixels                |
| Interface               | I2C                            |
| I2C Address             | 0x3C (Fixed)                   |
| Operating Voltage       | 3.3 V / 5 V Rated              |
| Communication Speed     | Standard & Fast (100 / 400 kHz)|
| Dimensions              | 33.2 mm x 33.2 mm              |
| Connectors              | 2x 4-pin 1mm JST SH            |

---

## I2C Addressing & Multiplexing

The *I2Connect: OLED* module uses a fixed I2C address of:

```text
0x3C
```
This address cannot be modified. If multiple OLED modules are required in the same system, an I2C multiplexer must be used.
The PTSolns I2Connect: MUX module provides four independent I2C channels, allowing up to four OLED modules with the same fixed I2C address to be connected to a single microcontroller.

---

## Advanced Graphics & Geometry

The **PTSolns I2Connect: OLED** library is intentionally designed to be extremely lightweight and strictly dependency-free. It provides a highly optimized, low-memory solution for printing text, sensor readouts, and basic status messages without consuming the limited SRAM on older microcontrollers like the Arduino UNO.

If your project requires advanced visual elements—such as geometric shapes (circles, triangles, line graphs), custom variable-sized fonts, or bitmap logos—the *I2Connect: OLED* hardware module is 100% physically compatible with the open-source **Adafruit_SSD1306** graphics engine.

To use advanced graphics, simply install the Adafruit SSD1306 and Adafruit GFX libraries and initialize the display using the module's fixed I2C address (`0x3C`). 

*Note: Please reference the `04_Advanced_Adafruit_Graphics` sketch located in the `examples` folder of this library for a complete, plug-and-play template on how to configure the Adafruit engine for this specific hardware.*

---

## *PTSolns IDE*

The [*PTSolns IDE*](https://ptsolns.com/ide) is a free desktop application for programming PTSolns microcontroller development boards and compatible Arduino platforms.

This library is available directly through both the PTSolns IDE Library Manager and the Arduino Library Manager.

To install:

1. Open the PTSolns IDE (or Arduino IDE).
2. Open the Library Manager.
3. Search for:

```text
I2Connect_OLED
```

4. Click Install.

Features of the [*PTSolns IDE*](https://ptsolns.com/ide) include:

* Integrated board management
* Built in serial monitor
* Library management
* Example project browser
* Support for over 8,500 third party libraries
* Built in CH340 USB driver support
* Open source and community driven

The [*PTSolns IDE*](https://ptsolns.com/ide) can be used alongside the Arduino IDE and supports many PTSolns development boards out of the box.

---

## Quick Start

```cpp
#include <Wire.h>
#include <PTSOLNS_I2CONNECT_OLED.h>

PTSOLNS_I2CONNECT_OLED display;

void setup()
{
    Serial.begin(115200);
    Wire.begin();

    // Initialize the display. The library automatically defaults to 0x3C.
    if (!display.begin()) {
        Serial.println("Failed to find OLED display.");
        while (true); // Halt if display is not found
    }

    display.clearDisplay();
    
    // Configure text properties
    display.setTextSize(2);
    display.setTextColor(1); // 1 = White, 0 = Black
    display.setCursor(10, 20);
    
    // Print to the screen buffer
    display.print("PTSolns");
    
    // Push the buffer to the physical screen
    display.display();
}

void loop()
{
    // Screen remains active without continuous updates
}
```

---

## Library Functions

| Function          | Description                                    |
| ----------------- | -----------------------------------------------|
| begin()           | Initialize the screen buffer and I2C hardware  |
| clearDisplay()    | Wipe the screen buffer clean                   |
| display()         | Push the current buffer to the physical screen |
| print()           | Print strings, integers, or floats to buffer   |
| setCursor(x, y)   | Set the starting coordinates for text          |
| setTextSize(s)    | Scale the text size (1 = default, 2 = double)  |
| drawPixel(x, y, c)| Draw a single pixel at specific coordinates    |
| drawLine(...)     | Draw a line between two points                 |
| drawRect(...)     | Draw an outlined rectangle                     |

---

## Basic Functions

### Initialization

```cpp
// Initializes screen at default address 0x3C
display.begin();
```

### Updating the Screen

```cpp
// All drawing commands only update the internal memory buffer.
// You MUST call display() to make the changes visible on the screen.
display.display();
```

### Drawing Shapes

```cpp
// Draw a filled circle at x=64, y=32 with a radius of 10
display.fillCircle(64, 32, 10, 1);
display.display();
```

---

## Typical Applications

* Custom user interfaces and menus
* Live sensor data readouts
* Diagnostic status monitors
* Smart home dashboard displays
* Graphical visualization (progress bars, line graphs)

---
## Included Examples

| Example                | Description                                          |
| ---------------------- | ---------------------------------------------------- |
| 01_HelloWorld          | Initialize the screen and print basic text           |
| 02_SensorDisplay       | Formats live AHT20 sensor data on the screen         |
| 03_GraphicsTest        | Demonstrates drawing lines, circles, and rectangles  |
| 04_CustomBitmap        | Shows how to render a custom logo from PROGMEM       |

---

## Documentation & Resources

Additional resources are available through the PTSolns Documentation Repository, including:

* Datasheets
* Schematics
* Pinout diagrams
* 3D models
* Fritzing files
* Tutorials
* Example projects

* [PTSolns Documentation Repository](https://docs.ptsolns.com)
* [PTSolns Website](https://ptsolns.com)
* [Contact Support](https://ptsolns.com/contact-us)
* [PTSolns IDE](https://ptsolns.com/ide)
