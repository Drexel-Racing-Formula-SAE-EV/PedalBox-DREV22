// -------------------------------------------------------------
// CANtest for Teensy 3.1
// by teachop
//
// This test is talking to a single other echo-node on the bus.
// 6 frames are transmitted and rx frames are counted.
// Tx and rx are done in a way to force some driver buffering.
// Serial is used to print the ongoing status.
//
#include <Arduino.h>
#include <kinetis_flexcan.h>
#include <FlexCAN.h>
#include <poten.h>

#define LEFT_POTEN_PIN A1
#define RIGHT_POTEN_PIN A2

#define LPOT_MIN    72      // Left Potentiometer Lower Bound
#define LPOT_MAX    1023    // Left Potentiometer Upper Bound
#define RPOT_MIN    359     // Right Potentiometer Lower Bound
#define RPOT_MAX    1023    // Right Potentiometer Upper Bound

#define PERIOD 250
#define LED 13
#define DEBUG 0

FlexCAN CANbus(500000);
static CAN_message_t msg;
uint16_t trq_hex;

int Lpot, Rpot;
unsigned long last_write;

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

uint16_t to_trq_hex(short percent){
  return map(percent, 0, 100, 0x0000, 0x5555);
}

// -------------------------------------------------------------
void setup(void)
{
  msg.timeout = 500;
  msg.len = 3;
  msg.id = 0x201;
  msg.ext = 0x0;
  msg.buf[0] = 0x90;

  pinMode(LEFT_POTEN_PIN, INPUT); // POT 1
  pinMode(RIGHT_POTEN_PIN, INPUT); // POT 2
  pinMode(LED, OUTPUT);
  
  last_write = millis();
  CANbus.begin();
  delay(1000);
}

// -------------------------------------------------------------
void loop(void)
{
  if ( (millis() - last_write) > PERIOD){
    last_write = millis();
    digitalWrite(LED, HIGH);
    Lpot = left_poten.read_percent();
    Rpot = right_poten.read_percent();
    trq_hex = to_trq_hex(Lpot);
    msg.buf[1] = trq_hex%256;
    msg.buf[2] = trq_hex/256;

    // For calibrating pots
    //Serial.print("Left: "); Serial.println(left_poten.read());
    //Serial.print("Right: "); Serial.println(right_poten.read());
    // send 6 at a time to force tx buffering
    int ret = CANbus.write(msg);
    if (DEBUG){
      Serial.println("\nCAN Message:");
      Serial.println("ret: " + String(ret));
      Serial.println("timeout: " + String(msg.timeout));
      Serial.print("0x"); Serial.println(trq_hex, HEX);
      Serial.print(Lpot); Serial.println("%");
    }
    if (PERIOD > 50){
      delay(50);
    }
  }
  digitalWrite(LED, LOW);
}