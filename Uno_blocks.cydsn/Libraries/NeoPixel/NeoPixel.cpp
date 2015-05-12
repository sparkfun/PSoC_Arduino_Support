#include <Arduino.h>
#include <project.h>
#include "NeoPixel.h"

NeoPixel::NeoPixel(uint16_t n, uint8_t p, uint8_t t):
  _numLEDs(n), _pin(p), _type(t)
{
  _pixels = new uint8_t [_numLEDs];
}

void NeoPixel::begin()
{
  pinMode(_pin + P0_D0, PERIPHERAL_OUT);
  WSDriver_Start();
  for (int i = 0; i < _numLEDs; i++)
  {
    WSDriver_Pixel(i, _pin, 0);
  }
  WSDriver_Dim(4);
}

void NeoPixel::end()
{
  WSDriver_Stop();
  pinMode(_pin + P0_D0, INPUT);
}

void NeoPixel::show()
{
  uint32_t c;

  for (uint16_t i = 0; i < _numLEDs; i++)
  {
    uint16_t c0 = *(_pixels+(i*3));
    uint16_t c1 = *(_pixels+(i*3)+1);
    uint16_t c2 = *(_pixels+(i*3)+2);
    if (_brightness == 0)
    {
      c = 0;
    }
    else if (_brightness == 255)
    {
      // We don't actually need to do anything here, just catch that we don't
      //  need to go on to the expensive math part up ahead.
      if ( (_type & NEO_COLMASK) == NEO_RGB)
      {
        c = ((c0&0xff00)>>8) + (c1&0xff00) + ((c2&0xff00)<<8);
      }
      else 
      {
        c = ((c0&0xff00)>>8) + (c2&0xff00) + ((c1&0xff00)<<8);
      }
    }
    else
    {
      c0 *= _brightness;
      c1 *= _brightness;
      c2 *= _brightness;
      if ( (_type & NEO_COLMASK) == NEO_RGB)
      {
        c = ((c0&0xff00)>>8) + (c1&0xff00) + ((c2&0xff00)<<8);
      }
      else 
      {
        c = ((c0&0xff00)>>8) + (c2&0xff00) + ((c1&0xff00)<<8);
      }
    }
    WSDriver_Pixel(i, _pin, c);
  }
  WSDriver_Trigger(1);
}

void NeoPixel::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b)
{
  *(_pixels+(n*3)) = b;
  *(_pixels+(n*3)+1) = g;
  *(_pixels+(n*3)+2) = r;
}

void NeoPixel::setPixelColor(uint16_t n, uint32_t c)
{
  *(_pixels+(n*3)) = c;
  *(_pixels+(n*3)+1) = c>>8;
  *(_pixels+(n*3)+2) = c>>16;
}

void NeoPixel::setBrightness(uint8_t brightness)
{
  _brightness = brightness;
}

uint16_t NeoPixel::numPixels(void)
{
  return _numLEDs;
}

uint32_t NeoPixel::Color(uint8_t r, uint8_t g, uint8_t b)
{
  return (r<<16) + (g<<8) + b;
}

uint32_t NeoPixel::getPixelColor(uint16_t n)
{
  uint32_t color;
  color = *(_pixels+(n*3));
  color += *(_pixels+(n*3)+1)<<8;
  color += *(_pixels+(n*3)+2)<<16;
  return color;
}

