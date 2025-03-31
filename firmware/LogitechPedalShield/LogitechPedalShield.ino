/*
 *  Project     Sim Racing Shields for Arduino
 *  @author     David Madison
 *  @link       github.com/dmadison/Sim-Racing-Shields
 *  @license    GPLv3 - Copyright (c) 2025 David Madison
 *
 *  This file is part of the Sim Racing Shields for Arduino project.
 *
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the Free
 *  Software Foundation, either version 3 of the License, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU General Public License along with
 *  this program.  If not, see <http://www.gnu.org/licenses/>.
 */

 /**
 * @details Reads from a set of Logitech pedals connected to the pedal shield
 *          and sends its data as a USB joystick.
 * @example LogitechPedalShield.ino
 */

 // This firmware requires the Arduino Joystick Library
 // Download Here: https://github.com/MHeironimus/ArduinoJoystickLibrary/releases/latest

#include <SimRacing.h>
#include <Joystick.h>

// Option to always send USB data, regardless of whether it has changed
// 
// This shouldn't be necessary, but if you're having issues with missed
// packets for some reason this could be useful
//#define ALWAYS_SEND_DATA

// Option to disable the USB TX / RX LEDs
// 
// These LEDs are useful to indicate that the device is working as intended,
// but can be disabled to make the device more stealthy
#define DISABLE_USB_LEDS

SimRacing::LogitechPedals pedals = SimRacing::CreateShieldObject<SimRacing::LogitechPedals, 2>();

Joystick_ Joystick(
	JOYSTICK_DEFAULT_REPORT_ID,          // default report (no additional pages)
	JOYSTICK_TYPE_JOYSTICK,              // so that this shows up in Windows joystick manager
	0,                                   // number of buttons (none)
	0,                                   // number of hat switches (none)
	false, false,                        // no X and Y axes
	pedals.hasPedal(SimRacing::Clutch),  // include Z axis for the clutch pedal
	pedals.hasPedal(SimRacing::Brake),   // include Rx axis for the brake pedal
	pedals.hasPedal(SimRacing::Gas),     // include Ry axis for the gas pedal
	false, false, false, false, false, false);  // no other axes

const int ADC_Max = 1023;  // max value of the analog inputs, 10-bit on AVR boards

#if defined(ALWAYS_SEND_DATA)
const bool AlwaysSend = true;
#else
const bool AlwaysSend = false;
#endif


void setup() {
	#if defined(DISABLE_USB_LEDS)
	pinMode(30, INPUT);  // TX LED
	pinMode(17, INPUT);  // RX LED
	#endif

	pedals.begin();  // initialize pedal pins

	// if you have one, your calibration line should go here
	
	Joystick.begin(false);  // 'false' to disable auto-send

	Joystick.setZAxisRange(0, ADC_Max);
	Joystick.setRxAxisRange(0, ADC_Max);
	Joystick.setRyAxisRange(0, ADC_Max);

	updateJoystick();  // send initial state
}

void loop() {
	pedals.update();

	if (pedals.positionChanged() || AlwaysSend) {
		updateJoystick();
	}
}

void updateJoystick() {
	if (pedals.hasPedal(SimRacing::Gas)) {
		int gasPedal = pedals.getPosition(SimRacing::Gas, 0, ADC_Max);
		Joystick.setRyAxis(gasPedal);
	}

	if (pedals.hasPedal(SimRacing::Brake)) {
		int brakePedal = pedals.getPosition(SimRacing::Brake, 0, ADC_Max);
		Joystick.setRxAxis(brakePedal);
	}

	if (pedals.hasPedal(SimRacing::Clutch)) {
		int clutchPedal = pedals.getPosition(SimRacing::Clutch, 0, ADC_Max);
		Joystick.setZAxis(clutchPedal);
	}

	Joystick.sendState();
}
