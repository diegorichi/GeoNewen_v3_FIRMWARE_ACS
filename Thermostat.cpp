

#include "Thermostat.h"

bool Thermostat::isOn() {
    return (
        (digitalRead(this->pinout->DI_MARCHA) == HIGH)
        && !this->modoFrio)
        || ((digitalRead(this->pinout->DI_MARCHA) == LOW)
            && this->modoFrio);

}

bool Thermostat::isOff() {
    return (
        (digitalRead(this->pinout->DI_MARCHA) == LOW)
        && !this->modoFrio)
        || ((digitalRead(this->pinout->DI_MARCHA) == HIGH)
            && this->modoFrio);
}
