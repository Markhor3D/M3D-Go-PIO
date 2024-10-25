/*
 * M3D-Go Range Finder Example 1: Display Live Distance
 *
 * This example demonstrates how to continuously measure and display the distance from an object on the OLED screen.
 *
 * Hardware Requirements:
 * - Rangefinder sensor (VL53L0X) must be attached to the M3D-Go.
 * - OLED Display (SSD1306) must be attached to show live distance feedback.
 *
 * For any inquiries or support, please contact us at:
 * Email: info@markhor3d.com
 * Website: https://www.markhor3d.com
 * 
 * Example Functionality:
 * - Continuously displays the distance (in mm) from an object on the OLED screen.
 */

#include <M3D-Go.h>

void setup() {
    go.begin();  // Initialize M3D-Go hardware
}

void loop() {
    if (range.attached()) {
        float distance = range.get_mm();  // Get distance in millimeters
        if (!range.isOutOfRange()) {
            display.text("Distance: " + String(distance) + " mm");
        } else {
            display.text("Out of Range");
        }
    } else {
        display.text("Sensor Not Attached");
    }
    go.delay(200);  // Update display every 200 ms
}
