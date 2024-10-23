#include <Arduino.h>
#include <Adafruit_SSD1306.h>

class Hinge;
class Display;
class RangeFinder;
class LineSensor;
class Remote;

extern Hinge hingeA;
extern Hinge hingeB;
extern Display display;
extern RangeFinder range;
extern LineSensor line;
extern Remote remote;

class M3DGo
{
private:
    /* data */
    int notLED = 0;
    int leftMotorPower = 0;
    int rightMotorPower = 0;
public:
    M3DGo(/* args */);
    // Initializes M3D Go HW.
    void begin();
    // Set the brightness in percentage of the notification LED [0-100]
    void setNotificationLED(int percentage);
    // Get the brightness in percentage of the notification LED [0-100]
    int getNotificationLED();
    // Puts the bot in forward motion with the specified power in percentage. (Default = 100);
    // A negative percentage will put the respective motor in opposite direction
    // If a second paramter is defined, the bot will take the specified number of seconds to accelerate/decelerate
    void forward(int percentage = 100, float accelerateInSeconds = 0);
    // Puts the bot in reverse motion with the specified power in percentage. (Default = 100);
    // A negative percentage will put the respective motor in opposite direction
    // If a second paramter is defined, the bot will take the specified number of seconds to accelerate/decelerate
    void reverse(int percentage = 100, float accelerateInSeconds = 0);
    // Puts the bot in Clockwise spin with the specified power in percentage. (Default = 100);
    // A negative percentage will put the respective motors in opposite direction
    // If a second paramter is defined, the bot will take the specified number of seconds to accelerate/decelerate
    void spinClockwise(int percentage = 100, float accelerateInSeconds = 0);
    // Puts the bot in Counter Clockwise spin with the specified power in percentage. (Default = 100);
    // A negative percentage will put the respective motors in opposite direction
    // If a second paramter is defined, the bot will take the specified number of seconds to accelerate/decelerate
    void spinCounterClockwise(int percentage = 100, float accelerateInSeconds = 0);
    // Stops the bot from all forward, reverse, spin or other maneuvers in the specified time in seconds (Default = 0)
    void stop(float stopInSeconds = 0);
    // Puts the left side wheels in motion with the specified power in percentage. (Default = 100);
    // A negative percentage will put the respective motor in opposite direction
    // If a second paramter is defined, the motor will take the specified number of seconds to accelerate/decelerate
    void setLeftMotor(int percentage = 100, float accelerateInSeconds = 0);
    // Puts the right side wheels in motion with the specified power in percentage. (Default = 100);
    // A negative percentage will put the respective motor in opposite direction
    // If a second paramter is defined, the motor will take the specified number of seconds to accelerate/decelerate
    void setRightMotor(int percentage = 100, float accelerateInSeconds = 0);    
    // Puts the (1) left and (2) right side wheels in motion with the specified individual powers in percentage. (Default = 100);
    // A negative percentage will put the respective motor in opposite direction
    // If a third paramter is defined, the motors will take the specified number of seconds to accelerate/decelerate
    void setMotors(int leftPercentage = 100, int rightPercentage = 100, float accelerateInSeconds = 0);
    // Gets the current power set for the left side motor
    int getLeftMotor();    
    // Gets the current power set for the right side motor
    int getRightMotor();    

    void ScratchLoop();
    ~M3DGo();
};

extern M3DGo go;

class Hinge
{
private:
    int index = -1;
public:
    Hinge(int index);
    ~Hinge();
    float getAngle();
    void setAngle(float angleDegrees, float openInSeconds = 0);
    void open(float openInSeconds = 0);
    void close(float closeInSeconds = 0);    
};

enum Emojis{
    
    Stuck = 0,
	Confused,
	Frustrated,
	Funny,
	Joy,
	Laugh,
	Like,
	Love,
	Smile,
	Wink,
};

class Display
{
private:
    /* data */
    bool hasBegun = false;
public:
    Display(/* args */);
    ~Display();
    Adafruit_SSD1306* raw;
    bool attached();
    void text(String str);
    void text(float value);
    void text(String str, float value);
    void text(String str, String value);
    void emoji(Emojis emoji);
    void emoji(String emojiName);
};

class Lidar {
    public: 
        float obstructionAt(int x, int y);
        int currentX;
        int currentY;

};

class RangeFinder
{
private:
    /* data */
public:
    RangeFinder(/* args */);
    ~RangeFinder();
    // Reads the range in millimeters
    float get_mm();
    // Reads the range in meter
    float get_m();
    // Reads the range in centimeters
    float get_cm();
    // Reads the range in inches
    float get_inches();
    // Reads the range in feet
    float get_ft();
    // Determines whether a sensor is connected
    bool attached();
    Lidar updateArea();
    
};

class LineSensor
{
private:
    /* data */
public:
    LineSensor(/* args */);
    ~LineSensor();
    // Any of the photo sensors are touching the line
    void setThreshold();
    void getThreshold();
    void getLeft();
    void getRight();
    bool touching();
    bool onTheLine();
    bool leftOfLine();
    bool rightOfLine();
    bool notOnLine();
};

class Notification
{
private:
int index = 0;
public:
    Notification(int _index);
    ~Notification();
    void hide();
    void show(String notification);
    // Notify with the given notification and the value
    void show(String notification, float value);
    // Notify with the given notification and the value
    void show(String notification, String value);
    void plot(float value);
    void plot(String title, float value);
    void polarPlot(float value);
    void polarPlot(String title, float value);
};

// BLE Remote
class Remote{
    private:    
    bool _isConnected = false;
public:    
    Remote(/* args */);
    ~Remote();
    bool isConnected(){
        return _isConnected;
    }
    float askNumber(String question);
    String askString(String question);
    
    Notification indication1 = Notification(0);
    Notification indication2 = Notification(1);
    Notification indication3 = Notification(2);
    Notification indication4 = Notification(3);

    String waitForButton(String question, String buttonA);
    String waitForButton(String question, String buttonA, String buttonB);
    String waitForButton(String question, String buttonA, String buttonB, String buttonC);
    String waitForButton(String question, String buttonA, String buttonB, String buttonC, String buttonD);
    String waitForButton(String question, String buttonA, String buttonB, String buttonC, String buttonD, String buttonE);
};

