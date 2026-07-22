#include "PTSolns_I2Connect_OLED.h"
#include <avr/pgmspace.h>
#include <math.h>

// Named to avoid clashing with any swap() macro some Arduino cores define
static inline void ptsolns_swap16(int16_t &a, int16_t &b) {
    int16_t t = a;
    a = b;
    b = t;
}

// =========================================================
// Standard 5x7 ASCII font, columns LSB-first (bit0 = top row)
// Covers 0x20 (space) through 0x7E (~), 95 glyphs
// =========================================================
static const uint8_t font5x7[][5] PROGMEM = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, // 0x20 space
    {0x00, 0x00, 0x5F, 0x00, 0x00}, // 0x21 !
    {0x00, 0x07, 0x00, 0x07, 0x00}, // 0x22 "
    {0x14, 0x7F, 0x14, 0x7F, 0x14}, // 0x23 #
    {0x24, 0x2A, 0x7F, 0x2A, 0x12}, // 0x24 $
    {0x23, 0x13, 0x08, 0x64, 0x62}, // 0x25 %
    {0x36, 0x49, 0x55, 0x22, 0x50}, // 0x26 &
    {0x00, 0x05, 0x03, 0x00, 0x00}, // 0x27 '
    {0x00, 0x1C, 0x22, 0x41, 0x00}, // 0x28 (
    {0x00, 0x41, 0x22, 0x1C, 0x00}, // 0x29 )
    {0x14, 0x08, 0x3E, 0x08, 0x14}, // 0x2A *
    {0x08, 0x08, 0x3E, 0x08, 0x08}, // 0x2B +
    {0x00, 0x50, 0x30, 0x00, 0x00}, // 0x2C ,
    {0x08, 0x08, 0x08, 0x08, 0x08}, // 0x2D -
    {0x00, 0x60, 0x60, 0x00, 0x00}, // 0x2E .
    {0x20, 0x10, 0x08, 0x04, 0x02}, // 0x2F /
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0x30 0
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 0x31 1
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 0x32 2
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // 0x33 3
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 0x34 4
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 0x35 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 0x36 6
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 0x37 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 0x38 8
    {0x06, 0x49, 0x49, 0x29, 0x1E}, // 0x39 9
    {0x00, 0x36, 0x36, 0x00, 0x00}, // 0x3A :
    {0x00, 0x56, 0x36, 0x00, 0x00}, // 0x3B ;
    {0x08, 0x14, 0x22, 0x41, 0x00}, // 0x3C <
    {0x14, 0x14, 0x14, 0x14, 0x14}, // 0x3D =
    {0x00, 0x41, 0x22, 0x14, 0x08}, // 0x3E >
    {0x02, 0x01, 0x51, 0x09, 0x06}, // 0x3F ?
    {0x32, 0x49, 0x79, 0x41, 0x3E}, // 0x40 @
    {0x7E, 0x11, 0x11, 0x11, 0x7E}, // 0x41 A
    {0x7F, 0x49, 0x49, 0x49, 0x36}, // 0x42 B
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // 0x43 C
    {0x7F, 0x41, 0x41, 0x22, 0x1C}, // 0x44 D
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // 0x45 E
    {0x7F, 0x09, 0x09, 0x09, 0x01}, // 0x46 F
    {0x3E, 0x41, 0x49, 0x49, 0x7A}, // 0x47 G
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // 0x48 H
    {0x00, 0x41, 0x7F, 0x41, 0x00}, // 0x49 I
    {0x20, 0x40, 0x41, 0x3F, 0x01}, // 0x4A J
    {0x7F, 0x08, 0x14, 0x22, 0x41}, // 0x4B K
    {0x7F, 0x40, 0x40, 0x40, 0x40}, // 0x4C L
    {0x7F, 0x02, 0x0C, 0x02, 0x7F}, // 0x4D M
    {0x7F, 0x04, 0x08, 0x10, 0x7F}, // 0x4E N
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // 0x4F O
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // 0x50 P
    {0x3E, 0x41, 0x51, 0x21, 0x5E}, // 0x51 Q
    {0x7F, 0x09, 0x19, 0x29, 0x46}, // 0x52 R
    {0x46, 0x49, 0x49, 0x49, 0x31}, // 0x53 S
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // 0x54 T
    {0x3F, 0x40, 0x40, 0x40, 0x3F}, // 0x55 U
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, // 0x56 V
    {0x3F, 0x40, 0x38, 0x40, 0x3F}, // 0x57 W
    {0x63, 0x14, 0x08, 0x14, 0x63}, // 0x58 X
    {0x07, 0x08, 0x70, 0x08, 0x07}, // 0x59 Y
    {0x61, 0x51, 0x49, 0x45, 0x43}, // 0x5A Z
    {0x00, 0x7F, 0x41, 0x41, 0x00}, // 0x5B [
    {0x02, 0x04, 0x08, 0x10, 0x20}, // 0x5C backslash
    {0x00, 0x41, 0x41, 0x7F, 0x00}, // 0x5D ]
    {0x04, 0x02, 0x01, 0x02, 0x04}, // 0x5E ^
    {0x40, 0x40, 0x40, 0x40, 0x40}, // 0x5F _
    {0x00, 0x01, 0x02, 0x04, 0x00}, // 0x60 `
    {0x20, 0x54, 0x54, 0x54, 0x78}, // 0x61 a
    {0x7F, 0x48, 0x44, 0x44, 0x38}, // 0x62 b
    {0x38, 0x44, 0x44, 0x44, 0x20}, // 0x63 c
    {0x38, 0x44, 0x44, 0x48, 0x7F}, // 0x64 d
    {0x38, 0x54, 0x54, 0x54, 0x18}, // 0x65 e
    {0x08, 0x7E, 0x09, 0x01, 0x02}, // 0x66 f
    {0x0C, 0x52, 0x52, 0x52, 0x3E}, // 0x67 g
    {0x7F, 0x08, 0x04, 0x04, 0x78}, // 0x68 h
    {0x00, 0x44, 0x7D, 0x40, 0x00}, // 0x69 i
    {0x20, 0x40, 0x44, 0x3D, 0x00}, // 0x6A j
    {0x7F, 0x10, 0x28, 0x44, 0x00}, // 0x6B k
    {0x00, 0x41, 0x7F, 0x40, 0x00}, // 0x6C l
    {0x7C, 0x04, 0x18, 0x04, 0x78}, // 0x6D m
    {0x7C, 0x08, 0x04, 0x04, 0x78}, // 0x6E n
    {0x38, 0x44, 0x44, 0x44, 0x38}, // 0x6F o
    {0x7C, 0x14, 0x14, 0x14, 0x08}, // 0x70 p
    {0x08, 0x14, 0x14, 0x18, 0x7C}, // 0x71 q
    {0x7C, 0x08, 0x04, 0x04, 0x08}, // 0x72 r
    {0x48, 0x54, 0x54, 0x54, 0x20}, // 0x73 s
    {0x04, 0x3F, 0x44, 0x40, 0x20}, // 0x74 t
    {0x3C, 0x40, 0x40, 0x20, 0x7C}, // 0x75 u
    {0x1C, 0x20, 0x40, 0x20, 0x1C}, // 0x76 v
    {0x3C, 0x40, 0x30, 0x40, 0x3C}, // 0x77 w
    {0x44, 0x28, 0x10, 0x28, 0x44}, // 0x78 x
    {0x0C, 0x50, 0x50, 0x50, 0x3C}, // 0x79 y
    {0x44, 0x64, 0x54, 0x4C, 0x44}, // 0x7A z
    {0x00, 0x08, 0x36, 0x41, 0x00}, // 0x7B {
    {0x00, 0x00, 0x7F, 0x00, 0x00}, // 0x7C |
    {0x00, 0x41, 0x36, 0x08, 0x00}, // 0x7D }
    {0x08, 0x08, 0x2A, 0x1C, 0x08}, // 0x7E ~
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
    
    // Verify hardware is attached before pushing initialization sequence
    if (!isConnected()) {
        return false;
    }

    // Set the requested I2C speed
    _wire->setClock(_i2cClock);

    // SSD1306 Initialization Sequence
    sendCommand(0xAE); // Display OFF
    
    sendCommand(0x20); // Set Memory Addressing Mode
    sendCommand(0x00); // Horizontal Addressing Mode
    
    sendCommand(0x21); // Set Column Address
    sendCommand(0x00); // Start
    sendCommand(0x7F); // End (127)
    
    sendCommand(0x22); // Set Page Address
    sendCommand(0x00); // Start
    sendCommand(0x07); // End (7)
    
    sendCommand(0x8D); // Charge Pump Setting
    sendCommand(0x14); // Enable Charge Pump (generates high voltage from 3.3V)
    
    sendCommand(0xAF); // Display ON

    // Wipe memory and screen on startup
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
    _wire->write(0x00); // Control byte: Co = 0, D/C = 0 (Command)
    _wire->write(command);
    _wire->endTransmission();
}

void PTSolns_I2Connect_OLED::sendData(uint8_t data) {
    _wire->beginTransmission(_i2cAddress);
    _wire->write(0x40); // Control byte: Co = 0, D/C = 1 (Data)
    _wire->write(data);
    _wire->endTransmission();
}

void PTSolns_I2Connect_OLED::clearDisplay() {
    // 0x00 = all pixels OFF. Use 0xFF only if you deliberately want an
    // all-lit "inverse" screen — that's not what a normal clear should do.
    memset(_buffer, 0x00, sizeof(_buffer)); 
    _cursorX = 0;
    _cursorY = 0;
}

void PTSolns_I2Connect_OLED::setCursor(int16_t x, int16_t y) {
    _cursorX = x;
    _cursorY = y;
}

void PTSolns_I2Connect_OLED::drawPixel(int16_t x, int16_t y, bool color) {
    if (x < 0 || x >= 128 || y < 0 || y >= 64) {
        return; // off-screen, ignore
    }

    uint8_t page = y / 8;
    uint8_t bit  = y % 8;
    uint16_t index = (uint16_t)page * 128 + x;

    if (color) {
        _buffer[index] |= (1 << bit);   // set the bit (pixel ON)
    } else {
        _buffer[index] &= ~(1 << bit);  // clear the bit (pixel OFF)
    }
}

// Midpoint circle algorithm (Bresenham's circle) - draws the outline only
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

// Filled circle - draws vertical scanlines between the two edges at each x offset
void PTSolns_I2Connect_OLED::fillCircle(int16_t x0, int16_t y0, int16_t r, bool color) {
    for (int16_t y = -r; y <= r; y++) {
        int16_t dx = (int16_t)sqrt((float)(r * r - y * y));
        for (int16_t x = -dx; x <= dx; x++) {
            drawPixel(x0 + x, y0 + y, color);
        }
    }
}

// Bresenham line algorithm - works in all 8 octants
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
    drawLine(x, y, x + w - 1, y, color);                 // top
    drawLine(x, y + h - 1, x + w - 1, y + h - 1, color);  // bottom
    drawLine(x, y, x, y + h - 1, color);                  // left
    drawLine(x + w - 1, y, x + w - 1, y + h - 1, color);  // right
}

void PTSolns_I2Connect_OLED::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, bool color) {
    for (int16_t i = x; i < x + w; i++) {
        drawLine(i, y, i, y + h - 1, color);
    }
}

// Draws one or more quarter-arcs, used to build rounded rect corners.
// cornerMask bits: 0x1 = top-left, 0x2 = top-right, 0x4 = bottom-right, 0x8 = bottom-left
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

        if (cornerMask & 0x4) { // bottom-right
            drawPixel(x0 + x, y0 + y, color);
            drawPixel(x0 + y, y0 + x, color);
        }
        if (cornerMask & 0x2) { // top-right
            drawPixel(x0 + x, y0 - y, color);
            drawPixel(x0 + y, y0 - x, color);
        }
        if (cornerMask & 0x8) { // bottom-left
            drawPixel(x0 - y, y0 + x, color);
            drawPixel(x0 - x, y0 + y, color);
        }
        if (cornerMask & 0x1) { // top-left
            drawPixel(x0 - y, y0 - x, color);
            drawPixel(x0 - x, y0 - y, color);
        }
    }
}

// Filled quarter-arc, used to build filled rounded rect corners.
// 'delta' extends the fill lines to bridge into the rectangle's straight edge.
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
    drawLine(x + r, y, x + w - r - 1, y, color);                // top edge
    drawLine(x + r, y + h - 1, x + w - r - 1, y + h - 1, color); // bottom edge
    drawLine(x, y + r, x, y + h - r - 1, color);                 // left edge
    drawLine(x + w - 1, y + r, x + w - 1, y + h - r - 1, color); // right edge

    drawCircleHelper(x + r, y + r, r, 0x1, color);                  // top-left
    drawCircleHelper(x + w - r - 1, y + r, r, 0x2, color);          // top-right
    drawCircleHelper(x + r, y + h - r - 1, r, 0x8, color);          // bottom-left
    drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 0x4, color);  // bottom-right
}

void PTSolns_I2Connect_OLED::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, bool color) {
    fillRect(x + r, y, w - 2 * r, h, color); // center body

    fillCircleHelper(x + w - r - 1, y + r, r, 0x1, h - 2 * r - 1, color); // right half
    fillCircleHelper(x + r, y + r, r, 0x2, h - 2 * r - 1, color);         // left half
}

void PTSolns_I2Connect_OLED::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, bool color) {
    drawLine(x0, y0, x1, y1, color);
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x0, y0, color);
}

// Fills using horizontal scanlines, splitting at the middle vertex's y-coordinate
void PTSolns_I2Connect_OLED::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, bool color) {
    int16_t a, b, y, last;

    // Sort vertices so y0 <= y1 <= y2
    if (y0 > y1) { ptsolns_swap16(y0, y1); ptsolns_swap16(x0, x1); }
    if (y1 > y2) { ptsolns_swap16(y1, y2); ptsolns_swap16(x1, x2); }
    if (y0 > y1) { ptsolns_swap16(y0, y1); ptsolns_swap16(x0, x1); }

    if (y0 == y2) { // Degenerate case: a flat line
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

// Blits a 1-bit-per-pixel bitmap from PROGMEM. Rows are packed MSB-first,
// padded to a whole byte at the end of each row (standard XBM-style format).
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
    // Reset pointers to start of memory before pushing the buffer
    sendCommand(0x21); 
    sendCommand(0x00);
    sendCommand(0x7F);
    sendCommand(0x22); 
    sendCommand(0x00);
    sendCommand(0x07);

    // Push the 1024-byte buffer to the screen in I2C chunks
    for (uint16_t i = 0; i < sizeof(_buffer); i += 16) {
        _wire->beginTransmission(_i2cAddress);
        _wire->write(0x40); // Data mode
        for (uint8_t j = 0; j < 16; j++) {
            _wire->write(_buffer[i + j]);
        }
        _wire->endTransmission();
    }
}

// This function processes every single character passed into print() or println()
size_t PTSolns_I2Connect_OLED::write(uint8_t c) {
    // Handle line breaks
    if (c == '\n') {
        _cursorY += 8;
        _cursorX = 0;
        return 1;
    }
    if (c == '\r') {
        return 1;
    }

    // Screen wrapping bounds check
    if (_cursorX >= 128) {
        _cursorX = 0;
        _cursorY += 8;
    }
    if (_cursorY >= 64) {
        _cursorY = 0; // Wrap back to the top
    }

    // Map character to font array (Assuming starting at ASCII 0x20 Space)
    if (c >= 0x20 && c <= 0x7E) {
        uint8_t charIndex = c - 0x20;

        // NOTE: this assumes _cursorY is page-aligned (a multiple of 8),
        // which holds for this library since write() only ever advances
        // _cursorY in steps of 8 on '\n'. Each font byte already maps
        // directly onto one vertical 8-pixel page (bit0 = top row).
        uint8_t page = _cursorY / 8;

        // Write the 5 columns of the character to the buffer
        for (uint8_t i = 0; i < 5; i++) {
            // Read pixel data from PROGMEM (Flash memory)
            uint8_t line = pgm_read_byte(&font5x7[charIndex][i]);

            uint16_t bufIndex = (uint16_t)page * 128 + _cursorX + i;
            if (bufIndex < sizeof(_buffer)) {
                _buffer[bufIndex] = line;
            }
        }

        // Blank 1-pixel spacing column between characters
        uint16_t spacingIndex = (uint16_t)page * 128 + _cursorX + 5;
        if (spacingIndex < sizeof(_buffer)) {
            _buffer[spacingIndex] = 0x00;
        }
    }

    _cursorX += 6; // Move cursor forward (5 pixels width + 1 pixel spacing)
    return 1;
}