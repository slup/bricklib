#ifndef ONDEVICE_H
#define ONDEVICE_H

#include <stdint.h>
#include <stdbool.h>

uint16_t ondevice_send(const void *data, const uint16_t length, uint32_t *options);
uint16_t ondevice_recv(void *data, const uint16_t length, uint32_t *options);
bool ondevice_init(void);

void send_to_bricklet(const char *data, const uint16_t length);

void ondevice_tick(const uint8_t tick_type);

void activate_lcd_backlight();
void send_text_to_lcd();

#endif
