#pragma once

#define GoVersion 2

#define Servo1Pin               23
#define Servo2Pin               19
#define ProximityAPin           34
#define ProximityBPin           35
#define DefaultTravelSpeed_mps  0.120
#define WheelSpan               0.082
#define CalibTrackLength        0.100

#if GoVersion == 1
#define LeftMotorEnPin      14
#define LeftMotorP1Pin      12
#define LeftMotorP2Pin      13
#define RightMotorEnPin     15
#define RightMotorP1Pin     2
#define RightMotorP2Pin     4
#define BatteryLevelPin     -1
#define StatusLEDPin        -1
#elif GoVersion == 2
#define LeftMotorEnPin      13
#define LeftMotorP1Pin      12
#define LeftMotorP2Pin      14
#define RightMotorEnPin     4
#define RightMotorP1Pin     2
#define RightMotorP2Pin     15
#define BatteryLevelPin     36
#define StatusLEDPin        18
#endif