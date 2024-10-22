#include <Adafruit_SSD1306.h>

void DisplaySetup();
void DisplayLoop();
void SetExpression(String& label);
void ShowMessage(String& msg);

extern Adafruit_SSD1306 oled;
extern bool weHaveOled;