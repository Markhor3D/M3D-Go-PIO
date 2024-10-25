/*
 * M3D-Go Remote Example 3: Ask for Float and Display on OLED
 *
 * This example demonstrates how to prompt the user to input a float value via the BLE client, 
 * and then display the entered value on the OLED screen.
 *
 * Hardware Requirements:
 * - Remote BLE client must be connected.
 * - OLED Display (SSD1306) must be attached to show the entered value.
 *
 * For any inquiries or support, please contact us at:
 * Email: info@markhor3d.com
 * Website: https://www.markhor3d.com
 * 
 * Example Functionality:
 * - Prompts the user for a float value and displays it on the OLED screen.
 */

#include <M3D-Go.h>

void setup() {
    go.begin();  // Initialize M3D-Go hardware
}

void loop() {
    if (remote.isConnected()) {
        // Prompt user to enter a float value
        float value = remote.askNumber("Enter a float value:");
        
        // Display the value on the OLED screen
        display.text("Entered Value: " + String(value));
    }
    go.delay(10);  // Short delay to avoid excessive polling
}
