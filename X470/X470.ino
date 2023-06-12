
#include "default_config.h"
#include "params.h"
#include <pwmWrite.h>         //  https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite
#include "sbus.h"             //  https://github.com/bolderflight/sbus
#include <IBusBM.h>           //  https://github.com/bmellink/IBusBM
#include <ESP32AnalogRead.h>  //  https://github.com/madhephaestus/ESP32AnalogRead

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebSrv.h>
#include <WebSerialLite.h>
#include <AsyncElegantOTA.h>

#define IBUS
//-------------------------DEBUGGING--------------------------------------------------
#define SERVO_DEBUG  //  Shows Servo & ESC PWM outputs
//#define RX_DEBUG  //  Shows RX channels from Receiver
//#define SENSOR_DEBUG    //  Shows Hall Sensor and Voltage Values
//#define ADVANCED_DEBUG  // Show Modes and Valve Controls

Pwm pwm = Pwm();  // pwm setup for ESC & Servo
TaskHandle_t coreTask1;
TaskHandle_t coreTask2;

void setup() {
  Serial.begin(115200);

  xTaskCreatePinnedToCore(core1Task, "Core 1 Task", 10000, NULL, 1, &coreTask1, 0);
  xTaskCreatePinnedToCore(core2Task, "Core 2 Task", 10000, NULL, 1, &coreTask2, 1);
}

void core1Task(void* parameter) {
  WifiSetup();
  for (;;) {
    //Repeat(blinkled, 500);
    Repeat(webupdate, 2000);
    delay(1000);
  }
}

void core2Task(void* parameter) {
  RXsetup();
  PwmInit();  // Function to Start Servo in Safe Position
  for (;;) {
    ReadRX();
    PumpControl();
    HydraulicControl();
    MotorControl();
    PwmOutput();
    delay(50);
  }
}

void loop() {
}