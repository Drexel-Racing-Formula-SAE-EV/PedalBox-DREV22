#include <Arduino.h>

#include "brake.h"

// sensor is analog, type 33 0.5 – 4.5V 5 ± 0.25V
// 5 1/4-18 NPT 0.596[15.14]

brake::brake(int pin_left, int pin_right, int light_pin)

int::brake get_pressure() {
    return (analogRead(pin_left) + analogRead(pin_right)) / 2;
}

int::brake pressure_percentage(){
    return map(get_pressure(), 0, 1023, 0, 100)
}

void::brake brake_light(){
    if get_pressure(){
        digitalWrite(light_pin, HIGH);
    }
}

