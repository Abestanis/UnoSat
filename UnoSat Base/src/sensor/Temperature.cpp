#include <math.h>
#include "sensor/Temperature.h"

bool Sensor::Temperature::connect() {
    sensor.begin();
    isConnected = !isnan(sensor.readTemperature());
    return isConnected;
}

bool Sensor::Temperature::readTemperature(celsius_t& temperature) {
    if (!isConnected) {
        return false;
    }
    temperature.celsius = sensor.readTemperature();
    return !isnan(temperature.celsius);
}

bool Sensor::Temperature::readHumidity(percent_t& humidity) {
    if (!isConnected) {
        return false;
    }
    humidity.percent = sensor.readHumidity();
    return !isnan(humidity.percent);
}
