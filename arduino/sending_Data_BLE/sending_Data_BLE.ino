#include "Arduino.h"
#include "ArduinoBLE.h"
#include "Nano33BLEAccelerometer.h"
#include "Nano33BLEGyroscope.h"
#include "Nano33BLETemperature.h"
#include "string.h"

#define CIRCULAR_BUFFER_INT_SAFE
//#include <C:\Users\Davide\Documents\Arduino\libraries\CircularBuffer\CircularBuffer.h>
#include <C:\Users\Matteo\Documents\Arduino\libraries\CircularBuffer\CircularBuffer.h>
CircularBuffer<Nano33BLEAccelerometerData, 300> accBuffer;
CircularBuffer<Nano33BLEGyroscopeData, 300> gyroBuffer;
CircularBuffer<Nano33BLETemperatureData, 300> tempBuffer;
CircularBuffer<unsigned long, 300> timeBuffer;

Nano33BLEAccelerometerData accelerometerData;
Nano33BLEGyroscopeData gyroscopeData;
Nano33BLETemperatureData temperatureData;

BLEService accGyroTempHumiService("e2e65ffc-5687-4cbe-8f2d-db76265f269f");
BLEStringCharacteristic sensorCharacteristic("3000", BLERead | BLENotify, 150);
bool connected = false;

char token[150] = "";
char accstr[50] = "";
char gyrostr[50] = "";
char tempstr[10] = "";
char timestr[40] = "";

rtos::Mutex buffer_mutex;
rtos::Mutex token_mutex;
osThreadId_t main_thread_id;
bool first_update = true;

unsigned long currentMillis = 0;

static rtos::Thread Data_thread(osPriorityRealtime);
static rtos::Thread BLE_thread(osPriorityNormal);
static rtos::Thread BLE_poll_thread(osPriorityNormal);

void updateSensors();
void writeBLECharacteristic();
void BLEPool();

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

  BLE.setEventHandler( BLEConnected, blePeripheralConnectHandler );
  BLE.setEventHandler( BLEDisconnected, blePeripheralDisconnectHandler );

  pinMode(LED_BUILTIN, OUTPUT);

  delay(5000);
}

void loop() {
  main_thread_id = osThreadGetId();

  Data_thread.start(mbed::callback(&updateSensors));
  BLE_poll_thread.start(mbed::callback(&BLEPool));
  BLE_thread.start(mbed::callback(&writeBLECharacteristic));

  rtos::Thread::signal_wait(0x1);

  while(1){
    if(!timeBuffer.isEmpty()){
      if(token_mutex.trylock()){
        if(buffer_mutex.trylock()){
          if(!accBuffer.isEmpty())
            accelerometerData = accBuffer.pop();
          if(!gyroBuffer.isEmpty())
            gyroscopeData = gyroBuffer.pop();
          if(!tempBuffer.isEmpty())
            temperatureData = tempBuffer.pop();
          currentMillis = timeBuffer.pop();
          
          osSignalSet(BLE_thread.get_id(), 0x2);

          buffer_mutex.unlock();
        }
        token_mutex.unlock();
      }
    }
    analogWrite(LED_BUILTIN, map(accBuffer.size(),0,300,0,255));
  }
}

void BLEPool(){
  while(1){
    BLE.poll(10);
  }
}

void blePeripheralConnectHandler( BLEDevice central )
{
  delay(1000);
  connected = true;
}


void blePeripheralDisconnectHandler( BLEDevice central )
{
  connected = false;
}

void writeBLECharacteristic(){
    while(1){
    rtos::Thread::signal_wait(0x2);
    token_mutex.lock();

    sprintf(token,"");

    sprintf(accstr, "%.3f,%.3f,%.3f,", accelerometerData.x, accelerometerData.y, accelerometerData.z);
    sprintf(gyrostr, "%.3f,%.3f,%.3f,", gyroscopeData.x, gyroscopeData.y, gyroscopeData.z);
    sprintf(tempstr, "%.f,%.f,", temperatureData.temperatureCelsius, temperatureData.humidity);

    sprintf(timestr, "%i", currentMillis);

    strcat(token, accstr);
    strcat(token, gyrostr);
    strcat(token, tempstr);
    strcat(token, timestr);
    
    if (connected){
      sensorCharacteristic.writeValue(token);
    }

    token_mutex.unlock();
  }
}

void updateSensors() {
  while(true){
    if(buffer_mutex.trylock()){
      Serial.println(millis());
      if(first_update){
        osSignalSet(main_thread_id, 0x1);
        first_update = false;
      }

      Nano33BLEAccelerometerData accelerometerData1;
      Nano33BLEGyroscopeData gyroscopeData1;
      Nano33BLETemperatureData temperatureData1;

      if(Accelerometer.pop(accelerometerData1))
        accBuffer.unshift(accelerometerData1);
      else
        accBuffer.unshift(accelerometerData);

      if(Gyroscope.pop(gyroscopeData1))
        gyroBuffer.unshift(gyroscopeData1);
      else
        gyroBuffer.unshift(gyroscopeData);

      if(Temperature.pop(temperatureData1))
        tempBuffer.unshift(temperatureData1);
      else
        tempBuffer.unshift(temperatureData);

      timeBuffer.unshift(millis());

      buffer_mutex.unlock();
    }

    rtos::Thread::wait(15);
  }
}
