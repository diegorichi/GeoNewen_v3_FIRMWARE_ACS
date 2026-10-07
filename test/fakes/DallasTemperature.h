#pragma once
#include <stdint.h>
extern float fake_temperature_by_sensor[256];
extern bool fake_conversion_complete;
extern int fake_temperature_requests;

class DallasTemperature {
public:
    explicit DallasTemperature(void*) {}
    void begin() {}
    void setWaitForConversion(bool) {}
    float getTempC(const uint8_t* address) const {
        return fake_temperature_by_sensor[address[1]];
    }
    bool isConversionComplete() const { return fake_conversion_complete; }
    void requestTemperatures() { ++fake_temperature_requests; }
};
