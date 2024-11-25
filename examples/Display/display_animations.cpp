/*
 * M3D-Go Display Example 3: Animation Using raw
 *
 * This example demonstrates a basic animation by moving a rectangle across the OLED screen.
 * 
 * Note: Each frame of the animation requires `display.display()` to refresh the screen.
 *
 * Hardware Requirements:
 * - OLED Display (SSD1306) must be attached to the M3D-Go.
 *
 * For any inquiries or support, please contact us at:
 * Email: info@markhor3d.com
 * Website: https://www.markhor3d.com
 * 
 * Example Functionality:
 * - Animates a rectangle moving across the screen.
 */

#include <M3DGo.h>

void setup() {
    go.begin();  // Initialize M3D-Go hardware
}

void loop() {
    if (display.attached()) {
        for (int x = 0; x < 128; x += 5) {
            display.raw->clearDisplay();        // Clear the display buffer

            // Draw moving rectangle
            display.raw->fillRect(x, 20, 20, 10, SSD1306_WHITE);
            display.raw->display();             // Refresh display to show updated frame

            go.delay(50);                       // Small delay for animation speed
        }
    }
}
