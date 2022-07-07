#include "sensor/Pressure.h"

bool Sensor::Pressure::connect() {
    isConnected = sensor.init();
    return isConnected;
}

bool Sensor::Pressure::readTemperature(celsius_t& temperature) {
    if (!isConnected) {
        return false;
    }
    temperature.celsius = sensor.getTemperature();
    return true;
}

bool Sensor::Pressure::readPressure(pascal_t& pressure) {
    if (!isConnected) {
        return false;
    }
    pressure.pascal = sensor.getPressure();
    return true;
}
