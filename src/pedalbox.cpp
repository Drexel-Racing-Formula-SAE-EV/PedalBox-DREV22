#include "poten.h"

#include <SPI.h>

#include <drev_can.h>

#include <Arduino.h>

#define FREQ    100

#define LEFT_POTEN_PIN A1
#define RIGHT_POTEN_PIN A2

#define RxID 0x201  // CAN recieving addr for motor controller
#define TORQUE_CMD 0x90 

unsigned long last_write = millis();
drev_can::can_node canBus{RxID};

const struct poten_range left_poten_range = {
    .lower_bound = LPOT_MIN,
    .upper_bound = LPOT_MAX,
};

const struct poten_range right_poten_range = {
    .lower_bound = RPOT_MIN,
    .upper_bound = RPOT_MAX,
};

poten left_poten = poten{LEFT_POTEN_PIN, left_poten_range};
poten right_poten = poten{RIGHT_POTEN_PIN, right_poten_range};

void setup() {
    Serial.begin(9600);
    pinMode(LEFT_POTEN_PIN, INPUT);
    pinMode(RIGHT_POTEN_PIN, INPUT);
}

void loop() {
    //short trq;

    if (millis() - last_write > (1000/FREQ)){
        //trq = (left_poten.read_percent() + right_poten.read_percent()) / 2;

        drev_can::can_message message;
        message.data[0] = TORQUE_CMD;
        message.data[1] = 0xFC;
        message.data[2] = 0x3F;
        message.data[3] = 0x00;
        message.data[4] = 0x00;
        message.data[5] = 0x00;
        message.data[6] = 0x00;
        message.data[7] = 0x00;

        Serial.println(canBus.send(message));
        last_write = millis();
    }
}
