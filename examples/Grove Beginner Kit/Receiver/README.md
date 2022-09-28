# Grove Beginner Kit for Arduino wireless example - Receiver

This is the receiver part of the UnoSat Grove Beginner Kit for Arduino wireless example.
It consists of a Grove Beginner Kit for Arduino with a LoRa module, which doesn't use any of
its sensors, but receives sensor information from the base Grove Beginner Kit and displays it.

## Requirements

This project uses the [ECom](https://gitlab.com/team-aster/software/ecom) library
to define how to communicate with the Arduino and for generating some code for it.
The library must be installed if any changes to the communication are necessary.

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
Temporarily remove the UART1 connection to the LoRa module on the kit
and execute the following command in the [current directory](.):
```shell
pio run --target upload
```

This will upload the code onto the Arduino.
If multiple devices are connected to the computer, it might be necessary to specify
which serial port to upload to via the `--upload-port` argument.

## Updating the communication definition

When changing the communication database in the [`communication` folder](../communication),
the following command has to be executed in the [current directory](.) to update the generated code:
```shell
ecomUpdate --forBase --database ../communication src include
```
