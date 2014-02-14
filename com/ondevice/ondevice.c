/* bricklib
 * Copyright (C) 2010-2012 Olaf Lüke <olaf@tinkerforge.com>
 *
 * none.c: Communication stubs, used when no interface is available
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "ondevice.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "config.h"

#include "extensions/brickd.h"

#include "bricklib/bricklet/bricklet_config.h"
#include "bricklib/com/com.h"
#include "bricklib/com/com_common.h"
#include "bricklib/utility/led.h"
#include "bricklib/utility/init.h"
#include "bricklib/utility/util_definitions.h"

uint8_t counter = 0;
uint16_t status = 0;

uint16_t ondevice_send(const void *data, const uint16_t length, uint32_t *options) {
	status += 20;
	return length;
}

uint16_t ondevice_recv(void *data, const uint16_t length, uint32_t *options) {
	status += 10;
	return length;
}

void ondevice_init_extension(uint8_t extension) {
	ondevice_init();
}

bool ondevice_init() {
	/*xTaskCreate(ondevice_message_loop,
				(signed char *)"ond_ml",
				MESSAGE_LOOP_SIZE,
				NULL,
				1,
				(xTaskHandle *)NULL);
				*/

	status = 5;

	activate_lcd_backlight();

	return true;
}


void ondevice_message_loop(void *parameters) {
	MessageLoopParameter mlp;
	mlp.buffer_size = ONDEVICE_MAX_DATA_LENGTH;
	mlp.com_type    = COM_ONDEVICE;
	mlp.return_func = ondevice_message_loop_return;
	com_message_loop(&mlp);
}

void ondevice_message_loop_return(const char *data, const uint16_t length) {
	com_route_message_from_pc(data, length, COM_ONDEVICE);
}


void send_to_bricklet(const char *data, const uint16_t length) {
	com_route_message_from_pc(data, length, COM_ONDEVICE);
}

void ondevice_tick(const uint8_t tick_type) {

	counter++;

	send_get_baro_chip_temp();

	char info[] = "xxx yyy";
	sprintf(&info[0], "%3d", counter);
	sprintf(&info[3], "%3d", status);


	send_text_to_lcd(0, 0, info);

	SLEEP_MS(500);

}

void activate_lcd_backlight() {

	static MessageHeader lcdblStruct;

	lcdblStruct.uid = base58_decode("gLt");
	lcdblStruct.length = 8;
	lcdblStruct.fid = 3;
	lcdblStruct.sequence_num = 1;
	lcdblStruct.return_expected = 0;
	lcdblStruct.authentication = 0;
	lcdblStruct.other_options = 0;
	lcdblStruct.error = 0;
	lcdblStruct.future_use = 0;

	send_to_bricklet((char*)&lcdblStruct, lcdblStruct.length);
}

void send_text_to_lcd(uint8_t line, uint8_t pos, char * text) {

	static WriteLine lineStruct;

	lineStruct.header.uid = base58_decode("gLt");
	lineStruct.header.length = 30;
	lineStruct.header.fid = 1;
	lineStruct.header.sequence_num = 1;
	lineStruct.header.return_expected = 0;
	lineStruct.header.authentication = 0;
	lineStruct.header.other_options = 0;
	lineStruct.header.error = 0;
	lineStruct.header.future_use = 0;

	lineStruct.line = line;
	lineStruct.position = pos;
	strncpy(lineStruct.text, text, 20);

	send_to_bricklet((char*)&lineStruct, lineStruct.header.length);
}

void send_get_baro_chip_temp() {

	static MessageHeader baroStruct;

	baroStruct.uid = base58_decode("g2j");
	baroStruct.length = 8;
	baroStruct.fid = 14;
	baroStruct.sequence_num = 1;
	baroStruct.return_expected = 1;
	baroStruct.authentication = 0;
	baroStruct.other_options = 0;
	baroStruct.error = 0;
	baroStruct.future_use = 0;

	send_to_bricklet((char*)&baroStruct, baroStruct.length);
}

/*
 *
 * BASE58 ############################
 *
 */

#define BASE58_MAX_STR_SIZE 13

static const char BASE58_ALPHABET[] = "123456789abcdefghijkmnopqrstuvwxyzABCDEFGHJKLMNPQRSTUVWXYZ";

uint32_t base58_decode(const char *str) {
	int i;
	int k;
	uint32_t value = 0;
	uint32_t base = 1;

	for (i = 0; i < BASE58_MAX_STR_SIZE; i++) {
		if (str[i] == '\0') {
			break;
		}
	}

	--i;

	for (; i >= 0; i--) {
		if (str[i] == '\0') {
			continue;
		}

		for (k = 0; k < 58; k++) {
			if (BASE58_ALPHABET[k] == str[i]) {
				break;
			}
		}

		value += k * base;
		base *= 58;
	}

	return value;
}
