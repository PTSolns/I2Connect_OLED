#ifndef PTSOLNS_I2CONNECT_OLED_H
#define PTSOLNS_I2CONNECT_OLED_H

#include <Arduino.h>
#include <Wire.h>
#include <Print.h>

class PTSolns_I2Connect_OLED : public Print {
public:
    static const uint8_t DEFAULT_I2C_ADDRESS = 0x3C;
    static const uint32_t DEFAULT_I2C_CLOCK = 400000UL;

    PTSolns_I2Connect_OLED();

    // Initialization
    bool begin(TwoWire &wirePort = Wire, uint8_t i2cAddress = DEFAULT_I2C_ADDRESS, uint32_t i2cClock = DEFAULT_I2C_CLOCK);

    // Screen Control
    void clearDisplay();
    void display();
    void setCursor(int16_t x, int16_t y);

    // Graphics primitives
    void drawPixel(int16_t x, int16_t y, bool color = true);
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, bool color = true);
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, bool color = true);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, bool color = true);
    void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, bool color = true);
    void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, bool color = true);
    void drawCircle(int16_t x0, int16_t y0, int16_t r, bool color = true);
    void fillCircle(int16_t x0, int16_t y0, int16_t r, bool color = true);
    void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, bool color = true);
    void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, bool color = true);
    void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, bool color = true);

    virtual size_t write(uint8_t c);

private:
    TwoWire *_wire;
    uint8_t _i2cAddress;
    uint32_t _i2cClock;
    
    int16_t _cursorX;
    int16_t _cursorY;
    
    // 128x64 display , 1024-byte buffer (128 * 64 / 8)
    uint8_t _buffer[1024]; 

    // Low-level hardware communication
    void sendCommand(uint8_t command);
    void sendData(uint8_t data);
    bool isConnected();

    // Internal helpers for compound shapes
    void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornerMask, bool color);
    void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornerMask, int16_t delta, bool color);
};

#endif