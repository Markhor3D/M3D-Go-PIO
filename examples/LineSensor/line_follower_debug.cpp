/*
 * M3D-Go Line Sensor Advanced Example 1: Line Detection with Display Feedback
 *
 * This example provides feedback on the OLED display showing the M3D-Go bot’s line detection status.
 * It dynamically displays the bot's status as it follows the line.
 * 
 * Hardware Requirements:
 * - Line sensor add-on must be attached to the M3D-Go.
 * - OLED Display (SSD1306) must be attached to show line detection feedback.
 *
 * For any inquiries or support, please contact us at:
 * Email: info@markhor3d.com
 * Website: https://www.markhor3d.com
 * 
 * Example Functionality:
 * - Displays "Following Line" if on track, "Adjusting Right" or "Adjusting Left" if off-center,
 *   and "No Line Detected" if no line is detected.
 */

#include <M3D-Go.h>

void setup() {
    go.begin();  // Initialize M3D-Go hardware
}

void loop() {
    if (line.onTheLine()) {
        go.forward();              // Move forward if on the line
        display.text("Following Line");
    } else if (line.leftOfLine()) {
        go.spinClockwise();        // Adjust to the right
        display.text("Adjusting Right");
    } else if (line.rightOfLine()) {
        go.spinCounterClockwise(); // Adjust to the left
        display.text("Adjusting Left");
    } else {
        go.stop();                 // Stop if no line detected
        display.text("No Line Detected");
    }
    go.delay(10);  // Delay for readability and control stability
}
