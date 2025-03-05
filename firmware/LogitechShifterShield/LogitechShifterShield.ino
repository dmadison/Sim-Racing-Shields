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
 * @details Reads from a Logitech shifter connected to the shifter shield
 *          and sends its data as a USB joystick.
 * @example LogitechShifterShield.ino
 */

 // This firmware requires the Arduino Joystick Library
 // Download Here: https://github.com/MHeironimus/ArduinoJoystickLibrary/releases/latest

#include <SimRacing.h>
#include <Joystick.h>

// Option to compile with support for the Logitech Driving Force shifter
// 
// The Driving Force shifter is included with the G923, G920, and G29 wheels
// Includes: 7 gears
#define USE_SHIFTER_DRIVING_FORCE

// Option to compile with support for the Logitech G27 shifter
// 
// Includes: 7 gears, 8 buttons, hat switch
#define USE_SHIFTER_G27

// Option to compile with support for the Logitech G25 shifter
// 
// Includes: 7 gears, 8 buttons, hat switch, 2 sequential shift buttons
#define USE_SHIFTER_G25

// Option to send the analog axis data as joystick axes.
// 
// NOT needed for any standard games, but can be useful for custom
// controller purposes
//#define SEND_ANALOG_AXES

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

// -----------------------------------

#if !defined(USE_SHIFTER_DRIVING_FORCE) && !defined(USE_SHIFTER_G27) && !defined(USE_SHIFTER_G25)
#error "You must define at least 1 shifter type"
#endif


// Gear definitions. Change this array to change the gear to USB
// button assignments
const int Gears[] = { 1, 2, 3, 4, 5, 6, -1 };
const int NumGears = sizeof(Gears) / sizeof(Gears[0]);

// Button definitions. Change this array to change the shifter button
// to USB button assignments
using ShifterButton = SimRacing::LogitechShifterG25::Button;
const ShifterButton Buttons[] = {
	ShifterButton::BUTTON_SOUTH,
	ShifterButton::BUTTON_EAST,
	ShifterButton::BUTTON_WEST,
	ShifterButton::BUTTON_NORTH,
	ShifterButton::BUTTON_1,
	ShifterButton::BUTTON_2,
	ShifterButton::BUTTON_3,
	ShifterButton::BUTTON_4,
};
#if defined(USE_SHIFTER_G27) || defined(USE_SHIFTER_G25)
const int NumButtons = sizeof(Buttons) / sizeof(Buttons[0]);
#else
const int NumButtons = 0;
#endif


#if defined(USE_SHIFTER_G25)
// 7 gears, 8 buttons, 2 sequential inputs
const int NumJoystickButtons = NumGears + NumButtons + 2;
const int NumHatSwitches = 1;

#elif defined(USE_SHIFTER_G27)
// 7 gears, 8 buttons
const int NumJoystickButtons = NumGears + NumButtons;
const int NumHatSwitches = 1;

#elif defined(USE_SHIFTER_DRIVING_FORCE)
// 7 gears, no buttons
const int NumJoystickButtons = NumGears;
const int NumHatSwitches = 0;

#else
// nothing D:
// (fixes compilation issues so that the preprocessor #error shows first)
const int NumJoystickButtons = 0;
const int NumHatSwitches = 0;

#endif

#if defined(SEND_ANALOG_AXES)
const bool SendAnalogAxes = true;
#else
const bool SendAnalogAxes = false;
#endif


Joystick_ Joystick(
	JOYSTICK_DEFAULT_REPORT_ID,      // default report (no additional pages)
	JOYSTICK_TYPE_JOYSTICK,          // so that this shows up in Windows joystick manager
	NumJoystickButtons,              // number of buttons
	NumHatSwitches,                  // number of hat switches
	SendAnalogAxes, SendAnalogAxes,  // include X and Y axes for analog output, if enabled
	false, false, false, false, false, false, false, false, false);  // no other axes

const int ADC_Max = 1023;  // 10-bit on AVR

struct JoystickData {
	int gear = 0;

	#if defined(USE_SHIFTER_G27) || defined(USE_SHIFTER_G25)
	int      dpadAngle = -1;
	uint32_t buttons   = 0x00;
	#endif

	#if defined(USE_SHIFTER_G25)
	bool     seqUp     = 0;
	bool     seqDown   = 0;
	#endif

	#if defined(SEND_ANALOG_AXES)
	int x = ADC_Max / 2;
	int y = ADC_Max / 2;
	#endif

	bool operator == (const JoystickData& rhs) const {
		if (this->gear != rhs.gear) return false;

		#if defined(USE_SHIFTER_G27) || defined(USE_SHIFTER_G25)
		if (this->dpadAngle != rhs.dpadAngle) return false;
		if (this->buttons   != rhs.buttons)   return false;
		#endif

		#if defined(USE_SHIFTER_G25)
		if (this->seqUp     != rhs.seqUp)     return false;
		if (this->seqDown   != rhs.seqDown)   return false;
		#endif

		#if defined(SEND_ANALOG_AXES)
		if (this->x != rhs.x) return false;
		if (this->y != rhs.y) return false;
		#endif

		return true;
	}
};


// forward-declared functions for non-Arduino environments
void parseShifter(SimRacing::LogitechShifter& shifter,    JoystickData& data);
void parseShifter(SimRacing::LogitechShifterG27& shifter, JoystickData& data);
void parseShifter(SimRacing::LogitechShifterG25& shifter, JoystickData& data);

void updateUSB(const JoystickData& data);


/**
* Parse the joystick data from the Logitech Driving Force shifter
*
* @param shifter The Logitech shifter to read from
* @param data Reference to a joystick data struct to store the data into
*/
void parseShifter(SimRacing::LogitechShifter& shifter, JoystickData& data) {
	shifter.update();

	if (shifter.isConnected()) {
		data.gear = shifter.getGear();

		#if defined(SEND_ANALOG_AXES)
		data.x = shifter.getPosition(SimRacing::X, 0, ADC_Max);
		data.y = shifter.getPosition(SimRacing::Y, 0, ADC_Max);
		#endif
	}
}


/**
* Parse the joystick data from the Logitech G27 shifter
*
* @param shifter The Logitech G27 shifter to read from
* @param data Reference to a joystick data struct to store the data into
*/
void parseShifter(SimRacing::LogitechShifterG27& shifter, JoystickData& data) {
	shifter.update();

	if (shifter.isConnected()) {
		data.gear = shifter.getGear();

		#if defined(USE_SHIFTER_G27)
		for (int i = 0; i < NumButtons; ++i) {
			const bool state = shifter.getButton(Buttons[i]);
			data.buttons |= state << i;
		}

		data.dpadAngle = shifter.getDpadAngle();
		#endif

		#if defined(SEND_ANALOG_AXES)
		data.x = shifter.getPosition(SimRacing::X, 0, ADC_Max);
		data.y = shifter.getPosition(SimRacing::Y, 0, ADC_Max);
		#endif
	}
}


/**
* Parse the joystick data from the Logitech G25 shifter
*
* @param shifter The Logitech G25 shifter to read from
* @param data Reference to a joystick data struct to store the data into
*/
void parseShifter(SimRacing::LogitechShifterG25& shifter, JoystickData& data) {
	shifter.update();

	if (shifter.isConnected()) {
		data.gear = shifter.getGear();

		#if defined(USE_SHIFTER_G25)
		for (int i = 0; i < NumButtons; ++i) {
			const bool state = shifter.getButton(Buttons[i]);
			data.buttons |= state << i;
		}

		data.dpadAngle = shifter.getDpadAngle();

		data.seqUp   = shifter.getShiftUp();
		data.seqDown = shifter.getShiftDown();
		#endif

		#if defined(SEND_ANALOG_AXES)
		data.x = shifter.getPosition(SimRacing::X, 0, ADC_Max);
		data.y = shifter.getPosition(SimRacing::Y, 0, ADC_Max);
		#endif
	}
}


/**
* Read the joystick data and set the USB output
* 
* @param data Reference to the joystick data struct to read from
*/
void updateUSB(const JoystickData& data) {
	static JoystickData previousData;

	#if !defined(ALWAYS_SEND_DATA)
	// if data hasn't changed, don't send an update
	if (data == previousData) return;
	#endif

	uint8_t currentButton = 0;

	// set the buttons corresponding to the gears
	for (int i = 0; i < NumGears; ++i) {
		if (data.gear == Gears[i]) {
			Joystick.pressButton(currentButton);
		}
		else {
			Joystick.releaseButton(currentButton);
		}

		currentButton++;
	}

	#if defined(USE_SHIFTER_G27) || defined(USE_SHIFTER_G25)
	// set the hat switch (directional pad)
	Joystick.setHatSwitch(0, data.dpadAngle);

	// set the buttons from the bitfield
	for (int i = 0; i < NumButtons; ++i) {
		const bool state = data.buttons & (1 << i);
		Joystick.setButton(currentButton, state);
		currentButton++;
	}
	#endif

	#if defined(USE_SHIFTER_G25)
	// set the sequential buttons
	Joystick.setButton(currentButton++, data.seqUp);
	Joystick.setButton(currentButton++, data.seqDown);
	#endif

	#if defined(SEND_ANALOG_AXES)
	// set the analog axes
	Joystick.setXAxis(data.x);
	Joystick.setYAxis(data.y);
	#endif

	// send the USB update
	Joystick.sendState();

	previousData = data;
}

void setup() {
	#if defined(DISABLE_USB_LEDS)
	pinMode(30, INPUT);  // TX LED
	pinMode(17, INPUT);  // RX LED
	#endif

	Joystick.begin(false);  // 'false' to disable auto-send
	Joystick.setXAxisRange(0, ADC_Max);
	Joystick.setYAxisRange(ADC_Max, 0);  // invert axis so 'up' is up

	// Define the Driving Force (G923 / G920 / G29) instance
	// Since the G27 uses the same detection pin, only create the simpler Driving
	// Force instance if we're not supporting the G27 as well
	#if defined(USE_SHIFTER_DRIVING_FORCE) && !defined(USE_SHIFTER_G27)
		SimRacing::LogitechShifter df = SimRacing::CreateShieldObject<SimRacing::LogitechShifter, 2>();
		// if you have one, your calibration line should go here
		df.begin();
	#endif  // USE_SHIFTER_DRIVING_FORCE

	// Define the G27 instance
	#if defined(USE_SHIFTER_G27)
		SimRacing::LogitechShifterG27 g27 = SimRacing::CreateShieldObject<SimRacing::LogitechShifterG27, 2>();
		// if you have one, your calibration line should go here
		g27.begin();
	#endif  // USE_SHIFTER_G27

	// Define the G25 instance
	#if defined(USE_SHIFTER_G25)
		SimRacing::LogitechShifterG25 g25 = SimRacing::CreateShieldObject<SimRacing::LogitechShifterG25, 2>();
		// if you have one, your calibration line should go here
		g25.begin();
	#endif

	while (true) {
		JoystickData data;

		// Logitech Driving Force shifter processing
		#if defined(USE_SHIFTER_DRIVING_FORCE) && !defined(USE_SHIFTER_G27)
		parseShifter(df, data);
		#endif

		// Logitech G27 shifter processing
		#if defined(USE_SHIFTER_G27)
		parseShifter(g27, data);
		#endif

		// Logitech G25 shifter processing
		#if defined(USE_SHIFTER_G25)
		parseShifter(g25, data);
		#endif

		updateUSB(data);
	}
}

void loop() {}  // intentionally empty
