#include <Arduino_FreeRTOS.h>
#include <car_bluetooth.h>
#include <SoftwareSerial.h>
#include <String.h>
#include "MotorDriver.h"
#include <Ultrasonic.h>
#include "TM1637.h"

#define TRIGGER_PIN 5  //connect Trip of the Ultrasonic Sensor moudle to D5 of Arduino
#define ECHO_PIN 3
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);
#define CLK A5  //connect CLK of the 4-Digit Display to A5 of Arduino and can be changed to other ports
#define DIO A4  //
TM1637 disp(CLK, DIO);
#define RxD 2  //D2 of Arduino should connect to TX of the Serial Bluetooth module
#define TxD 4  //D4 of Arduino should connect to RX of the Serial Bluetooth module
CarBluetooth bluetooth(RxD, TxD);
#define CMD_INVALID 0XFF
#define CMD_FORWARD 'F'
#define CMD_RIGHT_FRONT 'R'
#define CMD_BACKWARD 'B'
#define CMD_LEFT_FRONT 'L'
#define CMD_STOP 'S'
#define SPEED_STEPS 20
uint8_t speed0 = 100;
uint8_t bt_command;
#define CAR_STOP 0
#define CAR_FORWARD 1
#define CAR_BACK 2
uint8_t car_status = CAR_STOP;
uint8_t new_status = car_status;

void TaskBlink1(void *pvParameters);
void TaskBlink2(void *pvParameters);
void Taskprint(void *pvParameters);

void setup() {
  disp.set(BRIGHT_TYPICAL);  //BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  disp.init();
  motordriver.init();
  motordriver.setSpeed(100, MOTORA);
  motordriver.setSpeed(100, MOTORB);
  bluetooth.waitConnected();
  Serial.begin(9600);
  xTaskCreate(TaskBlink1, "Task1", 128, NULL, 0, NULL);
  xTaskCreate(TaskBlink2, "Task2 ", 128, NULL, 0, NULL);
  xTaskCreate(Taskprint, "Task3", 128, NULL, 2, NULL);
  vTaskStartScheduler();
}


void loop() {}

void TaskBlink1(void *pvParameters) {
  while (1) {
    Serial.println("Task 1 =" + String(millis()));
    ultrasonicDisplay();
    bt_command = bluetooth.readByte();
    if (bt_command != CMD_INVALID) {
      controlCar(bt_command);
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void TaskBlink2(void *pvParameters) {
  while (1) {
    Serial.println("Task 2 =" + String(millis()));
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void Taskprint(void *pvParameters) {
  while (1) {
    Serial.println("Task 3 =" + String(millis()));
    // Serial.println(millis());
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}


void controlCar(uint8_t cmd) {
  switch (cmd) {
    case CMD_FORWARD: motordriver.goForward(); break;
    case CMD_RIGHT_FRONT:
      //  if(car_status != CAR_STOP)new_status = CAR_FORWARD;
      motordriver.goRight();
      // delay(200);
      break;
    case CMD_BACKWARD: motordriver.goBackward(); break;
    case CMD_LEFT_FRONT:
      motordriver.goLeft();
      // delay(200);
      break;
    case CMD_STOP: motordriver.stop(); break;
    default: break;
  }
  if ((cmd >= '0') && (cmd <= '9')) {
    speed0 = cmd - 0x30;
    Serial.print(speed0);
    Serial.print(">");
    speed0 = map(speed0, 0, 9, 0, 255);
    Serial.println(speed0);
    motordriver.setSpeed(speed0, MOTORA);
    motordriver.setSpeed(speed0, MOTORB);
  }
}

void speedUp() {
  if (speed0 < 236) speed0 += SPEED_STEPS;
  else speed0 = 255;
  motordriver.setSpeed(speed0, MOTORA);
  motordriver.setSpeed(speed0, MOTORB);
}

void speedDown() {
  if (speed0 > 70) speed0 -= SPEED_STEPS;
  else speed0 = 50;
  motordriver.setSpeed(speed0, MOTORA);
  motordriver.setSpeed(speed0, MOTORB);
}
void ultrasonicDisplay() {
  float cmMsec, inMsec;
  long microsec = ultrasonic.timing();
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  inMsec = ultrasonic.convert(microsec, Ultrasonic::IN);
  disp.display((int16_t)cmMsec);  //in centimeters
}
