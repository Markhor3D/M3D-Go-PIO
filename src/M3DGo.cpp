#include <M3DGo.h>
#include "go_main.h"

#define constraintPercentage(x) x = x > 100 ? 100: (x < -100 ? -100:x)
M3DGo::M3DGo(/* args */)
{
}

M3DGo::~M3DGo()
{
}
bool goHasBegun = false; // accessible to all classes

void M3DGo::begin(){
    if(goHasBegun)
        return;
    go_setup();
    goHasBegun = true;
}
void M3DGo::ScratchLoop(){    
    if (!goHasBegun)
        return;
    DisplayLoop();
    BLELoop();
    MechanicalLoop();
}

// Set the brightness in percentage of the notification LED [0-100]
void M3DGo::setNotificationLED(int percentage){

    notLED = percentage;
}
// Get the brightness in percentage of the notification LED [0-100]
int M3DGo::getNotificationLED(){
    return notLED;

}
// Puts the bot in forward motion with the specified power in percentage. (Default);
// If a second paramter is defined, the bot will take the specified number of seconds to accelerate/decelerate
void M3DGo::forward(int percentage, float accelerateInSeconds){
    constraintPercentage(percentage);
    setMotors(percentage, percentage, accelerateInSeconds);
}
// Puts the bot in reverse motion with the specified power in percentage. (Default);
// If a second paramter is defined, the bot will take the specified number of seconds to accelerate/decelerate
void M3DGo::reverse(int percentage, float accelerateInSeconds){
    constraintPercentage(percentage);
    setMotors(-percentage, -percentage, accelerateInSeconds);
}
// Puts the bot in Clockwise spin with the specified power in percentage. (Default);
// If a second paramter is defined, the bot will take the specified number of seconds to accelerate/decelerate
void M3DGo::spinClockwise(int percentage, float accelerateInSeconds){
    constraintPercentage(percentage);
    setMotors(-percentage, percentage, accelerateInSeconds);
}
// Puts the bot in Counter Clockwise spin with the specified power in percentage. (Default);
// If a second paramter is defined, the bot will take the specified number of seconds to accelerate/decelerate
void M3DGo::spinCounterClockwise(int percentage, float accelerateInSeconds){
    constraintPercentage(percentage);
    setMotors(percentage, -percentage, accelerateInSeconds);
}
// Stops the bot from all forward, reverse, spin or other maneuvers in the specified time in seconds (Default = 0)
void M3DGo::stop(float stopInSeconds){
    setMotors(0,0, stopInSeconds);
}
// Puts the left side wheels in motion with the specified power in percentage. (Default);
// If a second paramter is defined, the motor will take the specified number of seconds to accelerate/decelerate
void M3DGo::setLeftMotor(int percentage, float accelerateInSeconds){
    setMotors(percentage, rightMotorPower, accelerateInSeconds);
}
// Puts the right side wheels in motion with the specified power in percentage. (Default);
// If a second paramter is defined, the motor will take the specified number of seconds to accelerate/decelerate
void M3DGo::setRightMotor(int percentage, float accelerateInSeconds){
    setMotors(leftMotorPower, percentage, accelerateInSeconds);
}
// Puts the (1) left and (2) right side wheels in motion with the specified individual powers in percentage. (Default);
// If a third paramter is defined, the motors will take the specified number of seconds to accelerate/decelerate
// This is the core function for all go motor movements
void M3DGo::setMotors(int leftPercentage, int rightPercentage, float accelerateInSeconds){
    constraintPercentage(leftPercentage);
    constraintPercentage(rightPercentage);
    int ms = round(accelerateInSeconds * 1000.0F);
    float start = millis() / 1000.0F;
    int leftStart = leftMotorPower;
    int rightStart = rightMotorPower;
    int leftDelta = leftPercentage - leftStart;
    int rightDelta = rightPercentage - rightStart;
    if (isnan(accelerateInSeconds))
        accelerateInSeconds = 0;
    else if (isinf(accelerateInSeconds))
        accelerateInSeconds = 0;
    else if (accelerateInSeconds < 0.0001)
        accelerateInSeconds = 0;
    while(true){
        // Find fraction of movement
        float f = (millis() / 1000.0F - start) / accelerateInSeconds;
        if (f > 1)
            f = 1;
        if (f < 0)
            f = 0;
        
        int leftNew = leftStart + (int)round(leftDelta * f);
        int rightNew = rightStart + (int)round(leftDelta * f);
        SetLeftMotor(leftNew / 100.0F);
        SetRightMotor(leftNew / 100.0F);
        leftMotorPower = leftNew;
        rightMotorPower = rightNew;
        // Exit Condition
        if (f >= 1)
            return;
    }
}
// Gets the current power set for the left side motor
int M3DGo::getLeftMotor(){
    return leftMotorPower;
}
// Gets the current power set for the right side motor
int M3DGo::getRightMotor(){
    return rightMotorPower;
}

// Hinge

Hinge::Hinge(int index)
{
}

Hinge::~Hinge()
{
}


// Display

Display::Display(/* args */)
{
}

Display::~Display()
{
}

// Range Finder


RangeFinder::RangeFinder(/* args */)
{
}

RangeFinder::~RangeFinder()
{
}

// Line Sensor

LineSensor::LineSensor()
{
}

LineSensor::~LineSensor()
{
}

// BLERemote
Remote::Remote(){

}

Remote::~Remote(){
    
}

float Remote::askNumber(String question){
    
}
String Remote::askString(String question){
    
}
String Remote::waitForButton(String question, String buttonA){
    
}
String Remote::waitForButton(String question, String buttonA, String buttonB){
    
}
String Remote::waitForButton(String question, String buttonA, String buttonB, String buttonC){
    
}
String Remote::waitForButton(String question, String buttonA, String buttonB, String buttonC, String buttonD){
    
}
String Remote::waitForButton(String question, String buttonA, String buttonB, String buttonC, String buttonD, String buttonE){
    
}
// Notification

Notification::Notification(int _index)
{
    index=  _index;
}

Notification::~Notification()
{
}
void Notification::hide(){
}
void Notification::show(String notification){

}
// Notify with the given notification and the value
void Notification::show(String notification, float value){

}
// Notify with the given notification and the value
void Notification::show(String notification, String value){
    
}


// Default Objects

M3DGo go;
Hinge hingeA(0);
Hinge hingeB(1);
Display display;
RangeFinder range;
LineSensor line;
Remote remote;

