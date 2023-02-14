#define CIRCULAR_BUFFER_INT_SAFE

#include <arduino_activity_classification_inferencing.h>
#include <Arduino_LSM9DS1.h>
//#include <C:\Users\Davide\Documents\Arduino\libraries\CircularBuffer\CircularBuffer.h>
#include <C:\Users\Matteo\Documents\Arduino\libraries\CircularBuffer\CircularBuffer.h>
//#include <C:\Users\Martina\Documents\Arduino\libraries\CircularBuffer\CircularBuffer.h>
#include <ArduinoBLE.h>

/*setting parameter for inference module*/
static bool debug_nn = false;
static uint16_t run_inference_every_ms = 1000;
unsigned long time_passed;
uint8_t prediction_int; //result of prediction

static float inference_buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];
CircularBuffer<float, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE> circular_buffer;

BLEService prediction_service("e2e65ffc-5687-4cbe-8f2d-db76265f269a");
BLEUnsignedCharCharacteristic prediction_characteristic("3000", BLERead | BLENotify);

bool connected = false; //true if a central is connected
bool send_BLE = false; //true if data available to be sent
bool buffer_full = false; //true if buffer become full for first time

static rtos::Thread dataread_thread(osPriorityRealtime);
static rtos::Thread BLE_thread(osPriorityBelowNormal);
osThreadId_t main_thread_id; //id of loop main

void get_IMU_data();
void update_BLE();

void setup() {
  Serial.begin(115200);
  
  delay(5000);
  if (!IMU.begin()) {
    ei_printf("Failed to initialize IMU!\r\n");
  } else {
    ei_printf("IMU initialized\r\n");
  }
  if (EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME != 5) {
    ei_printf("ERR: EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME should be equal to 5 (3 acc + 2 gyro)\n");
    return;
  }

  if(!BLE.begin()){
    while(true);
  }

  /*BLE setup*/
  BLE.setLocalName("Activity Classificator");
  BLE.setAdvertisedService(prediction_service);
  prediction_service.addCharacteristic(prediction_characteristic);
  BLE.addService(prediction_service);
  BLE.advertise();
  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  /*start threads*/
  dataread_thread.start(mbed::callback(&get_IMU_data));
  BLE_thread.start(mbed::callback(&update_BLE));
}

void loop() {
  main_thread_id = osThreadGetId();

  // wait until we have a full buffer
  rtos::Thread::signal_wait(0x1);

  // This is a structure that smoothens the output result
  // With the default settings 70% of readings should be the same before classifying.
  ei_classifier_smooth_t smooth;
  ei_classifier_smooth_init(&smooth, 10 /* no. of readings */, 7 /* min. readings the same */, 0.8 /* min. confidence */, 0.3 /* max anomaly */);

  while (1) {
    if(circular_buffer.size() == 0){
      break;
    }

    for(unsigned short i = 0; i < circular_buffer.size(); i++){
      inference_buffer[i] = circular_buffer[i];
    }

    // Turn the raw buffer in a signal which we can the classify
    signal_t signal;
    int err = numpy::signal_from_buffer(inference_buffer, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
    if (err != 0) {
      ei_printf("Failed to create signal from buffer (%d)\n", err);
      return;
    }

    // Run the classifier
    ei_impulse_result_t result = { 0 };

    err = run_classifier(&signal, &result, debug_nn);
    if (err != EI_IMPULSE_OK) {
      ei_printf("ERR: Failed to run classifier (%d)\n", err);
      return;
    }
  
    // print the predictions
    ei_printf("Predictions ");
    ei_printf("(DSP: %d ms., Classification: %d ms., Anomaly: %d ms.)",
              result.timing.dsp, result.timing.classification, result.timing.anomaly);
    ei_printf(": ");

    const char* prediction = ei_classifier_smooth_update(&smooth, &result, prediction_int);
    send_BLE = true;

    ei_printf("%s ", prediction);
    ei_printf("%d ", prediction_int);
    // print the cumulative results
    ei_printf(" [ ");
    for (size_t ix = 0; ix < smooth.count_size; ix++) {
      ei_printf("%u", smooth.count[ix]);
      if (ix != smooth.count_size + 1) {
        ei_printf(", ");
      } else {
        ei_printf(" ");
      }
    }
    ei_printf("]\n");
   
    rtos::Thread::wait(run_inference_every_ms);
  }
  ei_classifier_smooth_free(&smooth);
}

void get_IMU_data() {
  float acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z;

  while (true) {
    if(connected){
      time_passed = millis();

      if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(acc_x,acc_y,acc_z);
      } 
      if (IMU.gyroscopeAvailable()) {
        IMU.readGyroscope(gyro_x,gyro_y,gyro_z);
      }

      circular_buffer.push(acc_x);
      circular_buffer.push(acc_y);
      circular_buffer.push(acc_z);
      circular_buffer.push(gyro_y);
      circular_buffer.push(gyro_z);

      if (!buffer_full && circular_buffer.size() == EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE){
        osSignalSet(main_thread_id, 0x1); //signal buffer is full
        buffer_full = true;
      }

      time_passed = millis() - time_passed;

      if(time_passed <0) //handle ulong overflow situation
        time_passed = 0 - time_passed;

      if(time_passed > EI_CLASSIFIER_INTERVAL_MS) //handle error situation
        time_passed = EI_CLASSIFIER_INTERVAL_MS;

      rtos::Thread::wait(EI_CLASSIFIER_INTERVAL_MS - time_passed);
    }
    else{
      rtos::Thread::wait(10);
    }
  }
}

void update_BLE(){
  while(1){
    BLE.poll(); //poll for Bluetooth® Low Energy radio events and handle them.

    if(connected && send_BLE){
      prediction_characteristic.writeValue(prediction_int);
      send_BLE = false;
    }
    rtos::Thread::wait(10);
  }
}

void blePeripheralConnectHandler( BLEDevice central )
{
  //central is connected
  connected = true;
  prediction_characteristic.writeValue(5); //inizialize service
}


void blePeripheralDisconnectHandler( BLEDevice central )
{
  //central is disconnected
  connected = false;
  buffer_full = false;
  circular_buffer.clear();
}

#if !defined(EI_CLASSIFIER_SENSOR) || EI_CLASSIFIER_SENSOR != EI_CLASSIFIER_SENSOR_FUSION
#error "Invalid model for current sensor"
#endif
