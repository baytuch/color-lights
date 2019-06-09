
#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>
#include "WS2812.h"


#define STRIP_LEN 30
#define STRIP_LIGHT 30

struct LED strip[STRIP_LEN];

void strip_run(struct LED *data) {
  uint8_t i = STRIP_LEN;
  while (i > 0) {
    i--;
    data[i + 1].r = data[i].r;
    data[i + 1].g = data[i].g;
    data[i + 1].b = data[i].b;
  } 
  data[0].r = (uint8_t) rand() % STRIP_LIGHT;
  data[0].g = (uint8_t) rand() % STRIP_LIGHT;
  data[0].b = (uint8_t) rand() % STRIP_LIGHT;
}

static struct LED led_smooth_target;
void strip_run_smooth(struct LED *data) {
  uint8_t i = STRIP_LEN;
  while (i > 0) {
    i--;
    data[i + 1].r = data[i].r;
    data[i + 1].g = data[i].g;
    data[i + 1].b = data[i].b;
  }
  if (data[0].r > led_smooth_target.r) data[0].r--;
  if (data[0].r < led_smooth_target.r) data[0].r++;
  if (data[0].g > led_smooth_target.g) data[0].g--;
  if (data[0].g < led_smooth_target.g) data[0].g++;
  if (data[0].b > led_smooth_target.b) data[0].b--;
  if (data[0].b < led_smooth_target.b) data[0].b++;
  if (data[0].r == led_smooth_target.r && data[0].g == led_smooth_target.g && data[0].b == led_smooth_target.b) {
    led_smooth_target.r = (uint8_t) rand() % STRIP_LIGHT;
    led_smooth_target.g = (uint8_t) rand() % STRIP_LIGHT;
    led_smooth_target.b = (uint8_t) rand() % STRIP_LIGHT;
  }
}

static uint8_t strip_police_n;
void strip_police(struct LED *data) {
  for (uint8_t i = 0; i < STRIP_LEN; i++) {
    if (i <= STRIP_LEN / 2 - 3 && (strip_police_n == 0 || strip_police_n == 2)) {
      strip[i].r = STRIP_LIGHT;
      strip[i].g = 0x00;
      strip[i].b = 0x00;
    } else if (i >= STRIP_LEN / 2 + 2 && (strip_police_n == 0 || strip_police_n == 2)) {
      strip[i].r = 0x00;
      strip[i].g = 0x00;
      strip[i].b = STRIP_LIGHT;
    } else {
      strip[i].r = 0x00;
      strip[i].g = 0x00;
      strip[i].b = 0x00;
    }
  }
  strip_police_n++;
  if (strip_police_n > 10) strip_police_n = 0;
}

static uint8_t strip_police_n;
void strip_police_dobs(struct LED *data) {
  for (uint8_t i = 0; i < STRIP_LEN; i++) {
    if (i <= STRIP_LEN / 2 - 3) {
      if (strip_police_n == i) {
        strip[i].r = STRIP_LIGHT;
      } else {
        strip[i].r = 0x00;
      }
      strip[i].g = 0x00;
      strip[i].b = 0x00;
    } else if (i >= STRIP_LEN / 2 + 2) {
      if (strip_police_n == STRIP_LEN - i) {
        strip[i].b = STRIP_LIGHT;
      } else {
        strip[i].b = 0x00;
      }
      strip[i].g = 0x00;
    } else {
      strip[i].r = 0x00;
      strip[i].g = 0x00;
      strip[i].b = 0x00;
    }
  }
  strip_police_n++;
  if (strip_police_n > STRIP_LEN / 2 - 3) strip_police_n = 0;
}  

int main(void) {

  ws2812_setup();

  for (uint8_t i = 0; i < STRIP_LEN; i++) {
    strip[i].r = 0x00;
    strip[i].g = 0x00;
    strip[i].b = 0x00;
  }

  while(1){

    strip_run_smooth(strip);

    ws2812_send(strip, STRIP_LEN);

    ws2812_wait();
  }

  return 0;
}
