# Sim Racing Shields for Arduino

![Sim Racing Shields Header](../master/images/Sim-Racing-Shields_Header.jpg)

This repo contains add-on circuit boards ("shields") for connecting Arduino development boards to sim racing equipment. These shields are meant to be used in conjunction with the [Sim Racing Library for Arduino](https://github.com/dmadison/Sim-Racing-Arduino). These shields can be used to create inexpensive, standalone USB HID adapters for use with racing games and simulators.

Both of these shields are designed to be used with a [SparkFun Pro Micro, 5V (32U4)](https://github.com/sparkfun/Pro_Micro). You can use either genuine SparkFun boards or 3rd party clones. The [SparkFun Qwiic Pro Micro with a USB-C port (DEV-15795)](https://www.sparkfun.com/products/15795) is recommended. The SparkFun Pro Micro RP2040 is theoretically compatible but has not been tested. Note that the Tag-Connect programming header is not compatible with the RP2040.

The shields can be assembled with either 0.1″ female headers for prototyping or 0.1″ male headers for a permanent connection. Only assembled boards with male headers will be able to fit inside the 3D printed case.

For more information, [see the article on PartsNotIncluded.com](https://www.partsnotincluded.com/sim-racing-shields-for-arduino/).

## Logitech Shifter Shield for Pro Micro

The [Logitech Shifter Shield](pcbs/Logitech_Shifter_Shield_Pro_Micro) is designed to interface with all available Logitech shifters using a single DB-9 male connector:
* [Logitech Driving Force shifter (G923 / G920 / G29)](https://www.logitechg.com/en-us/products/driving/driving-force-shifter.html)
* [Logitech G27 shifter](https://en.wikipedia.org/wiki/Logitech_G27)
* [Logitech G25 shifter](https://en.wikipedia.org/wiki/Logitech_G25)

This repo includes firmware ([LogitechShifterShield](firmware/LogitechShifterShield/LogitechShifterShield.ino)) which works out of the box as a USB HID adapter for all three shifter types.

You can also write your own firmware using the [Sim Racing Library for Arduino](https://github.com/dmadison/Sim-Racing-Arduino). Use the `SimRacing::CreateShieldObject()` template function for easy setup with the shifter shield:

```cpp
LogitechShifterG29 shifter = SimRacing::CreateShieldObject<SimRacing::LogitechShifterG29, 2>();
LogitechShifterG27 shifter = SimRacing::CreateShieldObject<SimRacing::LogitechShifterG27, 2>();
LogitechShifterG25 shifter = SimRacing::CreateShieldObject<SimRacing::LogitechShifterG25, 2>();
```

## Logitech Pedals Shield for Pro Micro

The [Logitech Pedal Shield](pcbs/Logitech_Pedal_Shield_Pro_Micro) is designed to interface with the Logitech three pedal attachment, included with the Logitech G923 “TRUEFORCE”, G29/G920 “Driving Force”, and G27/G25 wheels, via a single DB-9 female connector.

This repo includes firmware ([LogitechPedalsShield](firmware/LogitechPedalsShield/LogitechPedalsShield.ino)) which works out of the box as a USB HID adapter for the pedals.

You can also write your own firmware using the [Sim Racing Library for Arduino](https://github.com/dmadison/Sim-Racing-Arduino). Use the `SimRacing::CreateShieldObject()` template function for easy setup with the pedals shield:

```cpp
LogitechPedals pedals = SimRacing::CreateShieldObject<SimRacing::LogitechPedals, 2>();
```

## Firmware

The shields are designed to be used with the [Sim Racing Library for Arduino](https://github.com/dmadison/Sim-Racing-Arduino). You can use any included example for your specific hardware, or write your own using the data the library provides.

Both shields use the SparkFun Pro Micro (32U4), and firmware should be compiled using the "SparkFun Pro Micro" board in the Arduino IDE. You can find the compilation files and installation instructions for the Pro Micro [here](https://github.com/sparkfun/Arduino_Boards). You do not need to install the SparkFun boards package if you are using the custom boards package below.

### Custom Boards Installation

To install the custom boards package, start up the Arduino IDE and open the preferences window (**File > Preferences**). Copy and paste this URL for the package index into the "Additional Boards Manager URLs" field:

```
https://github.com/dmadison/Sim-Racing-Shields/raw/master/arduino-boards/boards-manager/package_simracing_index.json
```

In the menu, navigate to **Tools > Board > Boards Manager** and open up the boards manager. Search for "Sim Racing" and then install the "Sim Racing AVR Boards" package. After the package is installed you should be able to select the "SparkFun Pro Micro (for Sim Racing)" board.

### Custom Boards Configuration

The custom boards package adds two new options to the boards menu:

* USB Identity
* Serial Port

The first option, 'USB Identity', changes the USB PID and USB description for the board. Firmware compiled with the 'Shifter' option will present as "Sim Racing Shifter" with a PID of 0x9101, while firmware compiled with the 'Pedals' option will present as "Sim Racing Pedals" with a PID of 0x9102.

The second option, 'Serial Port', allows you to disable the USB CDC serial port. This improves compatibility with some systems and prevents accidentally reprogramming the board. Note that if you remove the USB CDC serial port the board will no longer automatically reset to the bootloader during programming. You will have to reset the board yourself or use an external programmer.

## Case

[A 3D printed case design](cad) is available to protect the assembled shield and microcontroller. Both shields share the same general footprint and the same case design. When assembled, the case should hold the microcontroller and shield securely without any play.

The case is printed in two parts, and assembled using two M3-10 bolts and two M3 nuts. The recommended print orientation is as-is. Note that the top half of the case is a little tricky to print, and must be printed using bed supports.

## Building Your Own

If you'd like to make these yourself, you can find the [Gerber files](https://en.wikipedia.org/wiki/Gerber_format) and [Bill of Materials (BOM)](https://en.wikipedia.org/wiki/Bill_of_materials) in [the latest release](https://github.com/dmadison/Sim-Racing-Shields/releases/latest).

## License

The files in this repository are licensed under the terms of the [GNU General Public License (GPL)](https://www.gnu.org/licenses/gpl-3.0.html), either version 3 of the License, or (at your option) any later version. See the [LICENSE](LICENSE) file for more information.

Included images are licensed under the [CC BY-NC-ND 4.0 license](https://creativecommons.org/licenses/by-nc-nd/4.0/). Attribution should be to Dave Madison ([@dmadison](https://github.com/dmadison)).
