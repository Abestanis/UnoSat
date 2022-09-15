# Communication database

This database defines both telemetry and telecommands between the Arduino and the base.
It is spread out over multiple files and folders:

* [telemetry.csv](telemetry.csv) [(Documentation)](documentation/telemetry.md)):
  A list of possible types of telemetry messages.
* [telemetryArguments](telemetryArguments) [(Documentation)](documentation/telemetryArguments.md):
  This folder contains the definition of the data that is sent with each telemetry type.
  The definition must be in a file with the same name as the telemetry type.
* [sharedConstants.csv](sharedConstants.csv) [(Documentation)](documentation/sharedConstants.md):
  Constant values that are used in the communication.
* [sharedDataTypes.json](sharedDataTypes.json) [(Documentation)](documentation/sharedDataTypes.md):
  A description of data types that are used in the communication.
* [units.csv](units.csv) [(Documentation)](documentation/units.md):
  A description of units and their base datatypes used in the communication.
