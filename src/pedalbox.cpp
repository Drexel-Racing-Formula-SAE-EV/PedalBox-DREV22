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

#define RxID        0x201   // CAN recieving addr for motor controller
#define TORQUE_CMD  0x90    // Register for TRQ command to be set on motorcontroller

const struct poten_range left_poten_range = {
    .lower_bound = LPOT_MIN,
    .upper_bound = LPOT_MAX,
};

const struct poten_range right_poten_range = {
    .lower_bound = RPOT_MIN,
    .upper_bound = RPOT_MAX,
};

unsigned long last_write = millis();

poten left_poten = poten{LEFT_POTEN_PIN, left_poten_range};
poten right_poten = poten{RIGHT_POTEN_PIN, right_poten_range};

uint8_t testData[8];

drev_can::can_node canBus(RxID);

drev_can::can_message canMSG {
    .id=RxID,
    .length=3,
};

void set_msg_data(drev_can::can_message msgObj, uint8_t *newData, uint8_t len) {
    for (int i=0; i<len; i++){
        msgObj.data[i] = newData[i];
    }
}

void setup() {
    Serial.begin(9600);
    pinMode(LEFT_POTEN_PIN, INPUT);
    pinMode(RIGHT_POTEN_PIN, INPUT);
    testData[0] = TORQUE_CMD;
    testData[1] = 0xFC;
    testData[2] = 0x3F;
}

void loop() {
    if (millis() - last_write > PERIOD){
        //trq = (left_poten.read_percent() + right_poten.read_percent()) / 2;

        set_msg_data(canMSG, testData, 3);

        Serial.println(canBus.send(canMSG));
        last_write = millis();
    }
