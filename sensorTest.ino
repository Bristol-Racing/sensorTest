
#include "sensors/testcounter.hpp"
#include "sensors/throttle.hpp"
#include "sensors/sensorManager.hpp"

#define THROTTLE_PIN A0

const double arm_length = 142.5 / 1000.0;  //  meters

int currentReadings = 0;
long reading = 0;

Sensor::CounterSensor counter1;
Sensor::CounterSensor counter2;
Sensor::Throttle throttle(THROTTLE_PIN, &throttleCallback);
Sensor::CPUMonitor* monitor;
int sensorCount = 4;

const int time_per_reading = 100;
const int readings = 10;

Sensor::SensorManager manager(sensorCount, time_per_reading * readings);

void setup() {
    Serial.begin(57600);

    counter1.setReadRate(1000);
    manager.addSensor(&counter1);

    counter2.setReadRate(2000);
    manager.addSensor(&counter2);

    throttle.setTickRate(10);
    throttle.setReadRate(50);
    manager.addSensor(&throttle);

    monitor = manager.getMonitor();
    manager.addSensor(monitor);

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

void throttleCallback(double voltage) {
    // if (isnan(voltage)) {
        Serial.println(voltage);
    // }
}
