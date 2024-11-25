/*
 * M3D-Go Basic Movements Example
 * 
 * This example demonstrates basic forward, reverse, and spin movements with delays.
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
 * - Moves the bot forward and backward, then spins clockwise and counterclockwise.
 */

#include <M3DGo.h>

void setup() {
    // Initialize M3D-Go hardware and Scratch core
    go.begin();  
}

void loop() {
    go.forward();           // Move forward
    go.delay_s(0.5);        // Delay for 0.5 seconds
    go.reverse();           // Move backward
    go.delay_s(0.5);
    go.stop();
    go.delay_s(2);          // Delay for 2 seconds after stopping

    go.spinClockwise();     // Spin clockwise
    go.delay_s(0.5);
    go.spinCounterClockwise(); // Spin counterclockwise
    go.delay_s(0.5);
    go.stop();
    go.delay_s(2);          // Delay for 2 seconds
}
