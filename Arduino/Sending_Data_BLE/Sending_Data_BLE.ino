#include "Arduino.h"
#include "ArduinoBLE.h"
#include "Nano33BLEAccelerometer.h"
#include "Nano33BLEGyroscope.h"
#include "Nano33BLETemperature.h"
#include "string.h"
//#include "Timer.h"

//Timer t;

Nano33BLEAccelerometerData accelerometerData;
Nano33BLEGyroscopeData gyroscopeData;
Nano33BLETemperatureData temperatureData;

BLEService accGyroTempHumiService("e2e65ffc-5687-4cbe-8f2d-db76265f269f");
BLEStringCharacteristic sensorCharacteristic("3000", BLERead | BLENotify, 150);

unsigned long previousMillis = 0;

char token[150] = "";
char accstr[50] = "";
char gyrostr[50] = "";
char tempstr[10] = "";
char timestr[40] = "";


void setup() {
  Serial.begin(115200);

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

    int num_timer;

    while(central.connect()){
      unsigned long currentMillis = millis();

      //num_timer=t.every(1000,updateSensors);
     if(currentMillis - previousMillis >= 10){
        previousMillis = currentMillis;

        updateSensors();
      }
    }

    //t.stop(num_timer);

    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }

  //t.update();
}

/*  Funzione per l'aggiornamento dei valori della caratteristica. 
 *  Aggiorna la caratteristica con i valori di accelerometro, giroscopio, temperatura e umidità.
 *  Per verificare il timing, inserisce un valore incrementale alla fine.
 */
void updateSensors() {
  sprintf(token,"");

  if (Accelerometer.pop(accelerometerData)) {
    sprintf(accstr, "%.3f,%.3f,%.3f,", accelerometerData.x, accelerometerData.y, accelerometerData.z);
  }

  if (Gyroscope.pop(gyroscopeData)) {
    sprintf(gyrostr, "%.3f,%.3f,%.3f,", gyroscopeData.x, gyroscopeData.y, gyroscopeData.z);
  }

  if (Temperature.pop(temperatureData)) {
    sprintf(tempstr, "%.f,%.f,", temperatureData.temperatureCelsius, temperatureData.humidity);
  }

  sprintf(timestr, "%i", previousMillis);

  strcat(token, accstr);
  strcat(token, gyrostr);
  strcat(token, tempstr);
  strcat(token, timestr);

  //Serial.println(token);

  sensorCharacteristic.writeValue(token);
}
