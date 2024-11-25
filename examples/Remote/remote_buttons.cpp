/*
 * M3D-Go Remote Example 2: Five Buttons for Basic Movement Control
 *
 * This example demonstrates using five buttons on the BLE client to control basic movements: 
 * forward, backward, clockwise spin, counterclockwise spin, and stop.
 *
 * Hardware Requirements:
 * - Remote BLE client must be connected.
 *
 * For any inquiries or support, please contact us at:
 * Email: info@markhor3d.com
 * Website: https://www.markhor3d.com
 * 
 * Example Functionality:
 * - Provides buttons on the remote for basic movement control, allowing users to control the bot’s motion.
 */

#include <M3DGo.h>

void setup() {
    go.begin();  // Initialize M3D-Go hardware
}

void loop() {
    if (remote.isConnected()) {
        // Ask user to press one of five movement buttons
        String command = remote.waitForButton("Select Action:", "Forward", "Backward", "Spin CW", "Spin CCW", "Stop");

        if (command == "Forward") {
            go.forward();
        } else if (command == "Backward") {
            go.reverse();
        } else if (command == "Spin CW") {
            go.spinClockwise();
        } else if (command == "Spin CCW") {
            go.spinCounterClockwise();
        } else if (command == "Stop") {
            go.stop();
        }
    }
    go.delay(10);  // Short delay for button press stability
}
