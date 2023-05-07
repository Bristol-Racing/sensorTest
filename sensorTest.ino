
#include "sensors/testcounter.hpp"
#include "sensors/sensorManager.hpp"

const int time_per_reading = 100;
const int readings = 10;

const double arm_length = 142.5 / 1000.0;  //  meters

int currentReadings = 0;
long reading = 0;

int sensorCount = 1;
Sensor::CounterSensor counter;


Sensor::SensorManager manager(sensorCount, time_per_reading * readings);

void setup() {
    Serial.begin(57600);

    manager.addSensor(&counter);
    manager.setReadCallback(&readCallback);
}

void loop() {
    manager.spin();
}

void readCallback(double * results) {
    for (int i = 0; i < sensorCount; i++) {
        if (i > 0) {
            Serial.print(",");
        }
        Serial.print(results[i]);
    }
    Serial.println();
}
