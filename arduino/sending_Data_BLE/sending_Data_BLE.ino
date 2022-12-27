#include "Arduino.h"
#include "ArduinoBLE.h"
#include "Nano33BLEAccelerometer.h"
#include "Nano33BLEGyroscope.h"
#include "Nano33BLETemperature.h"
#include "string.h"
#include "NRF52_MBED_TimerInterrupt.h"
#include "NRF52_MBED_ISR_Timer.h"

#define CIRCULAR_BUFFER_INT_SAFE
#include <C:\Users\Matteo\Documents\Arduino\libraries\CircularBuffer\CircularBuffer.h>
CircularBuffer<Nano33BLEAccelerometerData, 3000> accBuffer;
CircularBuffer<Nano33BLEGyroscopeData, 3000> gyroBuffer;
CircularBuffer<Nano33BLETemperatureData, 3000> tempBuffer;
CircularBuffer<unsigned long, 3000> timeBuffer;

#define HW_TIMER_INTERVAL_MS          1
#define TIMER_INTERVAL              15L

Nano33BLEAccelerometerData accelerometerData;
Nano33BLEGyroscopeData gyroscopeData;
Nano33BLETemperatureData temperatureData;

BLEService accGyroTempHumiService("e2e65ffc-5687-4cbe-8f2d-db76265f269f");
BLEStringCharacteristic sensorCharacteristic("3000", BLERead | BLENotify, 150);
BLEDevice central;

char token[150] = "";
char accstr[50] = "";
char gyrostr[50] = "";
char tempstr[10] = "";
char timestr[40] = "";

NRF52_MBED_Timer ITimer(NRF_TIMER_3);
NRF52_MBED_ISRTimer ISR_Timer;

bool mutex= false;
unsigned long currentMillis = 0;

void TimerHandler()
{
  ISR_Timer.run();
}

void setup() {
  Accelerometer.begin();
  Gyroscope.begin();
  Temperature.begin();

  if(!BLE.begin()){
    while(true);
  }

  BLE.setLocalName("Activity Tracker");
  BLE.setAdvertisedService(accGyroTempHumiService);
  accGyroTempHumiService.addCharacteristic(sensorCharacteristic);
  BLE.addService(accGyroTempHumiService);
  BLE.advertise();

  ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS * 1000, TimerHandler);
  ISR_Timer.setInterval(TIMER_INTERVAL,  updateSensors);
}

void loop() {
  central = BLE.central();

  if(!mutex && !timeBuffer.isEmpty()){
    mutex = true;
    if(!accBuffer.isEmpty())
      accelerometerData = accBuffer.pop();
    if(!accBuffer.isEmpty())
      gyroscopeData = gyroBuffer.pop();
    if(!tempBuffer.isEmpty())
      temperatureData = tempBuffer.pop();
    currentMillis = timeBuffer.pop();
    mutex = false;

    sprintf(token,"");

    sprintf(accstr, "%.3f,%.3f,%.3f,", accelerometerData.x, accelerometerData.y, accelerometerData.z);
    sprintf(gyrostr, "%.3f,%.3f,%.3f,", gyroscopeData.x, gyroscopeData.y, gyroscopeData.z);
    sprintf(tempstr, "%.f,%.f,", temperatureData.temperatureCelsius, temperatureData.humidity);

    sprintf(timestr, "%i", currentMillis);

    strcat(token, accstr);
    strcat(token, gyrostr);
    strcat(token, tempstr);
    strcat(token, timestr);

    if(central)
      sensorCharacteristic.writeValue(token);
  }
}

void updateSensors() {
  if(!mutex){
    mutex = true;
    Nano33BLEAccelerometerData accelerometerData1;
    Nano33BLEGyroscopeData gyroscopeData1;
    Nano33BLETemperatureData temperatureData1;

    if(Accelerometer.pop(accelerometerData1))
      accBuffer.unshift(accelerometerData1);

    if(Gyroscope.pop(gyroscopeData1))
      gyroBuffer.unshift(gyroscopeData1);

    if(Temperature.pop(temperatureData1))
      tempBuffer.unshift(temperatureData1);

    timeBuffer.unshift(millis());
    mutex = false;
  }
}
