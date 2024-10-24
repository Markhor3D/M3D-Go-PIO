#include "Mechanical.h"
#include <Arduino.h>
#include "L298N\L298N.h"
#include "HCSR04/HCSR04.h"
#include <ESP32_Servo.h>
#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include "Pinout.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

Servo servo1;
Servo servo2;

L298N leftMotor = L298N(LeftMotorEnPin, LeftMotorP1Pin, LeftMotorP2Pin); // 12,13 // eN, P1, P2
L298N rightMotor = L298N(RightMotorEnPin, RightMotorP1Pin, RightMotorP2Pin);  // 2,4
float Sensor_Distance = 0;
float Sensor_ProximityA = 0;
float Sensor_ProximityB = 0;
void WriteServoAAngle(int angle);
void WriteServoBAngle(int angle);
void SetServoAQuick(int angle);
void SetServoBQuick(int angle);

// HCSR04 US(33, 32);

bool weHaveVLX = false;
void MechanicalSetup()
{
  pinMode(ProximityAPin, INPUT);
  pinMode(ProximityBPin, INPUT);
  servo1.attach(Servo1Pin);
  servo2.attach(Servo2Pin);
  #if GoVersion == 1
  SetServoAQuick(0);
  SetServoBQuick(0);
  #else
  SetServoAQuick(90);
  SetServoBQuick(90);
  #endif

  Wire.begin();
  Wire.beginTransmission(41);
  byte error = Wire.endTransmission();

  if (error == 0)
  {
    // Serial.println("VL53L0X found! Proceeding with initialization...");
    if (!lox.begin())
    {
      Serial.println(F("Failed to boot VL53L0X"));
      weHaveVLX = false;
    }
    else
    {
      weHaveVLX = true;
      Serial.println(F("Found VL53L0X"));
    }
  }
  else
  {
    Serial.println("VL53L0X not found or not responding!");
      weHaveVLX = false;
    // Handle the case where the sensor is not found or not responding
  }
}
float lastDistance = 0;
long lastSensorsRead = 0;
long lastServo1Set = 0;
long lastServo2Set = 0;
int servo1Speed = 20;
int servo2Speed = 20;
int servo1Target = 0, servo2Target = 0;
int servo1Current = -1, servo2Current = -1; // force an update at power up
void MechanicalLoop()
{
  if (servo1Speed < 0)
    servo1Speed = 1;
  else if (servo1Speed > 100)
    servo1Speed = 100;
  if (servo2Speed < 0)
    servo2Speed = 0;
  else if (servo2Speed > 100)
    servo2Speed = 100;
  if (millis() - lastServo1Set > (100 - servo1Speed))
  {
    if (servo1Current > servo1Target)
    {
      servo1Current--;
      Serial.printf("servo 1 write: %d\n", servo1Current);
      WriteServoAAngle(servo1Current);
    }
    else if (servo1Current < servo1Target)
    {
      servo1Current++;
      Serial.printf("servo 1 write: %d\n", servo1Current);
      WriteServoAAngle(servo1Current);
    }
    lastServo1Set = millis();
  }
  if (millis() - lastServo2Set > (100 - servo2Speed))
  {
    if (servo2Current > servo2Target)
    {
      servo2Current--;
      Serial.printf("servo 2 write: %d\n", servo2Current);
      WriteServoBAngle(servo2Current);
    }
    else if (servo2Current < servo2Target)
    {
      servo2Current++;
      Serial.printf("servo 2 write: %d\n", servo2Current);
      WriteServoBAngle(servo2Current);
    }
    lastServo2Set = millis();
  }
  if (millis() - lastSensorsRead > 10)
  {
    Sensor_ProximityA = (int)((float)analogRead(ProximityAPin) / 40.95F);
    Sensor_ProximityB = (int)((float)analogRead(ProximityBPin) / 40.95F);
    // Serial.print("S0 = ");
    // Serial.print(Sensors[0]);
    // Serial.print(", S1 = ");
    // Serial.println(Sensors[1]);

    if (weHaveVLX)
    {
      // Read VL53L0X
      VL53L0X_RangingMeasurementData_t measure;
      lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

      if (measure.RangeStatus != 4)
      { 
        float newDistance = measure.RangeMilliMeter;
        float fac = 0.2;
        newDistance = newDistance * fac + lastDistance * (1 - fac);
        lastDistance = newDistance;
        Sensor_Distance = round((newDistance) / 6.00 - 100); // 0 = -100%, 1200 is 100%
        // Serial.println(Sensors[2]);
      }
    }
    // phase failures have incorrect data
    else
      Sensor_Distance = 0;

    lastSensorsRead = millis();
  }
}

void SetL298Motor(L298N &motor, float factor)
{
  motor.setSpeed(abs(factor) * 255);
  if (factor > 0)
    motor.forward();
  else if (factor < 0)
    motor.backward();
  else
    motor.stop();
}

void SetLeftMotor(float factor)
{
  Serial.printf("SetLeftMotor(%f)\n", factor);
  SetL298Motor(leftMotor, factor);
}
void SetRightMotor(float factor)
{
  Serial.printf("SetRightMotor(%f)\n", factor);
  SetL298Motor(rightMotor, -factor); // default wiring is inverted.
}

void SetServoA(int angle)
{
  Serial.printf("SetServo1(%i)\n", angle);
  servo1Target = angle;
}
void SetServoB(int angle)
{
  Serial.printf("SetServo2(%i)\n", angle);
  servo2Target = angle;
}
void SetServoAQuick(int angle)
{
  WriteServoAAngle(angle);
  servo1Target = angle;
  servo1Current = angle;
}
void SetServoBQuick(int angle)
{
  WriteServoBAngle(angle);
  servo2Target = angle;
  servo2Current = angle;
}
void WriteServoAAngle(int angle)
{
  servo1.write(angle * 2); // TF
}
void WriteServoBAngle(int angle)
{
  servo2.write(angle * 2); // TF
}
void SetServo1Speed(int speed)
{
  servo1Speed = speed;
  if (servo1Speed < 0)
    servo1Speed = 0;
  else if (servo1Speed > 100)
    servo1Speed = 100;
}
void SetServo2Speed(int speed)
{
  servo2Speed = speed;
  if (servo2Speed < 0)
    servo2Speed = 0;
  else if (servo2Speed > 100)
    servo2Speed = 100;
}