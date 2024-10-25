/*
 * M3D-Go Line Sensor Basic Example 1: Display Line Position
 *
 * This example demonstrates how to display the bot’s position relative to a line on the OLED display.
 * 
 * Hardware Requirements:
 * - Line sensor add-on must be attached to the M3D-Go.
 * - OLED Display (SSD1306) must be attached to show line position feedback.
 *
 * For any inquiries or support, please contact us at:
 * Email: info@markhor3d.com
 * Website: https://www.markhor3d.com
 * 
 * Example Functionality:
 * - Displays "On the Line", "Left of Line", "Right of Line", or "Off the Line" based on the sensor readings.
 */

#include <M3D-Go.h>

void setup() {
    go.begin();  // Initialize M3D-Go hardware
}

void loop() {
    if (line.onTheLine()) {
        display.text("On the Line");
    } else if (line.leftOfLine()) {
        display.text("Left of Line");
    } else if (line.rightOfLine()) {
        display.text("Right of Line");
    } else {
        display.text("Off the Line");
    }
    go.delay(100);  // Delay for display readability
}
