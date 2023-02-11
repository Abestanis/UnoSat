# UnoSat SD-Card Example

This example shows how to use the [Seeed Studio SD-Card Shield V4](https://wiki.seeedstudio.com/SD_Card_shield_V4.0)
with the UnoSat platform.
The example is able to initialize the SD-Card while running (so it is possible to remove and re-insert the SD-Card
without restarting the Arduino, although that is not recommended),
read some information from it and write all telemetry to a log file on it.
The log is stored in a `log` folder on the SD-Card. When restarting, a new log file will be created.

![An Arduino Uno in the UnoSat Lego structure with an SD-Card shield](images/UnoSat%20with%20SD-Card%20Shield.jpg)

## Requirements

### Hardware

* Arduino Uno
* [Seeed Studio SD-Card Shield V4](https://wiki.seeedstudio.com/SD_Card_shield_V4.0)
* SD-Card or micro SD-Card up to 32Gb

### Software

[Python](https://www.python.org) needs to be installed on the system, at least version `3.7` is required.

This project uses the [ECom](https://gitlab.com/team-aster/software/ecom) library (version `1.1`)
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
ecomUpdate --database communication src include
```

## Pin usage

The SD-Card shield uses the following pins:

* D4 (SD_CS)
* D11 (SD_DI)
* D12 (SD_DO)
* D13 (SD_CLK)

## Read the log files

The log files on the SD-Card are stored in the same format as if they were sent as regular telemetry,
therefore they can not be read as simple text with a text editor.
Because the telemetry is optimized for size and allows to detect invalid messages, the same applies
to the saved files. They are as small as possible and if the SD-Card were to get corrupted, one could
detect that while parsing the file.

To parse the log files, The [ECom library](https://gitlab.com/team-aster/software/ecom) contains a
small tool called `logConverter` that can convert the saved file into a csv file. The tool can be downloaded
and its documentation can be read [here](https://gitlab.com/team-aster/software/ecom/-/tree/main/examples/logConverter).

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
