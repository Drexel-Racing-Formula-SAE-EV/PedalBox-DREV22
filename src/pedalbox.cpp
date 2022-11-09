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

#define DEBUG 1

#define LEFT_POTEN_PIN A1
#define RIGHT_POTEN_PIN A2
#define LED 13

// TODO: Calibrate Potentiometers
#define LPOT_MIN    320     // Left Potentiometer Lower Bound
#define LPOT_MAX    1023    // Left Potentiometer Upper Bound
#define RPOT_MIN    545     // Right Potentiometer Lower Bound
#define RPOT_MAX    1023    // Right Potentiometer Upper Bound

#define PERIOD 250
#define THRESH 5

FlexCAN CANbus(500000);
static CAN_message_t msg;
uint16_t trq_hex;

int Lpot, Rpot, PotFinal, LRaw, RRaw;
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
    LRaw = left_poten.read();
    Rpot = right_poten.read_percent();
    RRaw = right_poten.read();

    PotFinal = (Lpot+Rpot)/2;
    // If the Potentiometers are significantly different, default to LPot
    if (abs(Lpot - Rpot) > THRESH){
      PotFinal=Lpot;
    }

    PotFinal = PotFinal>100? 100 : PotFinal;

    trq_hex = to_trq_hex(PotFinal);
    msg.buf[1] = trq_hex%256;
    msg.buf[2] = trq_hex/256;

   int ret = CANbus.write(msg);
    if (DEBUG){
      Serial.println("\nPotentiometer Data:");
      Serial.printf("Left: [%d, %d\%]\n", LRaw, Lpot);
      Serial.printf("Right: [%d, %d\%]\n", RRaw, Rpot);
      Serial.println("CAN Message:");
      Serial.printf("ret: %d\n", ret);
      Serial.printf("timeout: %d\n", msg.timeout);
      Serial.printf("PotFinal: %d\n", PotFinal);
      Serial.print("0x"); Serial.println(trq_hex, HEX);
    }
    if (PERIOD > 50){
      delay(25);
    }
  }
  digitalWrite(LED, LOW);
}