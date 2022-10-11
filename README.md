# UnoSat Platform

The UnoSat Platform is a software and 3D structure platform build with Arduino Uno boards
that allows easier implementation of a satellite project in a 1-n unit CubeSat format.
It provides a Lego based extensible structures files that can be 3D printed.

This project is designed for the [M2 TSI master](https://m2tsi.eu)
at [Université Toulouse III-Paul Sabatier](https://www.univ-tlse3.fr)
and to be used in the Balloon Project of the master.

![An assembled UnoSat Lego structure with two Arduino Uno microcontrollers](images/UnoSat%20Stack.jpg)

## How to use it

This repository contains a [template software project](template) that can be used with
an Arduino Uno to get a basic system that is able to send telemetry via its serial port
to a connected computer. This template can be copied and used as a starting point to build
a satellite system on top of it.

There are also various [examples](examples) which showcase how the template was used and modified
to quickly built some functionality which is commonly required of most satellite systems,
like [local data storage using an SD-Card](examples/SD-Card) and
[reading and transmitting sensor data](examples/Grove%20Beginner%20Kit).

The UnoSat structure can be found in the [`structure`](structure) folder. 
