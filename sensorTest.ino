
#include "sensors/testcounter.hpp"
#include "sensors/sensorManager.hpp"

const int time_per_reading = 100;
const int readings = 10;

const double arm_length = 142.5 / 1000.0;  //  meters

int currentReadings = 0;
long reading = 0;

Sensor::CounterSensor counter1;
Sensor::CounterSensor counter2;
int sensorCount = 2;


Sensor::SensorManager manager(sensorCount, time_per_reading * readings);

void setup() {
    Serial.begin(57600);

    counter1.setReadRate(1000);
    counter2.setReadRate(2000);
    manager.addSensor(&counter1);
    manager.addSensor(&counter2);
    manager.setReadCallback(&readCallback);
}

void loop() {
    manager.spin(5000);
    Serial.println(manager.getLastRead(&counter1));
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
