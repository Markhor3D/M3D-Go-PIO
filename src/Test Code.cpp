#include <Arduino.h>
#include <M3DGo.h>

void setup() 

{
    go.begin();  // Initialize M3D-Go hardware

   // Display Test

        display.text("Testing Display");      
        go.delay(1000);                          
        display.text("Hello, world!");           // Display single text
        go.delay(1000);                          // Wait 1 second
        display.emoji(Joy);                      // Display emoji by enum
        go.delay(4000);
        display.text("Checking Distance Sensor");
        go.delay(2000);

// Testing Motors
    display.text("Left Motor");
    go.setLeftMotor(100);           // Move left motor
    go.delay_s(1);        // Delay for 1 seconds
    go.stop();
    display.text("Right Motor");
    go.setRightMotor(100);
    go.delay_s(1);
    go.stop();
    display.text("Moving Forward");
    go.forward();
    go.delay_s(1);
    go.stop();
    display.text("Moving Backward");
    go.reverse();           // Move backward
    go.delay_s(1);
    go.stop();
    go.delay_s(2);          // Delay for 2 seconds after stopping
    display.text("Clockwise");
    go.spinClockwise();     // Spin clockwise
    go.delay_s(1);
    display.text("C_Clockwise");
    go.spinCounterClockwise(); // Spin counterclockwise
    go.delay_s(1);
    go.stop();
    go.delay_s(2);          // Delay for 2 seconds

// Distance Sensor Test
    display.text("Distance Sensor");
        float distance = range.get_mm();  // Get distance in millimeters
        if (!range.isOutOfRange()) {
            display.text("Distance: " + String(distance) + " mm");
        } else {
            display.text("Out of Range");
        }
    go.delay(4000);
    display.text("Testing Hinge A");
    go.delay(2000);

// Hinge Test

// Open the hinge to 90 degrees over 1 second
    hingeA.setAngle(90, 1);  
    go.delay(1000);  // Wait for the hinge to reach the angle

    // Display the current angle on the OLED screen
    float currentAngle = hingeA.getAngle();
    display.text("Angle: " + String(currentAngle) + "°");
    go.delay(10);  // Small delay for display readability

    // Close the hinge back to 0 degrees over 1 second
    hingeA.close(1);  
    go.delay(1000);  // Wait for the hinge to fully close

    // Display the current angle after closing
    currentAngle = hingeA.getAngle();
    display.text("Angle: " + String(currentAngle) + "°");
    go.delay(10);  // Small delay for display readability


    go.delay(4000);
    display.text("Testing Line Sensor");
    go.delay(2000);

// Line Follower Test

{
    if (line.onTheLine()) {
        display.text("On the Line");
    } else if (line.leftOfLine()) {
        display.text("Left of Line");
    } else if (line.rightOfLine()) {
        display.text("Right of Line");
    } else {
        display.text("Off the Line");
    }
    go.delay(3000);  // Delay for display readability
    int left = line.getLeft();
    int right = line.getRight();
    display.text(left);
    go.delay(2000);
    display.text(right);
    go.delay(2000);

    display.text("Test Complete!");
}
}

void loop() {
  // Nothing to run continuously
}
