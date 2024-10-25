/*
 * M3D-Go Acceleration Controls Example
 * 
 * This example demonstrates forward and reverse movement with smooth acceleration and deceleration.
 * 
 * Hardware Requirements:
 * - No additional add-ons are required for this example.
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
 * - Moves the bot forward with acceleration, stops with deceleration, then reverses with similar acceleration and deceleration.
 */

#include <M3D-Go.h>

void setup() {
    // Initialize M3D-Go hardware and Scratch core
    go.begin();  
}

void loop() {
    go.forward(100, 1);     // Move forward with 100% power, 1 second acceleration
    go.delay_s(0.5);
    go.stop(1);             // Stop with 1 second deceleration
    go.delay_s(2);          // Delay for 2 seconds

    go.reverse(100, 1);     // Move backward with 100% power, 1 second acceleration
    go.delay_s(0.5);
    go.stop(1);             // Stop with 1 second deceleration
    go.delay_s(2);          // Delay for 2 seconds
}
