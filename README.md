# Sim Racing Shields

![Sim Racing Shields Header](../master/images/Sim-Racing-Shields_Header.jpg)

This repo contains add-on circuit boards ("shields") for connecting Arduino development boards to sim racing equipment. These shields are meant to be used in conjunction with the [Sim Racing Library for Arduino](https://github.com/dmadison/Sim-Racing-Arduino). These shields can be used to create inexpensive, standalone USB HID adapters for use with racing games and simulators.

Both of these shields are designed to be used with a [SparkFun Pro Micro, 5V (32U4)](https://github.com/sparkfun/Pro_Micro). You can use either genuine SparkFun boards or 3rd party clones. The [SparkFun Qwiic Pro Micro with a USB-C port (DEV-15795)](https://www.sparkfun.com/products/15795) is recommended. The SparkFun Pro Micro RP2040 is theoretically compatible but has not been tested. Note that the Tag-Connect programming header is not compatible with the RP2040.

The shields can be assembled with either 0.1″ female headers for prototyping or 0.1″ male headers for a permanent connection. Only assembled boards with male headers will be able to fit inside the 3D printed case.

For more information, [see the article on PartsNotIncluded.com](https://www.partsnotincluded.com/sim-racing-shields-for-arduino/).

## Logitech Shifter Shield for Pro Micro

The [Logitech Shifter Shield](pcbs/Logitech_Shifter_Shield_Pro_Micro) is designed to interface with the Logitech Driving Force Shifter using a single DB-9 male connector.

Using the [Sim Racing Library for Arduino](https://github.com/dmadison/Sim-Racing-Arduino),
use the `SHIFTER_SHIELD_V1_PINS` macro for easy setup with the shield:

```cpp
LogitechShifter shifter(SHIFTER_SHIELD_V1_PINS);
```

For use as a USB HID adapter, upload the [ShiftJoystick](https://github.com/dmadison/Sim-Racing-Arduino/blob/master/examples/Shifter/ShiftJoystick/ShiftJoystick.ino) library example. Make sure to modify the object declaration with the shield-specific pin definition (above) before uploading.

## Logitech Pedals Shield for Pro Micro

The [Logitech Pedal Shield](pcbs/Logitech_Pedal_Shield_Pro_Micro) is designed to interface with the Logitech three pedal attachment, included with the Logitech G923 “TRUEFORCE”, G29/G920 “Driving Force”, and G27/G25 wheels, via a single DB-9 female connector.

Using the [Sim Racing Library for Arduino](https://github.com/dmadison/Sim-Racing-Arduino),
use the `PEDAL_SHIELD_V1_PINS` macro for easy setup with the shield:

```cpp
LogitechPedals pedals(PEDAL_SHIELD_V1_PINS);
```

For use as a USB HID adapter, upload the [PedalsJoystick](https://github.com/dmadison/Sim-Racing-Arduino/blob/master/examples/Pedals/PedalsJoystick/PedalsJoystick.ino) library example. Make sure to modify the object declaration with the shield-specific pin definition (above) before uploading.

## Firmware

The shields are designed to be used with the [Sim Racing Library for Arduino](https://github.com/dmadison/Sim-Racing-Arduino). You can use any included example for your specific hardware, or write your own using the data the library provides.

Both shields use the SparkFun Pro Micro (32U4), and firmware should be compiled using the "SparkFun Pro Micro" board in the Arduino IDE. You can find the compilation files and installation instructions for the Pro Micro [here](https://github.com/sparkfun/Arduino_Boards).

## Case

[A 3D printed case design](cad) is available to protect the assembled shield and microcontroller. Both shields share the same general footprint and the same case design. When assembled, the case should hold the microcontroller and shield securely without any play.

The case is printed in two parts, and assembled using two M3-10 bolts and two M3 nuts. The recommended print orientation is as-is. Note that the top half of the case is a little tricky to print, and must be printed using bed supports.

## License

The files in this repository are licensed under the terms of the [GNU General Public License (GPL)](https://www.gnu.org/licenses/gpl-3.0.html), either version 3 of the License, or (at your option) any later version. See the [LICENSE](LICENSE) file for more information.

Included images are licensed under the [CC BY-NC-ND 4.0 license](https://creativecommons.org/licenses/by-nc-nd/4.0/). Attribution should be to Dave Madison ([@dmadison](https://github.com/dmadison)).
