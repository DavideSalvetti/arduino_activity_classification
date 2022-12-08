#include "Arduino.h"
#include "ArduinoBLE.h"
#include "Nano33BLEAccelerometer.h"
#include "Nano33BLEGyroscope.h"
#include "Nano33BLETemperature.h"

Nano33BLEAccelerometerData accelerometerData;
Nano33BLEGyroscopeData gyroscopeData;
Nano33BLETemperatureData temperatureData;

BLEService accGyroTempHumiService("2A00");
BLEStringCharacteristic accelerometerXYZ("3000", BLERead | BLENotify, 30);
BLEStringCharacteristic gyroscopeXYZ("3001", BLERead | BLENotify, 30);
BLEStringCharacteristic temperatureHumidity("3002", BLERead | BLENotify, 30);

unsigned long previousMillis = 0;

void setup() {
  Serial.begin(9600);
  while(!Serial);

  Accelerometer.begin();
  Gyroscope.begin();
  Temperature.begin();

  pinMode(LED_BUILTIN, OUTPUT);

  if(!BLE.begin()){
    Serial.println("starting BLE failed!");

    while(true);
  }

  BLE.setLocalName("Activity Tracker");
  BLE.setAdvertisedService(accGyroTempHumiService);
  accGyroTempHumiService.addCharacteristic(accelerometerXYZ);
  accGyroTempHumiService.addCharacteristic(gyroscopeXYZ);
  accGyroTempHumiService.addCharacteristic(temperatureHumidity);
  BLE.addService(accGyroTempHumiService);

  BLE.advertise();

  Serial.println("Bluetooth® device active, waiting for connections...");
}

void loop() {
  BLEDevice central = BLE.central();

  if(central){
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    digitalWrite(LED_BUILTIN, HIGH);

    while(central.connect()){
      unsigned long currentMillis = millis();

      if(currentMillis - previousMillis >= 200){
        previousMillis = currentMillis;
        updateAccXYZ();
        updateGyrXYZ();
        updateTemHum();
      }
    }

    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}

void updateAccXYZ(){
  if (Accelerometer.pop(accelerometerData)) {
    char data[30];
    memset(data, 0, sizeof(data));
    sprintf(data, "%.3f,%.3f,%.3f", accelerometerData.x, accelerometerData.y, accelerometerData.z);
    accelerometerXYZ.writeValue(data);
    Serial.print("Accelerometer: ");
    Serial.println(accelerometerXYZ);
  }
}

void updateGyrXYZ(){
  if (Gyroscope.pop(gyroscopeData)) {
    char data[30];
    memset(data, 0, sizeof(data));
    sprintf(data, "%.3f,%.3f,%.3f", gyroscopeData.x, gyroscopeData.y, gyroscopeData.z);
    gyroscopeXYZ.writeValue(data);
    Serial.print("Gyroscope: ");
    Serial.println(gyroscopeXYZ);
  }
}

void updateTemHum(){
  if (Temperature.pop(temperatureData)) {
    char data[30];
    memset(data, 0, sizeof(data));
    sprintf(data, "%.3f,%.3f", temperatureData.temperatureCelsius, temperatureData.humidity);
    temperatureHumidity.writeValue(data);
    Serial.print("Temperature: ");
    Serial.println(temperatureHumidity);
  }
}
