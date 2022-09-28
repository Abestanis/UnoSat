# UnoSat LoRa Shield Example

This example shows the basic template example with a
[LoRa 433 Mhz Radio Shield](https://www.makerfabs.com/lora-radio-shield-433mhz.html).
It demonstrates full wireless communication between a groundstation running on a device
via a proxy Arduino Uno to a base Arduino Uno. 

![Arduino Uno in UnoSat Lego template](images/Full%20Setup.jpg)

## Requirements
### Hardware

* 2x Arduino Uno
* 2x LoRa 433 Mhz Radio Shield

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
Now the [`main.cpp` file](src/main.cpp) can be edited to call the new generated functions and pass the data
gathered in step 2 to send to the base. 
