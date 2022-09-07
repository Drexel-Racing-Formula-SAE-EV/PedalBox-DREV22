#include "poten.h"

#include <drev_can.h>

#include <Arduino.h>

#define LEFT_POTEN_PIN A1
#define RIGHT_POTEN_PIN A2

drev_can::can_node x{0};

const struct poten_range left_poten_range = {
    .lower_bound = 367,
    .upper_bound = 1023,
};

const struct poten_range right_poten_range = {
    .lower_bound = 104,
    .upper_bound = 1023,
};

poten left_poten = poten{LEFT_POTEN_PIN, left_poten_range};
poten right_poten = poten{RIGHT_POTEN_PIN, right_poten_range};

void setup() {
    Serial.begin(9600);
    pinMode(LEFT_POTEN_PIN, INPUT);
    pinMode(RIGHT_POTEN_PIN, INPUT);
}

void loop() {
    char buf[64];
    snprintf(buf, sizeof(buf), "[%d, %d] [%d, %d]\n", left_poten.read(),
             right_poten.read(), left_poten.read_percent(),
             right_poten.read_percent());
    Serial.println(buf);
}
