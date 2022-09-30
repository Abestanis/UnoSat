# UnoSat LoRa Shield Example

This example shows the basic template example with a
[LoRa 433 MHz Radio Shield](https://www.makerfabs.com/lora-radio-shield-433mhz.html).
It demonstrates full wireless communication between a groundstation running on a device
via a proxy Arduino Uno to a base Arduino Uno.

![Arduino Uno in UnoSat Lego template](images/Full%20Setup.jpg)

## Requirements

### Hardware

* 2x Arduino Uno
* 2x LoRa 433 MHz Radio Shield

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

## Pin usage

The LoRa shield uses the following pins:

* D2 (RFM95_INT)
* D9 (RFM9x_RS)
* D10 (RFM9x_CS)
* D11 (RFM9x MOSI)
* D12 (RFM9x MISO)
* D13 (RFM9x SCK)

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
