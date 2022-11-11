#include <Arduino.h>
#include <brake.h>


// sensor is analog, type 33 0.5 – 4.5V 5 ± 0.25V
// 5 1/4-18 NPT 0.596[15.14]

brake::brake(int pin_one, int pin_two, int light_pin){
    pin_one = pin_one;
    pin_two = pin_two;
    light_pin = light_pin;

}

int brake::get_pressure() {
    return (analogRead(pin_one) + analogRead(pin_two)) / 2;
}

int brake::pressure_percentage(){
    return map(get_pressure(), 0, 1023, 0, 100);
}

void brake::brake_light(){
    if (get_pressure() > BRAKE_THRESHOLD) {
        digitalWrite(light_pin, HIGH);
    } else {
        digitalWrite(light_pin, LOW);
    }
}
