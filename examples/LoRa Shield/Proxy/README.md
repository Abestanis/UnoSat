# UnoSat LoRa Shield - Proxy example

This is part of the UnoSat LoRa Shield example and demonstrate a very simple proxy
that sits between the groundstation and the base Arduino.
The groundstation is most likely is running on a device that doesn't have direct access
to a 433 MHz LoRa wireless module. This proxy, an Arduino Uno with a
[LoRa 433 Mhz Radio Shield](https://www.makerfabs.com/lora-radio-shield-433mhz.html), allows the
groundstation to receive the messages send with LoRa via their USB port.

![Arduino Uno in UnoSat Lego template](images/Proxy.jpg)

## Requirements

### Hardware

* Arduino Uno
* LoRa 433 Mhz Radio Shield

### Software

The [simple groundstation](https://gitlab.com/team-aster/software/ecom/-/tree/main/examples/simpleGroundstation)
from the [ECom](https://gitlab.com/team-aster/software/ecom) library can be used to read the telemetry from the Arduino.
To use it, [Python](https://www.python.org) needs to be installed on the system, at least version `3.7` is required.

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

## Troubleshooting

* `Error: Unknown command "pio"` or similar error:

  This means that PlatformIO is not installed, or not installed correctly,
  or that the PlatformIO shell commands are not installed correctly.
  Make sure that no step in the [software requirements section](#software) was skipped.

* The `pio run` command fails while trying to find an upload port, even though the Arduino is connected.

  Check that the cable to the Arduino is actually connected.
  Additionally, old Windows systems might not have the required drivers installed to communicate with the Arduino.
  In this case it is recommended to install the Arduino IDE, because that will also install all the necessary drivers.
