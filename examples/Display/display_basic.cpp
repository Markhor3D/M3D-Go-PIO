/*
 * M3D-Go Display Example 1: Show Text and Emojis
 *
 * This example demonstrates how to display text and emojis on the OLED screen.
 *
 * Hardware Requirements:
 * - OLED Display (SSD1306) must be attached to the M3D-Go.
 *
 * For any inquiries or support, please contact us at:
 * Email: info@markhor3d.com
 * Website: https://www.markhor3d.com
 * 
 * Example Functionality:
 * - Displays various text messages and emojis on the OLED screen.
 */

#include <M3DGo.h>

void setup() {
    go.begin();  // Initialize M3D-Go hardware

    if (display.attached()) {
        display.text("Hello, world!");           // Display single text
        go.delay(1000);                          // Wait 1 second

        display.text("Temp", 25.5);              // Display text with numeric value
        go.delay(1000);

        display.text("Status", "Active");        // Display two strings
        go.delay(1000);

        display.emoji(Joy);                      // Display emoji by enum
        go.delay(1000);

        display.emoji("Smile");                  // Display emoji by name
        go.delay(1000);
    }
}

void loop() {
    // No continuous actions needed
}
