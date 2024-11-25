/*
 * M3D-Go Remote Example 4: Display Notifications with Different Types
 *
 * This example demonstrates how to display notifications of different types (Normal, Exclamation, Warning, Error) 
 * on the remote BLE client.
 *
 * Hardware Requirements:
 * - Remote BLE client must be connected.
 *
 * For any inquiries or support, please contact us at:
 * Email: info@markhor3d.com
 * Website: https://www.markhor3d.com
 * 
 * Example Functionality:
 * - Shows different types of notifications on the remote client to demonstrate the visual effects for each type.
 */

#include <M3DGo.h>

void setup() {
    go.begin();  // Initialize M3D-Go hardware
}

void loop() {
    if (remote.isConnected()) {
        // Display normal notification
        remote.notify("Normal Message", NotificationType::Normal);
        go.delay(1000);

        // Display exclamation notification
        remote.notify("Exclamation Alert", NotificationType::Exclamation);
        go.delay(1000);

        // Display warning notification
        remote.notify("Warning Message", NotificationType::Warning);
        go.delay(1000);

        // Display error notification
        remote.notify("Error Message", NotificationType::Error);
        go.delay(1000);
    }
}
