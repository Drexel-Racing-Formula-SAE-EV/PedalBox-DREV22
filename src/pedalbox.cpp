#include "poten.h"

#include <SPI.h>

#include <drev_can.h>

#include <Arduino.h>

// Frequency of reading and sending torque data
#define FREQ    100 

// Define period as 10ms if frequency isn't defined above
#ifndef FREQ 
#define PERIOD 10 
#else
#define PERIOD  ( 1000 / FREQ ) 
#endif

#define LEFT_POTEN_PIN A1
#define RIGHT_POTEN_PIN A2

#define RxID 0x201  // CAN recieving addr for motor controller
#define TORQUE_CMD 0x90 

unsigned long last_write = millis();
drev_can::can_node canBus(RxID);
uint8_t testData[] = {TORQUE_CMD, 0xFC, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00};

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

void set_msg_data(drev_can::can_message msgObj, uint8_t *newMsg) {
    for (int i=0; i<8; i++){
        msgObj.data[i] = newMsg[i];
    }
}

void loop() {
    if (millis() - last_write > PERIOD){
        //trq = (left_poten.read_percent() + right_poten.read_percent()) / 2;
        drev_can::can_message message{
            .length=3,
        };

        set_msg_data(message, testData);

        Serial.println(canBus.send(message));
        last_write = millis();
    }
}
