int pin = A0;//left potentiometer
int sensorValue;
int pin2 = A1;//right potentiometer
int sensorValue2;
int RedLED = 9;
int GreenLED = 8;
float sensorPercentage1;
float sensorPercentage2;
float lowerBound1;
float lowBound2;
float sensorDifference;
float differenceAverage;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(RedLED, OUTPUT);
pinMode(GreenLED,OUTPUT);
}

void loop() {
  // put  your main code here, to run repeatedly:
sensorValue = analogRead(pin);
sensorValue2 = analogRead(pin2);
// maps analog values of Sensor1 and Sensor2 from 0,100
sensorPercentage1 = 100-(map(sensorValue,280,876,0,100));
sensorPercentage2 = 100-(map(sensorValue2,102,1023,0,100));

Serial.print("Sensor 1: ");
Serial.print(sensorPercentage1);

Serial.print(" Sensor 2: ");
Serial.println(sensorPercentage2);

sensorDifference = sensorPercentage1 - sensorPercentage2;

differenceAverage = (sensorPercentage1 + sensorPercentage2) / 2;
Serial.println("differenceAverage: " + String(differenceAverage));

//filter range edge values and adjust them
if (sensorDifference > -10 && sensorDifference < 10) {
  sensorDifference = sensorDifference;
}else{
  differenceAverage = 0;
}

Serial.println("sensorDifference: " + String(sensorDifference));
map(sensorDifference

if (sensorValue < 476 || sensorValue2 < 110) {
  digitalWrite(RedLED, HIGH);
}else{
  digitalWrite(RedLED, LOW);
}
if (sensorValue > 1020 || sensorValue2 > 878) {
  digitalWrite(GreenLED, HIGH);
}else{
  digitalWrite(GreenLED, LOW);
}
//if (sensorValue < 476) {
//  digitalWrite(RedLED, HIGH);
//}else{
//  digitalWrite(RedLED, LOW);
//}
//if (sensorValue > 1023) {
//   digitalWrite(GreenLED,HIGH);
//   }else{
//  digitalWrite(GreenLED, LOW);
//}
//if (sensorValue2 < 110) {
//  digitalWrite(RedLED, HIGH);
//  }else{
//  digitalWrite(RedLED, LOW);
//}
//if (sensorValue2 > 878) {
//   digitalWrite(GreenLED,HIGH);
//   }else{
//  digitalWrite(GreenLED, LOW);
//}
}
