#include "Arduino.h"
#include "ArduinoBLE.h"
#include "Nano33BLEAccelerometer.h"
#include "Nano33BLEGyroscope.h"
#include "Nano33BLETemperature.h"

Nano33BLEAccelerometerData accelerometerData;
Nano33BLEGyroscopeData gyroscopeData;
Nano33BLETemperatureData temperatureData;

BLEService accGyroTempHumiService("2A00");
BLEUnsignedCharCharacteristic accelerometerXYZ("2A00", BLERead | BLENotify);
BLEUnsignedCharCharacteristic gyroscopeXYZ("2A00", BLERead | BLENotify);
BLEUnsignedCharCharacteristic temperatureHumidity("2A00", BLERead | BLENotify);

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
    sprintf(updateAccXYZ, "%.3f,%.3f,%.3f", accelerometerData.x, accelerometerData.y, accelerometerData.z);
    accelerometerXYZ.writeValue(updateAccXYZ);
    Serial.print("Accelerometer: ");
    Serial.println(accelerometerXYZ);
  }
}

void updateGyrXYZ(){
  if (Gyroscope.pop(gyroscopeData)) {
    sprintf(updateGyrXYZ, "%.3f,%.3f,%.3f", gyroscopeData.x, gyroscopeData.y, gyroscopeData.z);
    gyroscopeXYZ.writeValue(updateGyrXYZ);
    Serial.print("Gyroscope: ");
    Serial.println(gyroscopeXYZ);
  }
}

void updateTemHum(){
  if (Temperature.pop(temperatureData)) {
    sprintf(updateTemHum, "%.3f,%.3f", temperatureData.temperatureCelsius, temperatureData.humidity);
    temperatureHumidity.writeValue(updateTemHum);
    Serial.print("Temperature: ");
    Serial.println(temperatureHumidity);
  }
}
