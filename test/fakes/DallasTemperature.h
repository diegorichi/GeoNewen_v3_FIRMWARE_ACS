#pragma once
#include <stdint.h>
extern float fakeTemperatureBySensor[256];
extern bool fakeConversionComplete;
extern int fakeTemperatureRequests;

class DallasTemperature {
public:
    explicit DallasTemperature(void*) {}
    void begin() {}
    void setWaitForConversion(bool) {}
    float getTempC(const uint8_t* address) const {
        return fakeTemperatureBySensor[address[1]];
    }
    bool isConversionComplete() const { return fakeConversionComplete; }
    void requestTemperatures() { ++fakeTemperatureRequests; }
};
