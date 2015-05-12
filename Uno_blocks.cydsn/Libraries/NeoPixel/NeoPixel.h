#ifndef PSOC_NEOPIXEL_H
#define PSOC_NEOPIXEL_H

#include <Arduino.h>

// 'type' flags for LED pixels (third parameter to constructor):
#define NEO_RGB     0x00 // Wired for RGB data order
#define NEO_GRB     0x01 // Wired for GRB data order
#define NEO_COLMASK 0x01

#define NEO_KHZ400  0x00 // 400 KHz datastream
#define NEO_KHZ800  0x02 // 800 KHz datastream
#define NEO_SPDMASK 0x02

class NeoPixel 
{
 public:

  // Constructor: number of LEDs, pin number, LED type
  NeoPixel(uint16_t n, uint8_t p=0, uint8_t t=NEO_GRB + NEO_KHZ800);

  void begin(void);
  void end(void);
  void show(void);
  void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
  void setPixelColor(uint16_t n, uint32_t c);
  void setBrightness(uint8_t brightness);
  uint16_t numPixels(void);
  static uint32_t Color(uint8_t r, uint8_t g, uint8_t b);
  uint32_t getPixelColor(uint16_t n);

 private:

  uint16_t _numLEDs;   // Number of RGB LEDs in strip
  uint16_t _numBytes;  // Size of 'pixels' buffer below
  uint8_t _pin;       // Output pin number
  uint8_t _type;      // Pixel flags (400 vs 800 KHz, RGB vs GRB color)
  uint8_t _pixels;
  uint8_t _brightness;
};

#endif

