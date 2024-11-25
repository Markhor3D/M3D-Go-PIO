/*
 * M3D-Go Display Example 2: Basic Shapes with raw
 *
 * This example demonstrates how to draw basic shapes using the raw display object.
 * 
 * Note: Since the raw object requires calling `display.display()` to update the screen, 
 * each drawing operation will need a display refresh.
 *
 * Hardware Requirements:
 * - OLED Display (SSD1306) must be attached to the M3D-Go.
 *
 * For any inquiries or support, please contact us at:
 * Email: info@markhor3d.com
 * Website: https://www.markhor3d.com
 * 
 * Example Functionality:
 * - Draws a rectangle, circle, and line on the OLED screen.
 */

#include <M3DGo.h>

void setup() {
    go.begin();  // Initialize M3D-Go hardware

    if (display.attached()) {
        // Clear the display buffer
        display.raw->clearDisplay();

        // Draw a rectangle
        display.raw->drawRect(10, 10, 50, 30, SSD1306_WHITE);
        display.raw->display();  // Update the display to show the rectangle
        go.delay(1000);

        // Draw a circle
        display.raw->drawCircle(64, 32, 20, SSD1306_WHITE);
        display.raw->display();  // Update the display to show the circle
        go.delay(1000);

        // Draw a line
        display.raw->drawLine(0, 0, 127, 63, SSD1306_WHITE);
        display.raw->display();  // Update the display to show the line
        go.delay(1000);
    }
}

void loop() {
    // No continuous actions needed
}
