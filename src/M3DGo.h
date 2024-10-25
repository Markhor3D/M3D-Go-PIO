#include <Arduino.h>  // Core Arduino library for general functionality
#include <Adafruit_SSD1306.h>  // Library for controlling the SSD1306 OLED display

// Forward declarations of classes used in the M3D Go bot
class Hinge;        // Represents a servo-controlled hinge
class Display;      // Manages the SSD1306 display
class RangeFinder;  // Represents the VL53L0X range-finding sensor
class LineSensor;   // Represents the IR-based line detection sensor
class Remote;       // Represents the Web BLE client

// Extern objects representing the add-ons connected to M3D Go bot
extern Hinge hingeA;      // Left hinge attached to the bot
extern Hinge hingeB;      // Right hinge attached to the bot
extern Display display;   // OLED display for showing information
extern RangeFinder range; // VL53L0X based rangefinder for distance measurement
extern LineSensor line;   // Line sensor for detecting lines and boundaries
extern Remote remote;     // BLE remote control interface

// Class managing the core functionality of the M3D Go robot
class M3DGo
{
private:
    int notLED = 0;  // Stores the brightness of the notification LED
    int leftMotorPower = 0;  // Stores the current power of the left motor
    int rightMotorPower = 0; // Stores the current power of the right motor
    float maxTravelSpeed = 0; // to be restored from prefs
    float maxSpinSpeed = 0; // to be restored from prefs
public:
    // Constructor for M3DGo
    M3DGo(/* args */);

    // Initializes the hardware components of the M3D Go bot.
    // Example usage:
    //   M3DGo go;
    //   go.begin();
    void begin();

    // Delays the execution of the code be the specified seconds while respecting the background tasks of M3D Go
    // Example usage:
    //   go.delay(500); // Halts the execution for half a second
    void delay_s(float seconds);
    // Delays the execution of the code be the specified milliseconds while respecting the background tasks of M3D Go
    // Example usage:
    //   go.delay(0.5); // Halts the execution for half a second
    void delay(long ms);
    // Set the brightness of the notification LED [0-100%]
    // Example usage:
    //   go.setNotificationLED(50);  // Set LED brightness to 50%
    void setNotificationLED(int percentage);

    // Puts the LED in a blinking routine.
    // Example usage:
    //   go.blinkNotificationLED(1000);  // LED will start blinking once every second
    void blinkNotificationLED(int period_ms);

    // Get the current brightness of the notification LED [0-100%]
    // Example usage:
    //   int brightness = go.getNotificationLED();
    int getNotificationLED();

    // Move the bot forward with a specified power [0-100%]. If a second parameter
    // is defined, the bot will take that number of seconds to accelerate.
    // Example usage:
    //   go.forward(80, 2.5);  // Move forward with 80% power, accelerating over 2.5 seconds
    void forward(int percentage = 100, float accelerateInSeconds = 0);

    // Move the bot in reverse with a specified power [0-100%]. If a second parameter
    // is defined, the bot will take that number of seconds to decelerate.
    // Example usage:
    //   go.reverse(70);  // Move backward with 70% power
    void reverse(int percentage = 100, float accelerateInSeconds = 0);

    // Spin the bot by a specified angle in degrees. A positive angle spins clockwise,
    // while a negative angle spins counterclockwise.
    // Example usage:
    //   go.spinAngle(90);  // Rotate the bot 90 degrees clockwise
    void spinAngle(int angle = 90);

    // Move the bot in a straight line for the specified distance (in centimeters).
    // Negative distance moves the bot backward. Requires a calibrated bot.
    // Example usage:
    //   go.travel_cm(50);  // Move forward for 50 cm
    void travel_cm(int distance = 90);

    // Spin the bot clockwise with the specified power [0-100%]. Optionally, set a duration
    // for acceleration in seconds.
    // Example usage:
    //   go.spinClockwise(100, 3);  // Spin clockwise with 100% power over 3 seconds
    void spinClockwise(int percentage = 100, float accelerateInSeconds = 0);

    // Spin the bot counterclockwise with the specified power [0-100%].
    // Example usage:
    //   go.spinCounterClockwise(100);  // Spin counterclockwise with 100% power
    void spinCounterClockwise(int percentage = 100, float accelerateInSeconds = 0);

    // Stop all movement of the bot. Optionally, set a stop duration in seconds.
    // Example usage:
    //   go.stop(2);  // Stop the bot over 2 seconds
    void stop(float stopInSeconds = 0);

    // Set the power of the left motor [0-100%], with optional acceleration.
    // Example usage:
    //   go.setLeftMotor(70, 1);  // Set left motor to 70% power, accelerate over 1 second
    void setLeftMotor(int percentage = 100, float accelerateInSeconds = 0);

    // Set the power of the right motor [0-100%], with optional acceleration.
    // Example usage:
    //   go.setRightMotor(70);  // Set right motor to 70% power
    void setRightMotor(int percentage = 100, float accelerateInSeconds = 0);

    // Set the power of both motors (left and right) individually [0-100%].
    // Optional parameter to set acceleration.
    // Example usage:
    //   go.setMotors(70, 50, 2);  // Set left motor to 70%, right motor to 50%, accelerate over 2 seconds
    void setMotors(int leftPercentage = 100, int rightPercentage = 100, float accelerateInSeconds = 0);

    // Get the current power of the left motor.
    // Example usage:
    //   int leftPower = go.getLeftMotor();
    int getLeftMotor();

    // Get the current power of the right motor.
    // Example usage:
    //   int rightPower = go.getRightMotor();
    int getRightMotor();

    // Manually calibrate the bot with user feedback. Returns true if successful.
    // Example usage:
    //   bool success = go.calibrate();
    bool calibrate();

    // Automatically calibrate the bot using a calibration mat. Returns true if successful.
    // Example usage:
    //   bool success = go.autoCalibrateWithLineSensor();
    bool autoCalibrateWithLineSensor();

    // Automatically calibrate the bot using a wall and the distance sensor. Returns true if successful.
    // Example usage:
    //   bool success = go.autoCalibrateWithDistanceSensor();
    bool autoCalibrateWithDistanceSensor();


    // Function used for the Scratch environment loop.
    void ScratchLoop();
    
    // Get spin speed in rad/sec
    float getMaxSpinSpeed();

    // Set spin speed in rad/sec
    void setMaxSpinSpeed(float speed);
    
    // Get travel speed in m/sec
    float getMaxTravelSpeed();

    // Set travel speed in m/sec
    void setMaxTravelSpeed(float speed);


    // Destructor for M3DGo.
    ~M3DGo();
};

extern M3DGo go;  // Global object to control the M3D Go bot

// Class to manage a servo-driven hinge. 
// This hinge can be opened and closed or set to a specific angle.
class Hinge
{
private:
    int index = -1;  // Index to identify the hinge
    float currentAngle = 0;
public:
    // Constructor for Hinge
    Hinge(int index);
    ~Hinge();
    // Get the current angle of the hinge (in degrees).
    // Example usage:
    //   float angle = hingeA.getAngle();
    float getAngle();

    // Set the hinge to a specific angle (in degrees) with an optional duration.
    // Example usage:
    //   hingeA.setAngle(45, 2);  // Set hinge to 45 degrees, over 2 seconds
    void setAngle(float angleDegrees, float openInSeconds = 0);

    // Open the hinge completely. Optional time to control the duration.
    // Example usage:
    //   hingeA.open(1.5);  // Open hinge in 1.5 seconds
    void open(float openInSeconds = 0);

    // Close the hinge completely. Optional time to control the duration.
    // Example usage:
    //   hingeA.close(1);  // Close hinge in 1 second
    void close(float closeInSeconds = 0);    
};

// Enum for different emoji states that can be displayed on the OLED screen.
enum Emojis{
    Stuck = 0,       // Robot is stuck
    Confused,        // Robot is confused
    Frustrated,      // Robot is frustrated
    Funny,           // Display funny emoji
    Joy,             // Display joyful emoji
    Laugh,           // Display laughing emoji
    Like,            // Display a "Like" emoji
    Love,            // Display a love emoji
    Smile,           // Display a smile emoji
    Wink             // Display a wink emoji
};

// Class to control the OLED display, which is based on the SSD1306.
class Display
{
private:
    bool hasBegun = false;  // Flag indicating if the display has been initialized
public:
    Display(/* args */);
    ~Display();
    
    Adafruit_SSD1306* raw;  // Pointer to the raw display object

    // Check if the display is connected and ready.
    // Example usage:
    //   if (display.attached()) { ... }
    bool attached();

    // Display text on the screen.
    // Example usage:
    //   display.text("Hello, world!");
    void text(String str);

    // Display a floating-point value on the screen.
    // Example usage:
    //   display.text(3.14);
    void text(float value);

    // Display text and a numeric value on the screen.
    // Example usage:
    //   display.text("Temp", 25.5);
    void text(String str, float value);

    // Display two strings on the screen.
    // Example usage:
    //   display.text("Hello", "World");
    void text(String str, String value);

    // Display an emoji on the screen using an enum value.
    // Example usage:
    //   display.emoji(Joy);
    void emoji(Emojis emoji);

    // Display an emoji on the screen using the emoji name.
    // Example usage:
    //   display.emoji("Smile");
    void emoji(String emojiName);
};

// Class to manage the VL53L0X rangefinder sensor.
class RangeFinder
{
public:
    RangeFinder(/* args */);
    ~RangeFinder();

    // Get the distance in millimeters.
    // Example usage:
    //   float distance = range.get_mm();
    float get_mm();

    // Get the distance in meters.
    // Example usage:
    //   float distance = range.get_m();
    float get_m();

    // Get the distance in centimeters.
    // Example usage:
    //   float distance = range.get_cm();
    float get_cm();

    // Get the distance in inches.
    // Example usage:
    //   float distance = range.get_inches();
    float get_inches();

    // Get the distance in feet.
    // Example usage:
    //   float distance = range.get_ft();
    float get_ft();

    // Check if the rangefinder sensor is connected.
    // Example usage:
    //   if (range.attached()) { ... }
    bool attached();

    // Check if the rangefinder sensor is out of range
    // Example usage:
    //   if (range.isOutOfRange()) { ... }
    bool isOutOfRange();
};

// Class representing the IR-based line sensor.
// The left and right are relative to the sensor, not the bot itself.
class LineSensor
{
    private:
    int threshold;
    bool invertLogic = false;
public:
    LineSensor(/* args */);
    ~LineSensor();

    // Set the threshold value for detecting a line.
    // Example usage:
    //   line.setThreshold();
    void setThreshold(int value);

    // Teach that the background is lighter than the line
    // Example usage:
    //   lin.setBackGroundIsLight();
    void setBackgroundIsLighter();

    // Teach that the background is darker than the line
    // Example usage:
    //   line.backGroundIsLight();
    void setBackgroundIsDarker();

    // Determines if the background is lighter than the line
    // Example usage:
    //   if(line.getBackGroundIsLight()){ // ... }
    bool getBackgroundIsLighter();

    // Determines if the background is darker than the line
    // Example usage:
    //   if(line.getBackGroundIsDarker()){ // ... }
    bool getBackgroundIsDarker();

    // Get the current threshold value for the line sensor.
    // Example usage:
    //   line.getThreshold();
    int getThreshold();

    // Get the value from the left side of the sensor (relative to the sensor itself).
    // Example usage:
    //   line.getLeft();
    int getLeft();

    // Get the value from the right side of the sensor (relative to the sensor itself).
    // Example usage:
    //   line.getRight();
    int getRight();

    // Check if any part of the sensor is touching the line.
    // Example usage:
    //   bool touching = line.touching();
    bool touching();

    // Check if the bot is on the line.
    // Example usage:
    //   bool onLine = line.onTheLine();
    bool onTheLine();

    // Check if the bot is to the left of the line.
    // Example usage:
    //   bool leftOfLine = line.leftOfLine();
    bool leftOfLine();

    // Check if the bot is to the right of the line.
    // Example usage:
    //   bool rightOfLine = line.rightOfLine();
    bool rightOfLine();

    // Check if the bot is not on the line.
    // Example usage:
    //   bool offLine = line.notOnLine();
    bool notOnLine();
};

// Possible values for a notification type
enum NotificationType{
    Normal = 0,     
    Exclamation,     
    Warning,
    Error, 
};
// Renamed class representing indications on the Web BLE client.
class Indication
{
private:
    int index = 0;  // Index for identifying the indication
public:
    Indication(int _index);
    ~Indication();

    // Hide the respective indication widget on the remote.
    // Example usage:
    //   indication1.hide();
    void hide();

    // Show a notification with the given text on the remote.
    // Example usage:
    //   indication1.show("Battery Low");
    void show(String notification);

    // Show a notification with text and a floating-point value.
    // Example usage:
    //   indication1.show("Temperature", 23.5);
    void show(String notification, float value);

    // Show a notification with text and a string value.
    // Example usage:
    //   indication1.show("Mode", "Autonomous");
    void show(String notification, String value);

    // Plot a floating-point value on the remote, against time.
    // Example usage:
    //   indication1.plot(20.5);
    void plot(float yValue);

    // Plot a value with a title.
    // Example usage:
    //   indication1.plot("Temperature", 25.0);
    void plot(String title, float yValue);

    // Plot an (x, y) pair on the remote.
    // Example usage:
    //   indication1.plot(1.0, 20.0);
    void plot(float xValue, float yValue);

    // Create a polar plot with a floating-point value.
    // Example usage:
    //   indication1.polarPlot(3.14);
    void polarPlot(float value);

    // Create a polar plot with a title and value.
    // Example usage:
    //   indication1.polarPlot("Angle", 45.0);
    void polarPlot(String title, float value);

    // Plot a value in polar coordinates (angle in degrees and radius).
    // Example usage:
    //   indication1.polarPlot(30.0, 5.0);
    void polarPlot(float angle_d, float value_r);
};

// BLE Remote class managing interaction with a Web BLE client
class Remote{
private:
    bool _isConnected = false;  // Connection status flag

public:    
    Remote(/* args */);
    ~Remote();

    // Check if the remote is currently connected.
    // Example usage:
    //   if (remote.isConnected()) { ... }
    bool isConnected(){
        return _isConnected;
    }

    // Prompt the user to input a number via the Web BLE client.
    // Example usage:
    //   float value = remote.askNumber("Enter speed:");
    float askNumber(String question);

    // Prompt the user to input a string via the Web BLE client.
    // Example usage:
    //   String name = remote.askString("Enter robot name:");
    String askString(String question);

    // Shows a message on the screen
    // Example usage:
    //   remote.askString("M3D Go rocks!");
    bool notify(String message, NotificationType type = NotificationType::Normal);


    // Create multiple Indication objects for the remote client.
    Indication indication1 = Indication(0);
    Indication indication2 = Indication(1);
    Indication indication3 = Indication(2);
    Indication indication4 = Indication(3);

    // Wait for the user to press a specific button (one or more) via Web BLE client.
    // Example usage:
    //   String buttonPressed = remote.waitForButton("Choose action", "Start", "Stop");
    String waitForButton(String question, String buttonA);
    String waitForButton(String question, String buttonA, String buttonB);
    String waitForButton(String question, String buttonA, String buttonB, String buttonC);
    String waitForButton(String question, String buttonA, String buttonB, String buttonC, String buttonD);
    String waitForButton(String question, String buttonA, String buttonB, String buttonC, String buttonD, String buttonE);
};
