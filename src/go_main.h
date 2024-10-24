// Private functions
// Dont include this is user code
#include "ScratchServer.h"
#include "Mechanical.h"
#include "Display.h"
#include "bluetooth.h"
void go_setup();
void go_loop();
// 0 = auto, >0 = blink period, <0 = duty cycle
void notification_led_loop(int period = 0);
