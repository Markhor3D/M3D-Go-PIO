/*
 * M3D-Go Range Finder Example 2: Maintain Fixed Distance
 *
 * This example demonstrates how the bot can maintain a specified distance (50mm) from an object.
 * The bot will move forward or backward to adjust its position as needed.
 *
 * Hardware Requirements:
 * - Rangefinder sensor (VL53L0X) must be attached to the M3D-Go.
 *
 * For any inquiries or support, please contact us at:
 * Email: info@markhor3d.com
 * Website: https://www.markhor3d.com
 * 
 * Example Functionality:
 * - Measures distance to an object and adjusts movement to maintain 50mm from the object.
 */

#include <M3D-Go.h>

const float targetDistance = 50.0;  // Desired distance from the object in mm
const float tolerance = 5.0;        // Tolerance for distance adjustment in mm

void setup() {
    go.begin();  // Initialize M3D-Go hardware
}

void loop() {
    if (range.attached()) {
        float distance = range.get_mm();  // Get distance in millimeters

        if (!range.isOutOfRange()) {
            if (distance > targetDistance + tolerance) {
                go.forward();  // Move forward to get closer
            } else if (distance < targetDistance - tolerance) {
                go.reverse();  // Move backward to increase distance
            } else {
                go.stop();     // Stop if within target range
            }
        } else {
            go.stop();  // Stop if no object is within range
        }
    } else {
        go.stop();  // Stop if the sensor is not attached
    }
    go.delay(10);  // Small delay for stability in adjustments
}
