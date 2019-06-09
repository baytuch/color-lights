
#include <stdint.h>

#ifndef WS2812_H_INCLUDED
#define WS2812_H_INCLUDED

struct LED {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

void ws2812_setup();
void ws2812_send(const struct LED *data, uint8_t len);
void ws2812_wait();

#endif // WS2812_H_INCLUDED
