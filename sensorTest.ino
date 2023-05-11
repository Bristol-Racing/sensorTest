
#include <SPI.h>
#include <RH_RF95.h>
#include <SD.h>

#include "sensors/check.hpp"
#include "sensors/testcounter.hpp"
#include "sensors/throttle.hpp"
#include "sensors/sensorManager.hpp"

#define THROTTLE_PIN A0

#define RFM95_CS  4
#define RFM95_INT 3
#define RFM95_RST 2

#define RF95_FREQ 915.0

RH_RF95 rf95(RFM95_CS, RFM95_INT);

#define SD_CS 5

const char filename[] = "log.txt";

// File object to represent file
// File txtFile;

// string to buffer output
// String buffer;

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
    pinMode(RFM95_RST, OUTPUT);
    digitalWrite(RFM95_RST, HIGH);

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

    // buffer.reserve(1024);

    // bool sdStatus = SD.begin(SD_CS);
    // CHECK(sdStatus == true, "SD initialisation failed.");

    // txtFile = SD.open(filename, FILE_WRITE);
    // CHECK(txtFile, "Error opening log file.");

    // txtFile.println("AAAAA");

    // Serial.println("AAAAA");

    delay(100);
    digitalWrite(RFM95_RST, LOW);
    delay(10);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);

    bool initStatus = rf95.init();
    CHECK(initStatus == true, "LoRa initialisation failed.");

    bool freqStatus = rf95.setFrequency(RF95_FREQ);
    CHECK(freqStatus == true, "LoRa frequency set failed.");

    rf95.setTxPower(23, false);
}

void loop() {
    Serial.println("B");
    manager.spin(5000);
    Serial.println(manager.getLastRead(&counter1));
}

void readCallback(double * results) {
    rf95.send((uint8_t*)results, sensorCount * sizeof(double));

    for (int i = 0; i < sensorCount; i++) {
        if (i > 0) {
            Serial.print(",");
            // txtFile.print(",");
        }
        Serial.print(results[i]);
        // txtFile.print(results[i]);
    }
    Serial.println();
    // txtFile.println();

    rf95.waitPacketSent();
}

void throttleCallback(double voltage) {
    // if (isnan(voltage)) {
        // Serial.println(voltage);
    // }
}
