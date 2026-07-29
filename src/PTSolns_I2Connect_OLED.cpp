#include "PTSolns_I2Connect_OLED.h"
#include <avr/pgmspace.h>
#include <math.h>

static inline void ptsolns_swap16(int16_t &a, int16_t &b) {
    int16_t t = a;
    a = b;
    b = t;
}

static const uint8_t font5x7[][5] PROGMEM = {
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x5F, 0x00, 0x00},
    {0x00, 0x07, 0x00, 0x07, 0x00},
    {0x14, 0x7F, 0x14, 0x7F, 0x14},
    {0x24, 0x2A, 0x7F, 0x2A, 0x12},
    {0x23, 0x13, 0x08, 0x64, 0x62},
    {0x36, 0x49, 0x55, 0x22, 0x50},
    {0x00, 0x05, 0x03, 0x00, 0x00},
    {0x00, 0x1C, 0x22, 0x41, 0x00},
    {0x00, 0x41, 0x22, 0x1C, 0x00},
    {0x14, 0x08, 0x3E, 0x08, 0x14},
    {0x08, 0x08, 0x3E, 0x08, 0x08},
    {0x00, 0x50, 0x30, 0x00, 0x00},
    {0x08, 0x08, 0x08, 0x08, 0x08},
    {0x00, 0x60, 0x60, 0x00, 0x00},
    {0x20, 0x10, 0x08, 0x04, 0x02},
    {0x3E, 0x51, 0x49, 0x45, 0x3E},
    {0x00, 0x42, 0x7F, 0x40, 0x00},
    {0x42, 0x61, 0x51, 0x49, 0x46},
    {0x21, 0x41, 0x45, 0x4B, 0x31},
    {0x18, 0x14, 0x12, 0x7F, 0x10},
    {0x27, 0x45, 0x45, 0x45, 0x39},
    {0x3C, 0x4A, 0x49, 0x49, 0x30},
    {0x01, 0x71, 0x09, 0x05, 0x03},
    {0x36, 0x49, 0x49, 0x49, 0x36},
    {0x06, 0x49, 0x49, 0x29, 0x1E},
    {0x00, 0x36, 0x36, 0x00, 0x00},
    {0x00, 0x56, 0x36, 0x00, 0x00},
    {0x08, 0x14, 0x22, 0x41, 0x00},
    {0x14, 0x14, 0x14, 0x14, 0x14},
    {0x00, 0x41, 0x22, 0x14, 0x08},
    {0x02, 0x01, 0x51, 0x09, 0x06},
    {0x32, 0x49, 0x79, 0x41, 0x3E},
    {0x7E, 0x11, 0x11, 0x11, 0x7E},
    {0x7F, 0x49, 0x49, 0x49, 0x36},
    {0x3E, 0x41, 0x41, 0x41, 0x22},
    {0x7F, 0x41, 0x41, 0x22, 0x1C},
    {0x7F, 0x49, 0x49, 0x49, 0x41},
    {0x7F, 0x09, 0x09, 0x09, 0x01},
    {0x3E, 0x41, 0x49, 0x49, 0x7A},
    {0x7F, 0x08, 0x08, 0x08, 0x7F},
    {0x00, 0x41, 0x7F, 0x41, 0x00},
    {0x20, 0x40, 0x41, 0x3F, 0x01},
    {0x7F, 0x08, 0x14, 0x22, 0x41},
    {0x7F, 0x40, 0x40, 0x40, 0x40},
    {0x7F, 0x02, 0x0C, 0x02, 0x7F},
    {0x7F, 0x04, 0x08, 0x10, 0x7F},
    {0x3E, 0x41, 0x41, 0x41, 0x3E},
    {0x7F, 0x09, 0x09, 0x09, 0x06},
    {0x3E, 0x41, 0x51, 0x21, 0x5E},
    {0x7F, 0x09, 0x19, 0x29, 0x46},
    {0x46, 0x49, 0x49, 0x49, 0x31},
    {0x01, 0x01, 0x7F, 0x01, 0x01},
    {0x3F, 0x40, 0x40, 0x40, 0x3F},
    {0x1F, 0x20, 0x40, 0x20, 0x1F},
    {0x3F, 0x40, 0x38, 0x40, 0x3F},
    {0x63, 0x14, 0x08, 0x14, 0x63},
    {0x07, 0x08, 0x70, 0x08, 0x07},
    {0x61, 0x51, 0x49, 0x45, 0x43},
    {0x00, 0x7F, 0x41, 0x41, 0x00},
    {0x02, 0x04, 0x08, 0x10, 0x20},
    {0x00, 0x41, 0x41, 0x7F, 0x00},
    {0x04, 0x02, 0x01, 0x02, 0x04},
    {0x40, 0x40, 0x40, 0x40, 0x40},
    {0x00, 0x01, 0x02, 0x04, 0x00},
    {0x20, 0x54, 0x54, 0x54, 0x78},
    {0x7F, 0x48, 0x44, 0x44, 0x38},
    {0x38, 0x44, 0x44, 0x44, 0x20},
    {0x38, 0x44, 0x44, 0x48, 0x7F},
    {0x38, 0x54, 0x54, 0x54, 0x18},
    {0x08, 0x7E, 0x09, 0x01, 0x02},
    {0x0C, 0x52, 0x52, 0x52, 0x3E},
    {0x7F, 0x08, 0x04, 0x04, 0x78},
    {0x00, 0x44, 0x7D, 0x40, 0x00},
    {0x20, 0x40, 0x44, 0x3D, 0x00},
    {0x7F, 0x10, 0x28, 0x44, 0x00},
    {0x00, 0x41, 0x7F, 0x40, 0x00},
    {0x7C, 0x04, 0x18, 0x04, 0x78},
    {0x7C, 0x08, 0x04, 0x04, 0x78},
    {0x38, 0x44, 0x44, 0x44, 0x38},
    {0x7C, 0x14, 0x14, 0x14, 0x08},
    {0x08, 0x14, 0x14, 0x18, 0x7C},
    {0x7C, 0x08, 0x04, 0x04, 0x08},
    {0x48, 0x54, 0x54, 0x54, 0x20},
    {0x04, 0x3F, 0x44, 0x40, 0x20},
    {0x3C, 0x40, 0x40, 0x20, 0x7C},
    {0x1C, 0x20, 0x40, 0x20, 0x1C},
    {0x3C, 0x40, 0x30, 0x40, 0x3C},
    {0x44, 0x28, 0x10, 0x28, 0x44},
    {0x0C, 0x50, 0x50, 0x50, 0x3C},
    {0x44, 0x64, 0x54, 0x4C, 0x44},
    {0x00, 0x08, 0x36, 0x41, 0x00},
    {0x00, 0x00, 0x7F, 0x00, 0x00},
    {0x00, 0x41, 0x36, 0x08, 0x00},
    {0x08, 0x08, 0x2A, 0x1C, 0x08},
};

PTSolns_I2Connect_OLED::PTSolns_I2Connect_OLED() {
    _wire = &Wire;
    _i2cAddress = DEFAULT_I2C_ADDRESS;
    _i2cClock = DEFAULT_I2C_CLOCK;
    _cursorX = 0;
    _cursorY = 0;
}

bool PTSolns_I2Connect_OLED::begin(TwoWire &wirePort, uint8_t i2cAddress, uint32_t i2cClock) {
    _wire = &wirePort;
    _i2cAddress = i2cAddress;
    _i2cClock = i2cClock;

    _wire->begin();
    
    if (!isConnected()) {
        return false;
    }

    _wire->setClock(_i2cClock);

    sendCommand(0xAE); 
    
    sendCommand(0x20); 
    sendCommand(0x00); 
    
    sendCommand(0x21); 
    sendCommand(0x00); 
    sendCommand(0x7F); 
    
    sendCommand(0x22); 
    sendCommand(0x00); 
    sendCommand(0x07); 
    
    sendCommand(0xA0); 
    sendCommand(0xC0); 

    sendCommand(0x8D); 
    sendCommand(0x14); 
    
    sendCommand(0xAF); 

    clearDisplay();
    display();

    return true;
}

bool PTSolns_I2Connect_OLED::isConnected() {
    _wire->beginTransmission(_i2cAddress);
    return (_wire->endTransmission() == 0);
}

void PTSolns_I2Connect_OLED::sendCommand(uint8_t command) {
    _wire->beginTransmission(_i2cAddress);
    _wire->write(0x00); 
    _wire->write(command);
    _wire->endTransmission();
}

void PTSolns_I2Connect_OLED::sendData(uint8_t data) {
    _wire->beginTransmission(_i2cAddress);
    _wire->write(0x40); 
    _wire->write(data);
    _wire->endTransmission();
}

void PTSolns_I2Connect_OLED::clearDisplay() {
    memset(_buffer, 0x00, sizeof(_buffer)); 
    _cursorX = 0;
    _cursorY = 0;
}

void PTSolns_I2Connect_OLED::setCursor(int16_t x, int16_t y) {
    _cursorX = x;
    _cursorY = y;
}

void PTSolns_I2Connect_OLED::setRotation(bool flip) {
    if (flip) {
        sendCommand(0xA1); 
        sendCommand(0xC8); 
    } else {
        sendCommand(0xA0); 
        sendCommand(0xC0); 
    }
}

void PTSolns_I2Connect_OLED::drawPixel(int16_t x, int16_t y, bool color) {
    if (x < 0 || x >= 128 || y < 0 || y >= 64) {
        return; 
    }

    uint8_t page = y / 8;
    uint8_t bit  = y % 8;
    uint16_t index = (uint16_t)page * 128 + x;

    if (color) {
        _buffer[index] |= (1 << bit);   
    } else {
        _buffer[index] &= ~(1 << bit);  
    }
}

void PTSolns_I2Connect_OLED::drawCircle(int16_t x0, int16_t y0, int16_t r, bool color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    drawPixel(x0, y0 + r, color);
    drawPixel(x0, y0 - r, color);
    drawPixel(x0 + r, y0, color);
    drawPixel(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        drawPixel(x0 + x, y0 + y, color);
        drawPixel(x0 - x, y0 + y, color);
        drawPixel(x0 + x, y0 - y, color);
        drawPixel(x0 - x, y0 - y, color);
        drawPixel(x0 + y, y0 + x, color);
        drawPixel(x0 - y, y0 + x, color);
        drawPixel(x0 + y, y0 - x, color);
        drawPixel(x0 - y, y0 - x, color);
    }
}

void PTSolns_I2Connect_OLED::fillCircle(int16_t x0, int16_t y0, int16_t r, bool color) {
    for (int16_t y = -r; y <= r; y++) {
        int16_t dx = (int16_t)sqrt((float)(r * r - y * y));
        for (int16_t x = -dx; x <= dx; x++) {
            drawPixel(x0 + x, y0 + y, color);
        }
    }
}

void PTSolns_I2Connect_OLED::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, bool color) {
    bool steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        ptsolns_swap16(x0, y0);
        ptsolns_swap16(x1, y1);
    }
    if (x0 > x1) {
        ptsolns_swap16(x0, x1);
        ptsolns_swap16(y0, y1);
    }

    int16_t dx = x1 - x0;
    int16_t dy = abs(y1 - y0);
    int16_t err = dx / 2;
    int16_t ystep = (y0 < y1) ? 1 : -1;

    for (; x0 <= x1; x0++) {
        if (steep) {
            drawPixel(y0, x0, color);
        } else {
            drawPixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

void PTSolns_I2Connect_OLED::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, bool color) {
    drawLine(x, y, x + w - 1, y, color);                 
    drawLine(x, y + h - 1, x + w - 1, y + h - 1, color);  
    drawLine(x, y, x, y + h - 1, color);                  
    drawLine(x + w - 1, y, x + w - 1, y + h - 1, color);  
}

void PTSolns_I2Connect_OLED::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, bool color) {
    for (int16_t i = x; i < x + w; i++) {
        drawLine(i, y, i, y + h - 1, color);
    }
}

void PTSolns_I2Connect_OLED::drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornerMask, bool color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        if (cornerMask & 0x4) { 
            drawPixel(x0 + x, y0 + y, color);
            drawPixel(x0 + y, y0 + x, color);
        }
        if (cornerMask & 0x2) { 
            drawPixel(x0 + x, y0 - y, color);
            drawPixel(x0 + y, y0 - x, color);
        }
        if (cornerMask & 0x8) { 
            drawPixel(x0 - y, y0 + x, color);
            drawPixel(x0 - x, y0 + y, color);
        }
        if (cornerMask & 0x1) { 
            drawPixel(x0 - y, y0 - x, color);
            drawPixel(x0 - x, y0 - y, color);
        }
    }
}

void PTSolns_I2Connect_OLED::fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornerMask, int16_t delta, bool color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;
    int16_t px = x;
    int16_t py = y;

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        if (x < (y + 1)) {
            if (cornerMask & 0x1) drawLine(x0 + x, y0 - y, x0 + x, y0 - y + 2 * y + 1 + delta, color);
            if (cornerMask & 0x2) drawLine(x0 - x, y0 - y, x0 - x, y0 - y + 2 * y + 1 + delta, color);
        }
        if (y != py) {
            if (cornerMask & 0x1) drawLine(x0 + py, y0 - px, x0 + py, y0 - px + 2 * px + 1 + delta, color);
            if (cornerMask & 0x2) drawLine(x0 - py, y0 - px, x0 - py, y0 - px + 2 * px + 1 + delta, color);
            py = y;
        }
        px = x;
    }
}

void PTSolns_I2Connect_OLED::drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, bool color) {
    drawLine(x + r, y, x + w - r - 1, y, color);                
    drawLine(x + r, y + h - 1, x + w - r - 1, y + h - 1, color); 
    drawLine(x, y + r, x, y + h - r - 1, color);                 
    drawLine(x + w - 1, y + r, x + w - 1, y + h - r - 1, color); 

    drawCircleHelper(x + r, y + r, r, 0x1, color);                  
    drawCircleHelper(x + w - r - 1, y + r, r, 0x2, color);          
    drawCircleHelper(x + r, y + h - r - 1, r, 0x8, color);          
    drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 0x4, color);  
}

void PTSolns_I2Connect_OLED::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, bool color) {
    fillRect(x + r, y, w - 2 * r, h, color); 

    fillCircleHelper(x + w - r - 1, y + r, r, 0x1, h - 2 * r - 1, color); 
    fillCircleHelper(x + r, y + r, r, 0x2, h - 2 * r - 1, color);         
}

void PTSolns_I2Connect_OLED::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, bool color) {
    drawLine(x0, y0, x1, y1, color);
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x0, y0, color);
}

void PTSolns_I2Connect_OLED::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, bool color) {
    int16_t a, b, y, last;

    if (y0 > y1) { ptsolns_swap16(y0, y1); ptsolns_swap16(x0, x1); }
    if (y1 > y2) { ptsolns_swap16(y1, y2); ptsolns_swap16(x1, x2); }
    if (y0 > y1) { ptsolns_swap16(y0, y1); ptsolns_swap16(x0, x1); }

    if (y0 == y2) { 
        a = b = x0;
        if (x1 < a) a = x1; else if (x1 > b) b = x1;
        if (x2 < a) a = x2; else if (x2 > b) b = x2;
        drawLine(a, y0, b, y0, color);
        return;
    }

    int16_t dx01 = x1 - x0, dy01 = y1 - y0;
    int16_t dx02 = x2 - x0, dy02 = y2 - y0;
    int16_t dx12 = x2 - x1, dy12 = y2 - y1;
    int32_t sa = 0, sb = 0;

    last = (y1 == y2) ? y1 : y1 - 1;

    for (y = y0; y <= last; y++) {
        a = x0 + sa / dy01;
        b = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        if (a > b) ptsolns_swap16(a, b);
        drawLine(a, y, b, y, color);
    }

    sa = (int32_t)dx12 * (y - y1);
    sb = (int32_t)dx02 * (y - y0);
    for (; y <= y2; y++) {
        a = x1 + sa / dy12;
        b = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        if (a > b) ptsolns_swap16(a, b);
        drawLine(a, y, b, y, color);
    }
}

void PTSolns_I2Connect_OLED::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, bool color) {
    int16_t byteWidth = (w + 7) / 8;
    uint8_t b = 0;

    for (int16_t j = 0; j < h; j++) {
        for (int16_t i = 0; i < w; i++) {
            if (i & 7) {
                b <<= 1;
            } else {
                b = pgm_read_byte(bitmap + j * byteWidth + i / 8);
            }
            if (b & 0x80) {
                drawPixel(x + i, y + j, color);
            }
        }
    }
}

void PTSolns_I2Connect_OLED::display() {
    sendCommand(0x21); 
    sendCommand(0x00);
    sendCommand(0x7F);
    sendCommand(0x22); 
    sendCommand(0x00);
    sendCommand(0x07);

    for (uint16_t i = 0; i < sizeof(_buffer); i += 16) {
        _wire->beginTransmission(_i2cAddress);
        _wire->write(0x40); 
        for (uint8_t j = 0; j < 16; j++) {
            _wire->write(_buffer[i + j]);
        }
        _wire->endTransmission();
    }
}

size_t PTSolns_I2Connect_OLED::write(uint8_t c) {
    if (c == '\n') {
        _cursorY += 8;
        _cursorX = 0;
        return 1;
    }
    if (c == '\r') {
        return 1;
    }

    if (_cursorX >= 128) {
        _cursorX = 0;
        _cursorY += 8;
    }
    if (_cursorY >= 64) {
        _cursorY = 0; 
    }

    if (c >= 0x20 && c <= 0x7E) {
        uint8_t charIndex = c - 0x20;

        uint8_t page = _cursorY / 8;

        for (uint8_t i = 0; i < 5; i++) {
            uint8_t line = pgm_read_byte(&font5x7[charIndex][i]);

            uint16_t bufIndex = (uint16_t)page * 128 + _cursorX + i;
            if (bufIndex < sizeof(_buffer)) {
                _buffer[bufIndex] = line;
            }
        }

        uint16_t spacingIndex = (uint16_t)page * 128 + _cursorX + 5;
        if (spacingIndex < sizeof(_buffer)) {
            _buffer[spacingIndex] = 0x00;
        }
    }

    _cursorX += 6; 
    return 1;
}
