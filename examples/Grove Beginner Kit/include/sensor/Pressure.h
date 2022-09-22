#pragma once

#include "Seeed_BMP280.h"
#include "generated/units.h"


namespace Sensor {
    class Pressure {
    public:
        Pressure() = default;
        ~Pressure() = default;

        bool connect();
        bool readTemperature(celsius_t& temperature);
        bool readPressure(pascal_t& pressure);

    private:
        bool isConnected = false;
        BMP280 sensor = {};
    };
}
