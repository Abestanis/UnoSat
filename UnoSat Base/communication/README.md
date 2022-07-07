# Communication database

This database defines both telemetry and telecommands between the main board and the ground station.
It is spread out over multiple files and folders:

* [commands.csv](commands.csv) [(Documentation)](documentation/commands.md):
  Contains a list of all telecommands and describes an optional return value.
* [commandArguments](commandArguments) [(Documentation)](documentation/commandArguments.md):
  This folder contains the arguments for the telecommands. If a telecommand has arguments,
  they must be in a file with the same name as the telecommand.
* [telemetry.csv](telemetry.csv) [(Documentation)](documentation/telemetry.md)):
  A list of possible types of telemetry messages.
* [telemetryArguments](telemetryArguments) [(Documentation)](documentation/telemetryArguments.md):
  This folder contains the definition of the data that is sent with each telemetry type.
  The definition must be in a file with the same name as the telemetry type.
* [sharedConstants.csv](sharedConstants.csv) [(Documentation)](documentation/sharedConstants.md):
  Constant values that are used in the communication.
* [sharedDataTypes.json](sharedDataTypes.json) [(Documentation)](documentation/sharedDataTypes.md):
  A description of data types that are used in the communication.
* [configuration.csv](configuration.csv) [(Documentation)](documentation/configuration.md):
  A description of configuration parameters that can be changed with the configuration telecommand.
* [units.csv](units.csv) [(Documentation)](documentation/units.md):
  A description of units and their base datatypes used in the communication.

Further documentation can be found in the [documentation](documentation) folder.

## Library

The [lib](lib) folder contains a Python library to parse and serialize communication packages.
It also contains a code generator which can generate a C parser and serializer.
