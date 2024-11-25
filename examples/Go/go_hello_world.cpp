/*
 * M3D-Go Hello World Example
 * 
 * This example displays "Hello, world!" on the M3D-Go's OLED screen.
 * 
 * Hardware Requirements:
 * - OLED Display (SSD1306) add-on must be attached to the M3D-Go.
 * 
 * For any inquiries or support, please contact us at:
 * Email: info@markhor3d.com
 * Website: https://www.markhor3d.com
 * 
 * Disclaimer: 
 * This software is provided "as is", without warranty of any kind, express or implied, including but not limited to 
 * the warranties of merchantability, fitness for a particular purpose and noninfringement. In no event shall the 
 * authors or copyright holders be liable for any claim, damages, or other liability, whether in an action of contract, 
 * tort or otherwise, arising from, out of, or in connection with the software or the use or other dealings in the software.
 * 
 * Example Functionality:
 * - Displays "Hello, world!" on the OLED screen.
 */

#include <M3DGo.h>

void setup() {
    // Initialize M3D-Go hardware and Scratch core
    go.begin();  
    display.text("Hello, world!");  // Display message
}

void loop() {
    // No continuous actions needed in this example
}
