#define CIRCULAR_BUFFER_INT_SAFE 
#define BUFFER_SIZE 3000 //max size of the circular buffer

#include "Arduino.h"
#include "ArduinoBLE.h"
#include "Nano33BLEAccelerometer.h"
#include "Nano33BLEGyroscope.h"
#include "Nano33BLETemperature.h"
#include "string.h"
#include "NRF52_MBED_TimerInterrupt.h"
#include "NRF52_MBED_ISR_Timer.h"
#include <C:\Users\Davide\Documents\Arduino\libraries\CircularBuffer\CircularBuffer.h>
//#include <C:\Users\Matteo\Documents\Arduino\libraries\CircularBuffer\CircularBuffer.h>

CircularBuffer<Nano33BLEAccelerometerData, BUFFER_SIZE> accBuffer; //buffer for accelerometer data
CircularBuffer<Nano33BLEGyroscopeData, BUFFER_SIZE> gyroBuffer; //buffer for gyroscope data
CircularBuffer<Nano33BLETemperatureData, BUFFER_SIZE> tempBuffer; //buffer for temperature and humidity data
CircularBuffer<unsigned long, BUFFER_SIZE> timeBuffer; //buffer for timestamp data

#define HW_TIMER_INTERVAL_MS          1
#define TIMER_INTERVAL              15L //data acquisition rate

Nano33BLEAccelerometerData accelerometerData;
Nano33BLEGyroscopeData gyroscopeData;
Nano33BLETemperatureData temperatureData;

BLEService accGyroTempHumiService("e2e65ffc-5687-4cbe-8f2d-db76265f269f"); //BLE service
BLEStringCharacteristic sensorCharacteristic("3000", BLERead | BLENotify, 150); //BLE characteristic
bool connected = false; // central connected

char token[150] = "";
char accstr[50] = "";
char gyrostr[50] = "";
char tempstr[10] = "";
char timestr[40] = "";

NRF52_MBED_Timer ITimer(NRF_TIMER_3);
NRF52_MBED_ISRTimer ISR_Timer;

bool mutex= false;
unsigned long currentMillis = 0;

void setup() {
  Serial.begin(115200);
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
  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS * 1000, TimerHandler);
  ISR_Timer.setInterval(TIMER_INTERVAL,  updateSensors);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  BLE.poll(); //poll for Bluetooth® Low Energy radio events and handle them.
  Serial.println(timeBuffer.size());
  if(!mutex && !timeBuffer.isEmpty()){
    mutex = true;
    if(!accBuffer.isEmpty())
      accelerometerData = accBuffer.pop();
    if(!gyroBuffer.isEmpty())
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

    if(connected)
      sensorCharacteristic.writeValue(token); //send data to central connected
  }
  analogWrite(LED_BUILTIN, map(accBuffer.size(), 0, BUFFER_SIZE, 0, 255)); //visual feedback of the size of the buffer
}

void updateSensors() {
  //acquire data from sensors every TIMER_INTERVAL
  if(!mutex){
    mutex = true;
    Nano33BLEAccelerometerData accelerometerData_temp;
    Nano33BLEGyroscopeData gyroscopeData_temp;
    Nano33BLETemperatureData temperatureData_temp;

    if(Accelerometer.pop(accelerometerData_temp))
      accBuffer.unshift(accelerometerData_temp);
    else
      accBuffer.unshift(accelerometerData);

    if(Gyroscope.pop(gyroscopeData_temp))
      gyroBuffer.unshift(gyroscopeData_temp);
    else
      gyroBuffer.unshift(gyroscopeData);

    if(Temperature.pop(temperatureData_temp))
      tempBuffer.unshift(temperatureData_temp);
    else
      tempBuffer.unshift(temperatureData);

    timeBuffer.unshift(millis());
    mutex = false;
  }
}

void blePeripheralConnectHandler( BLEDevice central )
{
  //central is connected
  connected = true;
}


void blePeripheralDisconnectHandler( BLEDevice central )
{
  //central is disconnected
  connected = false;
}

void TimerHandler()
{
  //start ISR interrupts
  ISR_Timer.run();
}
