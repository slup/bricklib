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

#include <stdint.h>
#include <stdbool.h>

#include "config.h"
#include "bricklib/utility/led.h"
#include "bricklib/utility/init.h"

#include "bricklib/utility/util_definitions.h"

#define BLINK_DELAY 40

uint8_t toggle_led = 0;

uint16_t ondevice_send(const void *data, const uint16_t length, uint32_t *options) {
	return length;
}

uint16_t ondevice_recv(void *data, const uint16_t length, uint32_t *options) {
	/*led_blink(LED_EXT_BLUE_0, BLINK_DELAY);
	led_blink(LED_EXT_BLUE_3, BLINK_DELAY);*/
	return length;
}

bool ondevice_init(void) {
	return true;
}

void send_to_bricklet(const char *data, const uint16_t length) {
	com_route_message_from_pc(data, length, COM_ONDEVICE);
}

void ondevice_tick(const uint8_t tick_type) {

	if (toggle_led == 0) {
		toggle_led = 1;
		led_off(LED_EXT_BLUE_0);
		led_on(LED_EXT_BLUE_1);
		led_on(LED_EXT_BLUE_2);
		led_off(LED_EXT_BLUE_3);
	} else {
		toggle_led = 0;
		led_on(LED_EXT_BLUE_0);
		led_off(LED_EXT_BLUE_1);
		led_off(LED_EXT_BLUE_2);
		led_on(LED_EXT_BLUE_3);
	}

	activate_lcd_backlight();

	send_text_to_lcd();

	SLEEP_MS(500);

}

void activate_lcd_backlight() {
	/*
	 * UID: gLt -> 53039 ->
	 */
	char data[8];

	/* set UID (4 byte)*/
	data[0] = 0x2f;
	data[1] = 0xcf;
	data[2] = 0x0;
	data[3] = 0x0;

	/* set length (1 byte)*/
	data[4] = 0x08;

	/* set function id (1 byte)*/
	/* BrickletLCD20x4.write_line = 1 */
	data[5] = 0x03;

	/* set sequence number & options (1 byte)*/
	data[6] = 0x10;

	/* set flags (1 byte)*/
	data[7] = 0x0;

	send_to_bricklet(data, 8);

}

void send_text_to_lcd() {
	/*
	 * UID: gLt -> 53039 ->
	 */
	char data[30];

	/* set UID (4 byte)*/
	data[0] = 0x2f;
	data[1] = 0xcf;
	data[2] = 0x0;
	data[3] = 0x0;

	/* set length (1 byte)*/
	data[4] = 0x1e;

	/* set function id (1 byte)*/
	/* BrickletLCD20x4.write_line = 1 */
	data[5] = 0x01;

	/* set sequence number & options (1 byte)*/
	data[6] = 0x10;

	/* set flags (1 byte)*/
	data[7] = 0x0;

	/* set line (1 byte) */
	data[8] = 0x0;

	/* set pos (1 byte) */
	data[9] = 0x0;

	/* set text (20 byte) */
	data[10] = 0x48; /*H*/
	data[11] = 0x65; /*e*/
	data[12] = 0x6c; /*l*/
	data[13] = 0x6c; /*l*/
	data[14] = 0x6f; /*o*/
	data[15] = 0x3a; /*:*/
	data[16] = 0x20; /* */
	data[17] = 48 + toggle_led;
	data[18] = 0x0;
	data[19] = 0x0;
	data[20] = 0x0;
	data[21] = 0x0;
	data[22] = 0x0;
	data[23] = 0x0;
	data[24] = 0x0;
	data[25] = 0x0;
	data[26] = 0x0;
	data[27] = 0x0;
	data[28] = 0x0;
	data[29] = 0x0;

	send_to_bricklet(data, 30);
}
