/*
 * M3D-Go Remote Example 1: Display Range Finder Value on Remote BLE Client
 *
 * This example demonstrates how to continuously send the range finder value to the BLE client, 
 * where it is displayed as a plot on the remote screen.
 *
 * Hardware Requirements:
 * - Rangefinder sensor (VL53L0X) must be attached to the M3D-Go.
 *
 * For any inquiries or support, please contact us at:
 * Email: info@markhor3d.com
 * Website: https://www.markhor3d.com
 * 
 * Example Functionality:
 * - Measures distance to an object and displays it as a live plot on the remote client.
 */

#include <M3DGo.h>

void setup() {
    go.begin();  // Initialize M3D-Go hardware
}

void loop() {
    if (remote.isConnected() && range.attached()) {
        float distance = range.get_mm();  // Get distance in mm
        if (!range.isOutOfRange()) {
            remote.indication1.plot("Distance (mm)", distance);  // Plot on remote client
        } else {
            remote.indication1.show("Out of Range");  // Show message if out of range
        }
    }
    go.delay(10);  // Small delay for stable updates
}
