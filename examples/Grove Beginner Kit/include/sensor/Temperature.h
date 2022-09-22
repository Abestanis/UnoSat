#pragma once

#include <DHT.h>
#include "generated/units.h"


namespace Sensor {
    class Temperature {
    public:
        explicit Temperature(int pin, int dhtType = DHT11) : sensor(pin, dhtType) {} ;
        ~Temperature() = default;

        bool connect();
        bool readTemperature(celsius_t& temperature);
        bool readHumidity(percent_t& humidity);

    private:
        bool isConnected = false;
        DHT sensor;
    };
}
