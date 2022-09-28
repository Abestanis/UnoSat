# UnoSat RTK Example

This example shows how to get centimeter precision RTK GPS location using an
[Ardusimple simpleRTK2B module](https://www.ardusimple.com/product/simplertk2b).

The sensor Arduino (middle in the image) is responsible for reading and decoding the GPS position from
the simpleRTK2B module (left in the image) and sending it to the base Arduino (right in the image).
The base Arduino forwards the GPS position and any log from the sensor Arduino via the USB cable
to a connected device.

The reason for using two Arduino Uno microcontrollers is because of the very limited
memory resources available on the Arduino Uno. The library used to parse the location messages from the RTK module
is very large, and uses more than 96% of the Arduino Uno's available Flash memory.

Note that for the centimeter precision positioning an RTK base station is required in addition to everything.
Without the base station, the setup will provide GPS position with regular precision.

![Two Arduino Uno microcontrollers and a simpleRTK2B module](images/Full%20System.jpg)

## Requirements

### Hardware

* 2x Arduino Uno
* Ardusimple simpleRTK2B module
* At least 9 jumper cables
* Either a breadboard or a soldering iron

### Software

[Python](https://www.python.org) needs to be installed on the system, at least version `3.7` is required.

This project uses the [ECom](https://gitlab.com/team-aster/software/ecom) library
to define how to communicate with the Arduino and for generating some code for it.
The library must be installed if any changes to the communication are necessary. It also includes a
[simple groundstation](https://gitlab.com/team-aster/software/ecom/-/tree/main/examples/simpleGroundstation)
as an example, which can be used to read the telemetry from the Arduino.

Because the code in this project is spread out over multiple files,
the Arduino IDE can not be used to compile and upload the project onto the Arduino.
Instead, this project uses [PlatformIO](https://docs.platformio.org),
which can be downloaded [here](https://docs.platformio.org/en/latest/core/installation/index.html).
Also install the PlatformIO shell commands by following the instructions
[here](https://docs.platformio.org/en/latest/core/installation/shell-commands.html).

[Clion](https://www.jetbrains.com/clion) with the
[Platform IO plugin](https://www.jetbrains.com/help/clion/platformio.html#install-plugin) can be used as an
alternative to the Arduino IDE, but any other IDE should work as well.

After the installation, run the following command in the [root directory of the project](.)
(where this README is located) to finish initialization of the project:

```shell
pio project init
```

## Hardware setup

In this setup, the base Arduino is on a different power supply than the sensor Arduino and the simpleRTK2B module.
Both Arduino microcontrollers receive power either via the power jack or USB cable.
The base Arduino also uses the USB cable to send telemetry to a connected device.
The Arduino systems must share a common ground, so a ground pin of one Arduino must be connected
to a ground pin of the other Arduino. (In the image, they are connected via a breadboard.)

The communication line with the sensor Arduino needs to be connected as follows:
The RX pin (pin 7) on the Arduino is connected to the TX pin (pin 3) on the sensor Arduino.
The TX pin (pin 8) on the Arduino is connected to the RX pin (pin 2) on the sensor Arduino.

For the pin setup of the sensor Arduino and the simpleRTK2B module
see the [RTK sensor documentation](RTK%20Sensor/README.md#hardware-setup).

| Base Arduino pins                                   | Sensor Arduino pins                                       |
|-----------------------------------------------------|-----------------------------------------------------------|
| ![Arduino Uno pin setup](images/Base%20Arduino.jpg) | ![simpleRTK2B pin setup](RTK%20Sensor/images/Arduino.jpg) |

## Building and Flashing

To build the project and upload it to an Arduino Uno, connect the Arduino to the computer.
If some hardware is connected to the UART1 port of the Arduino, temporarily unplug it,
otherwise the upload will fail because the UART1 and USB connection share the same port.
Finally, execute the following command in the [root directory of the project](.):

```shell
pio run --target upload
```

This will upload the code onto the Arduino.
If multiple devices are connected to the computer, it might be necessary to specify
which serial port to upload to via the `--upload-port` argument.

### Change the communication definition

When adding a new telemetry package or changing the data sent with an existing telemetry package,
the communication database in the [`communication` folder](communication) has to be edited.
An explanation of the files and folders in that directory and how to edit them can be found in the
[ECom documentation](https://ecom.readthedocs.io/en/latest/database/README.html).
No special program is required, a simple text editor is enough. After the changes have been made,
the following command has to be executed in the [root directory of the project](.) to update the generated code:

```shell
ecomUpdate --database communication src include
```

If any changes are made to the [communication database of the RTK sensor](RTK%20Sensor/communication),
the following command has to be executed:

```shell
ecomUpdate --forBase --prefix rtk --database "RTK Sensor/communication" src include
```

## Troubleshooting

* `Error: Unknown command "pio"` or similar error:

  This means that PlatformIO is not installed, or not installed correctly,
  or that the PlatformIO shell commands are not installed correctly.
  Make sure that no step in the [software requirements section](#software) was skipped.

* `Error: Unknown command "ecomUpdate"` or similar error:

  This means that ECom library is not installed. If the ECom library was just downloaded,
  but not installed, then the full path to the `ecomUpdate.py` has to be specified when executing the command.

* The `pio run` command fails while trying to find an upload port, even though the Arduino is connected.

  Check that the cable to the Arduino is actually connected.
  Additionally, old Windows systems might not have the required drivers installed to communicate with the Arduino.
  In this case it is recommended to install the Arduino IDE, because that will also install all the necessary drivers.
