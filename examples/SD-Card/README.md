# UnoSat SD-Card Example

This example shows how to use the [Seeed Studio SD-Card Shield V4](https://wiki.seeedstudio.com/SD_Card_shield_V4.0)
with the UnoSat platform.
The example is able to initialize the SD-Card while running,
read some information from it and write all telemetry to a log file on it.

![An Arduino Uno in the UnoSat Lego structure with an SD-Card shield](images/UnoSat%20with%20SD-Card%20Shield.jpg)

## Requirements
### Hardware

* Arduino Uno
* [Seeed Studio SD-Card Shield V4](https://wiki.seeedstudio.com/SD_Card_shield_V4.0)
* SD-Card or micro SD-Card up to 32Gb

### Software
This project uses the [ECom](https://gitlab.com/team-aster/software/ecom) library
to define how to communicate with the Arduino and for generating some code for it.
The library must be installed if any changes to the communication are necessary. It also includes a
[simple groundstation](https://gitlab.com/team-aster/software/ecom/-/tree/main/examples/simpleGroundstation)
as an example, which can be used to read the telemetry from the Arduino.

Because the code in this project is spread out over multiple files,
the Arduino IDE can not be used to compile and upload the project onto the Arduino.
Instead, this project uses [PlatformIO](https://docs.platformio.org),
which can be downloaded [here](https://docs.platformio.org/en/latest/core/installation/index.html).

After the installation, run the following command in the [root directory of the project](.)
(where this README is located) to finish initialization of the project:

```shell
pio project init
```

## Building and Flashing

To build the project and upload it to an Arduino Uno, connect the Arduino to the computer.
If some hardware is connected to the UART1 port of the Arduino, temporarily unplug it,
otherwise the upload will fail because the UART1 and USB connection share the same port.
The SD-Card shield is not connected via UART1, so it can be attached to the Arduino during flashing. 
Finally, execute the following command in the [root directory of the project](.):

```shell
pio run --target upload
```

This will upload the code onto the Arduino.
If multiple devices are connected to the computer, it might be necessary to specify
which serial port to upload to via the `--upload-port` argument.

### Updating the communication definition

When changing the communication database in the [`communication` folder](communication),
the following command has to be executed in the [current directory](.) to update the generated code:
```shell
python ecom/codeGen.py --database communication src include
```

## Pin usage

The SD-Card shield uses the following pins:
* D4 (SD_CS)
* D11 (SD_DI)
* D12 (SD_DO)
* D13 (SD_CLK)
