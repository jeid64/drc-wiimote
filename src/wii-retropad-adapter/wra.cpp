/*
 * Wii RetroPad Adapter - Nintendo Wiimote adapter for retro-controllers!
 * Copyright (c) 2011 Bruno Freitas - bootsector@ig.com.br
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <WProgram.h>
#include <HardwareSerial.h>

#include "WMExtension.h"

// Classic Controller Buttons
int bdl = 0; // D-Pad Left state
int bdr = 0; // D-Pad Right state
int bdu = 0; // D-Pad Up state
int bdd = 0; // D-Pad Down state
int ba = 0; // A button state
int bb = 0; // B button state
int bx = 0; // X button state
int by = 0; // Y button state
int bl = 0; // L button state
int br = 0; // R button state
int bm = 0; // MINUS button state
int bp = 0; // PLUS button state
int bhome = 0; // HOME button state
int bzl = 0; // ZL button state
int bzr = 0; // ZR button state
int lt = 0; // L analog value
int rt = 0; // R analog value

// Analog Buttons
byte lx = WMExtension::get_calibration_byte(2)>>2;
byte ly = WMExtension::get_calibration_byte(5)>>2;
byte rx = WMExtension::get_calibration_byte(8)>>3;
byte ry = WMExtension::get_calibration_byte(11)>>3;

// Analog stick neutral radius
#define ANALOG_NEUTRAL_RADIUS 10

// Extension cable detection pins
#define DETPIN0 3  // DB9P2
#define DETPIN1	5  // DB9P4
#define DETPIN2	6  // DB9P6
#define DETPIN3	7  // DB9P7
#define DETPIN4	8  // DB9P9

// Possible values (as of today) returned by the detectPad() routine
// Normal pads
#define PAD_ARCADE		-1
#define PAD_GENESIS		0b00111
#define PAD_NES 		0b00110
#define PAD_SNES 		0b00101
#define PAD_PS2 		0b00100
#define PAD_GC	 		0b00011
#define PAD_N64			0b00010
#define PAD_NEOGEO		0b00001
#define PAD_WIICC		0b00000
// Extended pads (uses DB9 pin 4 and/or 2 for identification)
#define PAD_SATURN		0b01111
#define PAD_TG16		0b10111
#define PAD_DFU_DONGLE	0b01110 // Reserved for USBRA DFU dongle

/*
 * This is the new auto-detect function (non jumper based) which detects the extension
 * cable plugged in the DB9 port. It uses grounded pins from DB9 (4, 6, 7 and 9) for
 * the detection.
 *
 *  -1 - Arcade
 * 00111 - Sega Genesis (Default)
 * 00110 - NES
 * 00101 - SNES
 * 00100 - PS2
 * 00011 - Game Cube
 * 00010 - Nintendo 64
 * 00001 - Neo Geo
 * 00000 - Reserved 1
 * 01111 - Sega Saturn
 * 10111 - TurboGrafx 16
 */
int detectPad() {
	int pad;

	// Set pad/arcade detection pins as input, turning pull-ups on
	pinMode(DETPIN0, INPUT);
	digitalWrite(DETPIN0, HIGH);

	pinMode(DETPIN1, INPUT);
	digitalWrite(DETPIN1, HIGH);

	pinMode(DETPIN2, INPUT);
	digitalWrite(DETPIN2, HIGH);

	pinMode(DETPIN3, INPUT);
	digitalWrite(DETPIN3, HIGH);

	pinMode(DETPIN4, INPUT);
	digitalWrite(DETPIN4, HIGH);

	pad = (!digitalRead(DETPIN0) << 4) | (!digitalRead(DETPIN1) << 3) | (digitalRead(DETPIN2) << 2) | (digitalRead(DETPIN3) << 1) | (digitalRead(DETPIN4));

	if((pad >> 3) & 0b11) {
		switch(pad) {
		case 0b11011:
		case 0b10111:
			return PAD_TG16;
			break;
		case 0b11111:
		case 0b01111:
			return PAD_SATURN;
			break;
		case 0b11100:
			return PAD_PS2;
			break;
		default:
			return PAD_GENESIS;
			break;
		}
	}

	return (pad & 0b111);
}

void setup() {
	// Prepare wiimote communications
  Serial.begin(9600);           // start serial for output
	WMExtension::init();
}

void fake_loop() {
	int incomingByte = 0;
	for (;;) {
		if (Serial.available() > 0) {
			// read the incoming byte:
			incomingByte = Serial.read();
			Serial.write(incomingByte);
			switch (incomingByte)
			{
				case ',':
					ba = 1;
					break;
				case '.':
					bb = 1;
					break;
				case 'w':
					bdu = 1;
					bdr = bdl = bdd = 0;
					break;
				case 'a':
					bdl = 1;
					bdr = bdu = bdd = 0;
					break;
				case 's':
					bdd = 1;
					bdr = bdu = bdl = 0;
					break;
				case 'd':
					bdr = 1;
					bdl = bdu = bdd = 0;
					break;
				case 'q':
					bzl = !bzl;
					break;
				case 'e':
					bzr = 1;
					break;
				case ' ':
					bzl = bzr = ba = bb = bdu = bdr = 0;
					break;
			}
			//ba = incomingByte;
			WMExtension::set_button_data(bdl, bdr, bdu, bdd, ba, bb, bx, by, bl, br,
					bm, bp, bhome, lx, ly, rx, ry, bzl, bzr, lt, rt);
			delay(25);
			//bzr = 1;
			//ba = 0;
			WMExtension::set_button_data(bdl, bdr, bdu, bdd, ba, bb, bx, by, bl, br,
					bm, bp, bhome, lx, ly, rx, ry, bzl, bzr, lt, rt);
		}
	}
}

void loop() {
	fake_loop();
}
