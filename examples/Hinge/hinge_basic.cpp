/*
 * M3D-Go Hinge Example: Open and Close with Angle Feedback
 *
 * This example demonstrates how to control the hinge by setting specific angles and opening/closing durations.
 * The current angle of the hinge is displayed on the OLED screen as feedback.
 *
 * Hardware Requirements:
 * - Hinge (servo motor) add-on must be attached to the M3D-Go.
 * - OLED Display (SSD1306) must be attached to show the hinge angle.
 *
 * For any inquiries or support, please contact us at:
 * Email: info@markhor3d.com
 * Website: https://www.markhor3d.com
 * 
 * Example Functionality:
 * - Opens the hinge to 90 degrees, displays the current angle, then closes it back to 0 degrees.
 */

#include <M3D-Go.h>

void setup() {
    go.begin();      // Initialize M3D-Go hardware
    hingeA.close();  // Start with hinge fully closed
}

void loop() {
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
}
