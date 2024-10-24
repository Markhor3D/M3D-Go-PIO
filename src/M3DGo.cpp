#include <M3DGo.h>
#include "go_main.h"
#include <Preferences.h>
#include "pinout.h"

#define constraintPercentage(x) x = x > 100 ? 100: (x < -100 ? -100:x)
M3DGo::M3DGo(/* args */)
{
    Preferences pref;
    pref.begin("settings");

    maxTravelSpeed = DefaultTravelSpeed_mps;

    if(pref.isKey("motorSpeed"))
        maxTravelSpeed = pref.getFloat("motorSpeed");
    maxSpinSpeed = maxTravelSpeed / WheelSpan / 2.0;
    pref.end();
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
void M3DGo::delay_s(float s){
    delay(round(s * 1000));
}
void M3DGo::delay(long ms){
    long st = millis();
    while(millis() - st < ms){
        BLELoop();
            notification_led_loop(notLED);
    }
}
void M3DGo::ScratchLoop(){    
    if (!goHasBegun)
        return;
    go_loop();
    DisplayLoop();
    BLELoop();
    MechanicalLoop();
}

// Set the brightness in percentage of the notification LED [0-100]
void M3DGo::setNotificationLED(int percentage){
    if (percentage < 0) 
        notLED = -1; // almost off
    else
        notLED = -(percentage * 255) / 100; // duty cycle
}
// put the led in blink loop
void M3DGo::blinkNotificationLED(int period){  
    if (period < 0) 
        period = 0;
    notLED = period; // duty cycle
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

// Get spin speed in rad/sec
float M3DGo::getMaxSpinSpeed(){
    return maxSpinSpeed;
}

// Set spin speed in rad/sec
void M3DGo::setMaxSpinSpeed(float speed){
    maxSpinSpeed = speed;
    setMaxTravelSpeed(WheelSpan / 2 * speed);
}

// Get travel speed in m/sec
float M3DGo::getMaxTravelSpeed(){
    return maxTravelSpeed;
}

// Set travel speed in m/sec
void M3DGo::setMaxTravelSpeed(float speed){
    maxTravelSpeed = speed;
    Preferences pref;
    pref.begin("settings");
    pref.putFloat("motorSpeed", speed);
    pref.end();
}

bool calibFailed(String str){
    go.stop();
    go.blinkNotificationLED(200);
    display.emoji(Emojis::Confused);
    remote.notify(str, NotificationType::Error);
    return false;
}
void calibUpdate(String str){
    go.blinkNotificationLED(2000);
    display.emoji(Emojis::Stuck);
    remote.notify(str, NotificationType::Normal);
}
// Manually calibrate the bot with user feedback. Returns true if successful.
bool M3DGo::calibrate(){
    calibUpdate("Calibration started");
    go.delay(1000);
    remote.waitForButton("Mark the start of travel on the floor", "Ok. Start!");
    go.forward();
    go.delay(1000);
    go.stop();
    float d = remote.askNumber("Mark the end of travel and enter the measured distance");
    float speed = d / 1.0F;
    
    if (speed < DefaultTravelSpeed_mps / 2 || speed > DefaultTravelSpeed_mps * 2)
        return calibFailed("Calib failed. Something unexpected happened with the bot");
    
    calibUpdate(String("Calibration done, speed (m/s) = ") + String(speed));
    setMaxTravelSpeed(speed);
    display.emoji(Emojis::Like);
    return true;
}

// Automatically calibrate the bot using the distance sensor. Returns true if successful
bool M3DGo::autoCalibrateWithDistanceSensor(){
    calibUpdate("Waiting for calibration");
    go.delay(5000);
    calibUpdate("Calibration started");
    // make sure we are NOT on the line
    if (line.touching())
        return calibFailed("Could not find the calibration pad");
    go.forward();
    // lets move forward in search of the left mark
    long st = millis();
    while(true){
        if (line.rightOfLine())
            break;
        else if (line.leftOfLine() || line.onTheLine())
            return calibFailed("Calibration failed; Unexpected calibration mark detected [1]");
        if (millis() - st > 500)
            return calibFailed("Calibration failed; could not find the calibration mark [1]");
    }
    // We found the left mark
    calibUpdate("Calibration mark 1 found");
    // lets move forward in the search of white space
    st = millis();
    while(true){
        if (line.notOnLine())
            break;
        else if (line.leftOfLine()) // tolerate left of line
            return calibFailed("Calibration failed; Unexpected calibration mark detected [2]");
        if (millis() - st > 500)
            return calibFailed("Calibration failed; could not find the calibration mark [2]");
    }
    // We found the white. Lets find the right mark this time
    st = millis();
    while(true){
         if (line.leftOfLine())
            break;
        else if (line.rightOfLine() || line.onTheLine())
            return calibFailed("Calibration failed; Unexpected calibration mark detected [3]");
        if (millis() - st > 500)
            return calibFailed("Calibration failed; could not find the calibration mark [3]");
    }
    // We found the right mark
    calibUpdate("Calibration mark 2 found");
    // lets move forward in the search of white space again
    st = millis();
    while(true){
        if (line.notOnLine())
            break;
        else if (line.leftOfLine()) // tolerate left of line
            return calibFailed("Calibration failed; Unexpected calibration mark detected [4]");
        if (millis() - st > 500)
            return calibFailed("Calibration failed; could not find the calibration mark [4]");
    }
    // We found the white. Lets find the start line now.
    st = millis();
    while(true){
         if (line.onTheLine())
            break;
        if (millis() - st > 500)
            return calibFailed("Calibration failed; could not find the start line");
    }
    long l1Start = millis();
    // On the line. Lets find the end of line
    st = millis();
    while(true){
         if (line.notOnLine())
            break;
        if (millis() - st > 500)
            return calibFailed("Calibration failed; could not find the calib track");
    }
    long l1End= millis();
    // We found the white again. Race to the end
    
    st = millis();
    while(true){
         if (line.onTheLine())
            break;
        if (millis() - st > 1500)
            return calibFailed("Calibration failed; could not find the end line");
    }
    long l2Start = millis();
    // On the line. Lets find the end of line
    st = millis();
    while(true){
         if (line.notOnLine())
            break;
        if (millis() - st > 500)
            return calibFailed("Calibration failed; could not finish the track");
    }
    long l2End= millis();
    stop();
    float time = ((float)((l2Start + l2End) -  (l1Start + l1Start))) / 2.0 / 1000.0;
    float calibDistance = CalibTrackLength;
    float speed = calibDistance / time;
    if (speed < DefaultTravelSpeed_mps / 2 || speed > DefaultTravelSpeed_mps * 2)
        return calibFailed("Calib failed. Something unexpected happened with the bot");
    
    calibUpdate(String("Calibration done, speed (m/s) = ") + String(speed));
    setMaxTravelSpeed(speed);
    display.emoji(Emojis::Like);
    return true;
}
// Automatically calibrate the bot using a calibration mat. Returns true if successful
bool M3DGo::autoCalibrateWithLineSensor(){
    calibUpdate("Waiting for calibration");
    go.delay(5000);
    calibUpdate("Calibration started");
    if(!range.attached())
        return calibFailed("Attach the range finder to calibrate");
    if (range.isOutOfRange())
        return calibFailed("Range finder can't see a wall");
    int samples = 10;
    go.forward();
    long st = millis();
    float lastT = 0, lastS = 0;
    float speed = 0;
    for (int i = 0; i < samples; i++){
        go.delay(100);

        float thisT = (millis() - st) / 1000.0F;
        float thisS = range.get_m();
        
        if (i > 0){
            float mi = -(thisS - lastS) / (thisT - lastT); // distance is decreasing
            if (mi < DefaultTravelSpeed_mps / 4 || mi > DefaultTravelSpeed_mps * 4){
                return calibFailed("Calib failed. Something unexpected is happening with the bot");
            }
            speed += mi / (samples - 1);
        }
        
        lastT = thisT;
        lastS = thisS;
    }
    go.stop();
    if (speed < DefaultTravelSpeed_mps / 2 || speed > DefaultTravelSpeed_mps * 2)
        return calibFailed("Calib failed. Something unexpected happened with the bot");
    
    calibUpdate(String("Calibration done, speed (m/s) = ") + String(speed));
    setMaxTravelSpeed(speed);
    display.emoji(Emojis::Like);
    return true;
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
// Indication

Indication::Indication(int _index)
{
    index=  _index;
}

Indication::~Indication()
{
}
void Indication::hide(){
}
void Indication::show(String notification){

}
// Notify with the given notification and the value
void Indication::show(String notification, float value){

}
// Notify with the given notification and the value
void Indication::show(String notification, String value){
    
}


// Default Objects

M3DGo go;
Hinge hingeA(0);
Hinge hingeB(1);
Display display;
RangeFinder range;
LineSensor line;
Remote remote;

