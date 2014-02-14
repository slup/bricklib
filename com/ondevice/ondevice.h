#ifndef ONDEVICE_H
#define ONDEVICE_H

#include <stdint.h>
#include <stdbool.h>
#include "bricklib/com/com_common.h"

#define ONDEVICE_MAX_DATA_LENGTH 80

typedef struct {
	MessageHeader header;
	uint8_t line;
	uint8_t position;
	char text[20];
} __attribute__((__packed__)) WriteLine;

uint16_t ondevice_send(const void *data, const uint16_t length, uint32_t *options);
uint16_t ondevice_recv(void *data, const uint16_t length, uint32_t *options);

void ondevice_init_extension(uint8_t extension);
bool ondevice_init(void);

void ondevice_message_loop(void *parameters);
void ondevice_message_loop_return(const char *data, const uint16_t length);

void ondevice_tick(const uint8_t tick_type);

void send_to_bricklet(const char *data, const uint16_t length);

void activate_lcd_backlight();
void send_text_to_lcd(uint8_t line, uint8_t pos, char * text);
void send_get_baro_chip_temp();


uint32_t base58_decode(const char *str);

#endif
