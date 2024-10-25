/*
 * M3D-Go Turns Example
 * 
 * This example demonstrates turning movements using different power levels for left and right motors.
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
 * - Demonstrates turning by setting different power levels for left and right motors.
 */

#include <M3D-Go.h>

void setup() {
    // Initialize M3D-Go hardware and Scratch core
    go.begin();  
}

void loop() {
    go.setMotors(100, 50);  // Set left motor to 100%, right motor to 50% (turning effect)
    go.delay_s(1);          // Delay for 1 second
    go.stop(1);             // Stop with 1 second deceleration
    go.delay_s(2);          // Delay for 2 seconds

    go.setMotors(-100, -50); // Set left motor to -100%, right motor to -50% (turn in reverse)
    go.delay_s(1);           // Delay for 1 second
    go.stop(1);              // Stop with 1 second deceleration
    go.delay_s(2);           // Delay for 2 seconds
}
