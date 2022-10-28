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
#include <Metro.h>
#include <kinetis_flexcan.h>
#include <FlexCAN.h>
#include <poten.h>

#define LEFT_POTEN_PIN A1
#define RIGHT_POTEN_PIN A2

#define PERIOD  500
#define LED 13
#define DEBUG 0

Metro sysTimer = Metro(1);// milliseconds

FlexCAN CANbus(500000);
static CAN_message_t msg, rxmsg;
static uint8_t hex[17] = "0123456789abcdef";
uint16_t trq_hex;

int txCount,rxCount;
unsigned int txTimer,rxTimer;

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

// -------------------------------------------------------------
static void hexDump(uint8_t dumpLen, uint8_t *bytePtr)
{
  uint8_t working;
  while( dumpLen-- ) {
    working = *bytePtr++;
    Serial.write( hex[ working>>4 ] );
    Serial.write( hex[ working&15 ] );
  }
  Serial.write('\r');
  Serial.write('\n');
}

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

  pinMode(LEFT_POTEN_PIN, INPUT);
  pinMode(RIGHT_POTEN_PIN, INPUT);
  pinMode(LED, OUTPUT);
  
  CANbus.begin();
  digitalWrite(LED, 1);
  delay(1000);
  sysTimer.reset();
}

// -------------------------------------------------------------
void loop(void)
{
  // service software timers based on Metro tick
  if ( sysTimer.check() ) {
    if ( txTimer ) {
      --txTimer;
    }
    if ( rxTimer ) {
      --rxTimer;
    }
  }

  // if not time-delayed, read CAN messages and print 1st byte
  if ( !rxTimer ) {
    while ( CANbus.read(rxmsg) ) {
      if (DEBUG){
        hexDump( sizeof(rxmsg), (uint8_t *)&rxmsg );
        Serial.write(rxmsg.buf[0]);
      }
      rxCount++;
    }
  }

  // insert a time delay between transmissions
  if ( !txTimer ) {
    // if frames were received, print the count
    if ( rxCount ) {
      if (DEBUG){
        Serial.write('=');
        Serial.print(rxCount);
      }
      rxCount = 0;
    }
    txTimer = PERIOD; //milliseconds

      Serial.println("\nCAN Message:");
      //Define a torque command of 50% Torque
      trq_hex = to_trq_hex(left_poten.read_percent());
      msg.buf[1] = trq_hex%256;
      msg.buf[2] = trq_hex/256;
      Serial.print("0x"); Serial.println(trq_hex, HEX);
      Serial.print(left_poten.read_percent()); Serial.println("%");

      if (DEBUG){
        for( int idx=0; idx<8; ++idx ) {
          msg.buf[2] = 0x0 + idx;//a way to convert an int from 0 to 9 to it's char equivalent
          Serial.write('0'+idx);
          Serial.write("\n");
        }
      }
    
    
    // send 6 at a time to force tx buffering
    txCount = 6;
    digitalWrite(LED, 1);
    if (DEBUG){
      Serial.println(".");
      hexDump( sizeof(msg), (uint8_t *)&msg );
    }
    while ( txCount-- ) {
      int ret = CANbus.write(msg);
      if (DEBUG){
        Serial.println("ret: " + String(ret));
        Serial.println("timeout: " + String(msg.timeout));
        msg.buf[0]++;
      }
    }
    digitalWrite(LED, 0);
    // time delay to force some rx data queue use
    rxTimer = 3;//milliseconds
  }
}