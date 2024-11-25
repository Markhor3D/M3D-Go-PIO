/*
 * M3D-Go Scratch Core Example
 * 
 * This example initializes the M3D-Go robot and continuously runs the Scratch interface loop, allowing it to 
 * interact with Scratch-based applications.
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
 * - Initializes the M3D-Go hardware and starts the Scratch interface loop.
 */

#include <M3DGo.h>

void setup() {
    // Initialize M3D-Go hardware and Scratch core
    go.begin();  
}

void loop() {
    // Continuously run the Scratch interface loop
    ScratchLoop();  
}
