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
        BLE.poll();
        notification_led_loop(notLED);
        range_finder_loop();
    }
}
void M3DGo::ScratchLoop(){    
    if (!goHasBegun)
        return;
    go_loop();
    DisplayLoop();
    BLE.poll();
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

// Spin the bot by a specified angle in degrees. A positive angle spins clockwise,
// while a negative angle spins counterclockwise.
void M3DGo::spinAngle(float angle){   
    float s = (WheelSpan / 2) * angle * PI / 180.0F;
    float t = s / maxTravelSpeed;
    if (angle > 0)
        spinCounterClockwise();
    else
        spinClockwise();
    delay(round(t * 1000));
    stop();
}

// Move the bot in a straight line for the specified distance (in centimeters).
// Negative distance moves the bot backward. Requires a calibrated bot.
void M3DGo::travel_cm(float distance){
    float t = (distance / 100.0F) / maxTravelSpeed;
    if (distance > 0)
        forward();
    else
        reverse();
    delay(round(t * 1000));
    stop();
}

void M3DGo::travel_mm(float distance){
    travel_cm(distance / 10.0F);
}
void M3DGo::travel_inch(float distance){
    travel_mm(distance * 25.4);
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


// Get the current angle of the hinge (in degrees).
// Example usage:
//   float angle = hingeA.getAngle();
float Hinge::getAngle(){
    return currentAngle;
}

void servo_write(int index, float angleD){
    if (index == 0){
        WriteServoAAngle(round(angleD));
    }
    else if (index == 1){
        WriteServoBAngle(round(angleD));
    }
}
// Set the hinge to a specific angle (in degrees) with an optional duration.
// Example usage:
//   hingeA.setAngle(45, 2);  // Set hinge to 45 degrees, over 2 seconds
void Hinge::setAngle(float angleDegrees, float openInSeconds){
    if (angleDegrees < 0)
        angleDegrees = 0;
    else if (angleDegrees > 90)
        angleDegrees = 90;
    if (openInSeconds <= 0){
        currentAngle = angleDegrees;        
        servo_write(index, currentAngle);
        return;
    }
    int msPerCycle = 20;
    int cycles = round(openInSeconds * 1000.0) / 20;
    float dTh = (angleDegrees - currentAngle) / (float)cycles;
    while(cycles > 0){
        currentAngle += dTh; 
        servo_write(index, currentAngle);
        delay(msPerCycle);
    };
}

// Open the hinge completely. Optional time to control the duration.
// Example usage:
//   hingeA.open(1.5);  // Open hinge in 1.5 seconds
void Hinge::open(float openInSeconds){
    setAngle(90, openInSeconds);
}

// Close the hinge completely. Optional time to control the duration.
// Example usage:
//   hingeA.close(1);  // Close hinge in 1 second
void Hinge::close(float closeInSeconds){
    setAngle(0, closeInSeconds);
}

// Display

Display::Display(/* args */)
{
    raw = &oled;
}

Display::~Display()
{
}

// Check if the display is connected and ready.
bool Display::attached(){
    if (!weHaveOled)
        weHaveOled = oled.begin();
    return weHaveOled;
}

// Display text on the screen.
// Example usage:
//   display.text("Hello, world!");
void Display::text(String str){
    ShowMessage(str);
}

// Display a floating-point value on the screen.
void Display::text(float value){
    String str = String(value);
    ShowMessage(str);
}

// Display text and a numeric value on the screen.
void Display::text(String str, float value){
    str = str + ": " + String(value);
    ShowMessage(str);
}

// Display two strings on the screen.
void Display::text(String str, String value){
    str = str + ": " + value;
    ShowMessage(str);
}

// Display an emoji on the screen using an enum value.
void Display::emoji(Emojis emoji){
    String str = "stuck";
    if (emoji == Emojis::Confused) str = "confused"; else 
    if (emoji == Emojis::Frustrated) str = "frustrated"; else 
    if (emoji == Emojis::Funny) str = "funny"; else 
    if (emoji == Emojis::Joy) str = "joy"; else 
    if (emoji == Emojis::Laugh) str = "laugh"; else 
    if (emoji == Emojis::Like) str = "like"; else 
    if (emoji == Emojis::Love) str = "love"; else 
    if (emoji == Emojis::Smile) str = "smile"; else 
    if (emoji == Emojis::Wink) str = "wink";
    SetExpression(str);
}

// Display an emoji on the screen using the emoji name.
void Display::emoji(String emojiName){
    SetExpression(emojiName);
}

// Range Finder


RangeFinder::RangeFinder(/* args */)
{
}

RangeFinder::~RangeFinder()
{
}

// Get the distance in millimeters.
float RangeFinder::get_mm(){
    if(!attached())
        return 0;
    return range_finder_loop();
}

// Get the distance in meters.
float RangeFinder::get_m(){
    return get_mm() / 1000.0F;
}

// Get the distance in centimeters.
float RangeFinder::get_cm(){

    return get_mm() / 100.0F;
}

// Get the distance in inches.
float RangeFinder::get_inches(){
    return get_mm() / 25.4F;
}

// Get the distance in feet.
float RangeFinder::get_ft(){

    return get_inches() / 12.0F;
}

// Check if the rangefinder sensor is connected.
bool RangeFinder::attached(){
    if (!weHaveVLX)
        weHaveVLX = lox.begin();
    return weHaveVLX;
}

// Check if the rangefinder sensor is out of range
bool RangeFinder::isOutOfRange(){
    if (!attached())
        return true;
    if (range_finder_loop() >= 1200)
        return true;
    return false;
}
// Line Sensor

LineSensor::LineSensor()
{
    threshold = DefaultProximityThreshold;
}

LineSensor::~LineSensor()
{
}

// Set the threshold value for detecting a line.
void LineSensor::setThreshold(int value){
    threshold = value;
}

// Get the current threshold value for the line sensor.
int LineSensor::getThreshold(){    
    return threshold;
}

// Teach that the background is lighter than the line
void LineSensor::setBackgroundIsLighter(){
    invertLogic = false;
}

// Teach that the background is darker than the line
void LineSensor::setBackgroundIsDarker(){
    invertLogic = true;
}
// Teach that the background is lighter than the line
bool LineSensor::getBackgroundIsLighter(){
    return invertLogic == false;
}

// Teach that the background is darker than the line
bool LineSensor::getBackgroundIsDarker(){
    return invertLogic == true;
}

#define readRangePinRaw(x) ((int)((float)analogRead(x) / 40.95F))
#define readRangePin(x) (invertLogic?(100 - readRangePinRaw(x)): (readRangePinRaw(x)))
// Get the value from the left side of the sensor (relative to the sensor itself).
int LineSensor::getLeft(){
    return readRangePin(ProximityAPin);
}

// Get the value from the right side of the sensor (relative to the sensor itself).
int LineSensor::getRight(){
    return readRangePin(ProximityBPin);
}

// Check if any part of the sensor is touching the line.
bool LineSensor::touching(){
    return leftOfLine() || rightOfLine() || onTheLine();
}

// Check if the bot is on the line.
bool LineSensor::onTheLine(){
    return getLeft() > threshold && getRight() > threshold;
}

// Check if the bot is to the left of the line.
bool LineSensor::leftOfLine(){
    return getLeft() < threshold && getRight() > threshold;
}

// Check if the bot is to the right of the line.
bool LineSensor::rightOfLine(){
    return getLeft() > threshold && getRight() < threshold;
}

// Check if the bot is not on the line.
bool LineSensor::notOnLine(){
    return !onTheLine();
}


// BLERemote
Remote::Remote(){

}

Remote::~Remote(){
    
}
enum RemoteRequestID{
    AskNumber = 0,
    AskString,
    Notify,
    AskButton1,
    AskButton2,
    AskButton3,
    AskButton4,
    AskButton5,
    NotificationSetFloatA,
    NotificationSetFloatA2,
    NotificationSetFloatA3,
    NotificationSetFloatA4,    
    NotificationSetFloatB,
    NotificationSetFloatB2,
    NotificationSetFloatB3,
    NotificationSetFloatB4,    
    NotificationSetStringA,
    NotificationSetStringA2,
    NotificationSetStringA3,
    NotificationSetStringA4,    
    NotificationSetStringB,
    NotificationSetStringB2,
    NotificationSetStringB3,
    NotificationSetStringB4,
    NotificationHide,
    NotificationHide2,
    NotificationHide3,
    NotificationHide4,
    NotificationSetPlot,
    NotificationSetPlot2,
    NotificationSetPlot3,
    NotificationSetPlot4,
    NotificationSetPolarPlot,
    NotificationSetPolarPlot2,
    NotificationSetPolarPlot3,
    NotificationSetPolarPlot4,    
    NotificationSetIndicator,
    NotificationSetIndicator2,
    NotificationSetIndicator3,
    NotificationSetIndicator4,
};

void bleGetResponse(RemoteRequestID id){

    BLE.poll(); // flush previous
    aRemoteValueHasReturned = false;
    remoteProcessNotifyRequestCharacteristic.setValue(id);

    while(!aRemoteValueHasReturned){
        BLE.poll();
        if (CurrentCommunicationChannel != CommunicationType::BluetoothLE){
            delay(1);
            continue;
        }
    }
}
String& bleGetString(RemoteRequestID id){
    bleGetResponse(id);
    return remoteStringValueReturned;
}
float bleGetFloat(RemoteRequestID id){
    bleGetResponse(id);
    return remoteFloatValueReturned;
}
float Remote::askNumber(String question){
    remoteNotifyString0Characteristic.setValue(question);
    return bleGetFloat(RemoteRequestID::AskNumber);
}
String Remote::askString(String question){    
    remoteNotifyString0Characteristic.setValue(question);
    return bleGetString(RemoteRequestID::AskString);
}
void Remote::notify(String message, NotificationType type){
    remoteNotifyString0Characteristic.setValue(message);
    remoteNotifyFloatCharacteristic.setValue(type);
    remoteProcessNotifyRequestCharacteristic.setValue(RemoteRequestID::Notify);
}
String Remote::waitForButton(String question, String buttonA){    
    remoteNotifyString0Characteristic.setValue(question);
    remoteNotifyString1Characteristic.setValue(buttonA);
    return bleGetString(RemoteRequestID::AskButton1);
}
String Remote::waitForButton(String question, String buttonA, String buttonB){  
    remoteNotifyString0Characteristic.setValue(question);
    remoteNotifyString1Characteristic.setValue(buttonA);
    remoteNotifyString2Characteristic.setValue(buttonB);
    return bleGetString(RemoteRequestID::AskButton2);
}
String Remote::waitForButton(String question, String buttonA, String buttonB, String buttonC){
    remoteNotifyString0Characteristic.setValue(question);
    remoteNotifyString1Characteristic.setValue(buttonA);
    remoteNotifyString2Characteristic.setValue(buttonB);
    remoteNotifyString3Characteristic.setValue(buttonC);
    return bleGetString(RemoteRequestID::AskButton3);
}
String Remote::waitForButton(String question, String buttonA, String buttonB, String buttonC, String buttonD){
    remoteNotifyString0Characteristic.setValue(question);
    remoteNotifyString1Characteristic.setValue(buttonA);
    remoteNotifyString2Characteristic.setValue(buttonB);
    remoteNotifyString3Characteristic.setValue(buttonC);
    remoteNotifyString4Characteristic.setValue(buttonD);
    return bleGetString(RemoteRequestID::AskButton4);
}
String Remote::waitForButton(String question, String buttonA, String buttonB, String buttonC, String buttonD, String buttonE){
    
    remoteNotifyString0Characteristic.setValue(question);
    remoteNotifyString1Characteristic.setValue(buttonA);
    remoteNotifyString2Characteristic.setValue(buttonB);
    remoteNotifyString3Characteristic.setValue(buttonC);
    remoteNotifyString4Characteristic.setValue(buttonD);
    remoteNotifyString5Characteristic.setValue(buttonE);
    return bleGetString(RemoteRequestID::AskButton5);
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
    remoteProcessNotifyRequestCharacteristic.setValue(NotificationHide + index);
}
void Indication::show(String notification){
    remoteNotifyString0Characteristic.setValue(notification);
    remoteProcessNotifyRequestCharacteristic.setValue(NotificationSetStringA + index);
}
// Notify with the given notification and the value
void Indication::show(String notification, float value){
    remoteNotifyString0Characteristic.setValue(notification);
    remoteProcessNotifyRequestCharacteristic.setValue(NotificationSetStringA + index);
    remoteNotifyFloatCharacteristic.setValue(value);    
    remoteProcessNotifyRequestCharacteristic.setValue(NotificationSetFloatA + index);
    remoteProcessNotifyRequestCharacteristic.setValue(NotificationSetIndicator + index);
}
// Notify with the given notification and the value
void Indication::show(String notification, String value){    
    remoteNotifyString0Characteristic.setValue(notification);
    remoteProcessNotifyRequestCharacteristic.setValue(NotificationSetStringA + index);
    remoteNotifyString1Characteristic.setValue(value);
    remoteProcessNotifyRequestCharacteristic.setValue(NotificationSetFloatB + index);
    remoteProcessNotifyRequestCharacteristic.setValue(NotificationSetIndicator + index);
}

// Plot a floating-point value on the remote, against time.
void Indication::plot(float yValue){
    remoteNotifyFloatCharacteristic.setValue(yValue);    
    remoteProcessNotifyRequestCharacteristic.setValue(NotificationSetFloatB + index);
    remoteProcessNotifyRequestCharacteristic.setValue(NotificationSetPlot + index);
}

// Plot a value with a title.
void Indication::plot(String title, float yValue){
    remoteNotifyString0Characteristic.setValue(title);
    remoteProcessNotifyRequestCharacteristic.setValue(NotificationSetStringA + index);
    remoteNotifyFloatCharacteristic.setValue(yValue);    
    remoteProcessNotifyRequestCharacteristic.setValue(NotificationSetFloatB + index);
    remoteProcessNotifyRequestCharacteristic.setValue(NotificationSetPlot + index);
}

// Plot an (x, y) pair on the remote.
void Indication::plot(float xValue, float yValue){
    remoteNotifyFloatCharacteristic.setValue(xValue);    
    remoteProcessNotifyRequestCharacteristic.setValue(NotificationSetFloatA + index);
    remoteNotifyFloatCharacteristic.setValue(yValue);    
    remoteProcessNotifyRequestCharacteristic.setValue(NotificationSetFloatB + index);
    remoteProcessNotifyRequestCharacteristic.setValue(NotificationSetPlot + index);
}

// Create a polar plot with a floating-point value.
void Indication::polarPlot(float value){
    remoteNotifyFloatCharacteristic.setValue(value);    
    remoteProcessNotifyRequestCharacteristic.setValue(NotificationSetFloatB + index);
    remoteProcessNotifyRequestCharacteristic.setValue(NotificationSetPolarPlot + index);
}

// Create a polar plot with a title and value.
void Indication::polarPlot(String title, float value){    
    remoteNotifyString0Characteristic.setValue(title);
    remoteProcessNotifyRequestCharacteristic.setValue(NotificationSetStringA + index);
    remoteNotifyFloatCharacteristic.setValue(value);    
    remoteProcessNotifyRequestCharacteristic.setValue(NotificationSetFloatB + index);
    remoteProcessNotifyRequestCharacteristic.setValue(NotificationSetPlot + index);
}

// Plot a value in polar coordinates (angle in degrees and radius).
void Indication::polarPlot(float angle_d, float value_r){
    remoteNotifyFloatCharacteristic.setValue(angle_d);    
    remoteProcessNotifyRequestCharacteristic.setValue(NotificationSetFloatA + index);
    remoteNotifyFloatCharacteristic.setValue(value_r);    
    remoteProcessNotifyRequestCharacteristic.setValue(NotificationSetFloatB + index);
    remoteProcessNotifyRequestCharacteristic.setValue(NotificationSetPolarPlot + index);
}


// Default Objects

M3DGo go;
Hinge hingeA(0);
Hinge hingeB(1);
Display display;
RangeFinder range;
LineSensor line;
Remote remote;

