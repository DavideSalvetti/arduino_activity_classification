#include "Arduino.h"
#include "ArduinoBLE.h"
#include "Nano33BLEAccelerometer.h"
#include "Nano33BLEGyroscope.h"
#include "Nano33BLETemperature.h"

Nano33BLEAccelerometerData accelerometerData;
Nano33BLEGyroscopeData gyroscopeData;
Nano33BLETemperatureData temperatureData;

BLEService accGyroTempHumiService("e2e65ffc-5687-4cbe-8f2d-db76265f269f");
BLEStringCharacteristic sensorCharacteristic("3000", BLERead | BLENotify, 150);

unsigned long previousMillis = 0;
unsigned long incrementalVal = 0;

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
  accGyroTempHumiService.addCharacteristic(sensorCharacteristic);
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

      if(currentMillis - previousMillis >= 10){
        previousMillis = currentMillis;
        incrementalVal++;

        updateSensors();
      }
    }

    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}

/*  Funzione per l'aggiornamento dei valori della caratteristica. 
 *  Aggiorna la caratteristica con i valori di accelerometro, giroscopio, temperatura e umidità.
 *  Per verificare il timing, inserisce un valore incrementale alla fine.
 */
void updateSensors() {

  char data[150];
  memset(data, 0, sizeof(data));
  if (Accelerometer.pop(accelerometerData)) {
    sprintf(data, "%.3f,%.3f,%.3f,", accelerometerData.x, accelerometerData.y, accelerometerData.z);
  }

  int len = strlen(data);

  if (Gyroscope.pop(gyroscopeData)) {
    sprintf(&data[len], "%.3f,%.3f,%.3f,", gyroscopeData.x, gyroscopeData.y, gyroscopeData.z);
  }

  len = strlen(data);

  if (Temperature.pop(temperatureData)) {
    sprintf(&data[len], "%.3f,%.3f,", temperatureData.temperatureCelsius, temperatureData.humidity);
  }

  len = strlen(data);

  sprintf(&data[len], "%i", incrementalVal);

  sensorCharacteristic.writeValue(data);

}
