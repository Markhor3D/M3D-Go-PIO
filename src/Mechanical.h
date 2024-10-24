#pragma once

#include <Adafruit_VL53L0X.h>
extern Adafruit_VL53L0X lox;
extern bool weHaveVLX;
float range_finder_loop();
void MechanicalSetup();
void MechanicalLoop();

void SetLeftMotor(float factor);
void SetRightMotor(float factor);
void SetServoA(int angle);
void SetServoB(int angle);
void SetServo1Speed(int speed);
void SetServo2Speed(int speed);

extern float Sensor_Distance;
extern float Sensor_ProximityA;
extern float Sensor_ProximityB;
