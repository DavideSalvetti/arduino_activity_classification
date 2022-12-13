#include "Arduino.h"
#include "ArduinoBLE.h"
#include "Nano33BLEAccelerometer.h"
#include "Nano33BLEGyroscope.h"
#include "Nano33BLETemperature.h"
#include "string.h"

Nano33BLEAccelerometerData accelerometerData;
Nano33BLEGyroscopeData gyroscopeData;
Nano33BLETemperatureData temperatureData;

BLEService accGyroTempHumiService("e2e65ffc-5687-4cbe-8f2d-db76265f269f");
BLEStringCharacteristic sensorCharacteristic("3000", BLERead | BLENotify, 150);

unsigned long currentMillis = 0;

char token[150] = "";
char accstr[50] = "";
char gyrostr[50] = "";
char tempstr[10] = "";
char timestr[40] = "";

BLEDevice central;

#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0
#include "NRF52_MBED_TimerInterrupt.h"
#include "NRF52_MBED_ISR_Timer.h"
#define HW_TIMER_INTERVAL_MS      1
NRF52_MBED_Timer ITimer(NRF_TIMER_1);
NRF52_MBED_ISRTimer ISR_Timer;
#define TIMER_INTERVAL_1S             10L

bool mutex= false;

void TimerHandler()
{
  ISR_Timer.run();
}

void setup() {
  Accelerometer.begin();
  Gyroscope.begin();
  Temperature.begin();

  pinMode(LED_BUILTIN, OUTPUT);

  if(!BLE.begin()){
    while(true);
  }

  BLE.setLocalName("Activity Tracker");
  BLE.setAdvertisedService(accGyroTempHumiService);
  accGyroTempHumiService.addCharacteristic(sensorCharacteristic);
  BLE.addService(accGyroTempHumiService);

  BLE.advertise();

  Serial.begin(115200);

  ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS * 1000, TimerHandler);
  ISR_Timer.setInterval(TIMER_INTERVAL_1S,  updateSensors);
}

void loop() {
  central = BLE.central();

  if(mutex){

    sprintf(token,"");

    sprintf(accstr, "%.3f,%.3f,%.3f,", accelerometerData.x, accelerometerData.y, accelerometerData.z);
    sprintf(gyrostr, "%.3f,%.3f,%.3f,", gyroscopeData.x, gyroscopeData.y, gyroscopeData.z);
    sprintf(tempstr, "%.f,%.f,", temperatureData.temperatureCelsius, temperatureData.humidity);

    sprintf(timestr, "%i", currentMillis);

    strcat(token, accstr);
    strcat(token, gyrostr);
    strcat(token, tempstr);
    strcat(token, timestr);

    Serial.println(token);

    if(central){
     sensorCharacteristic.writeValue(token);
    }
    mutex = false;
  }
}

void updateSensors() {
  if(!mutex){
    mutex = true;

    Accelerometer.pop(accelerometerData);
    Gyroscope.pop(gyroscopeData);
    Temperature.pop(temperatureData);

    currentMillis = millis();
  }
}
