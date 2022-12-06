#include<ArduinoBLE.h>

BLEService accelerometerService("2A00");
BLEUnsignedCharCharacteristic accelerometerX("2A00", BLERead | BLENotify);

unsigned long previousMillis = 0;
int accX = 100;

void setup() {
  Serial.begin(9600);
  while(!Serial);

  pinMode(LED_BUILTIN, OUTPUT);

  if(!BLE.begin()){
    Serial.println("starting BLE failed!");

    while(true);
  }

  BLE.setLocalName("Activity Tracker");
  BLE.setAdvertisedService(accelerometerService);
  accelerometerService.addCharacteristic(accelerometerX);
  BLE.addService(accelerometerService);

  accelerometerX.writeValue(accX);

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
        updateAccX();
      }
    }

    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}

void updateAccX(){
  accelerometerX.writeValue(++accX);
  Serial.println(accX);
}