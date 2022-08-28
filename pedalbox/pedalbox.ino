#define LEFT_POTEN_PIN A1
#define RIGHT_POTEN_PIN A2

float sensorDifference;
float differenceAverage;

struct poten_range {
	int lower_bound;
	int upper_bound;
};

struct poten_range left_poten_range = {
	.lower_bound = 367,
	.upper_bound = 1023,
};

struct poten_range right_poten_range = {
	.lower_bound = 104,
	.upper_bound = 1023,
};

void setup() {
	Serial.begin(9600);
	pinMode(LEFT_POTEN_PIN, INPUT);
	pinMode(RIGHT_POTEN_PIN, INPUT);
}

void loop() {
	int left_poten_value = analogRead(LEFT_POTEN_PIN);
	int right_poten_value = analogRead(RIGHT_POTEN_PIN);

	int left_sensor_percentage = 100 - map(left_poten_value, left_poten_range.lower_bound, left_poten_range.upper_bound, 0, 100);
	int right_sensor_percentage = 100 - map(right_poten_value, right_poten_range.lower_bound, right_poten_range.upper_bound, 0, 100);

	char buf[64];
	snprintf(buf, sizeof(buf), "[%d, %d] [%d, %d]\n", left_poten_value, right_poten_value, left_sensor_percentage, right_sensor_percentage);
	Serial.println(buf);
}
#if 0
	// maps analog values of Sensor1 and Sensor2 from 0,100

	Serial.print("Sensor 1: ");
	Serial.print(left_sensor_percentage);

	Serial.print(" Sensor 2: ");
	Serial.println(right_sensor_percentage);

	sensorDifference = left_sensor_percentage - right_sensor_percentage;

	differenceAverage = (left_sensor_percentage + right_sensor_percentage) / 2;
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
	} else {
		digitalWrite(RedLED, LOW);
	}

	if (sensorValue > 1020 || sensorValue2 > 878) {
		digitalWrite(GreenLED, HIGH);
	}else{
		digitalWrite(GreenLED, LOW);
	}
#endif
