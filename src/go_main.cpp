#include <Arduino.h>
#include <LittleFS.h>
#include "Common.h"
#include "Network.h"
#include "WebSocketService.h"
#include "ScratchServer.h"
#include "Mechanical.h"
#include "Display.h"
#include "bluetooth.h"

CommunicationType CurrentCommunicationChannel = CommunicationType::NotConnected;
bool FindDevicesOnWire(){
  Wire.beginTransmission(0x3C);
  byte error = Wire.endTransmission();
  if (error != 0) // no device on 0x3C
  {
    Wire.beginTransmission(41);  // vl
    error = Wire.endTransmission();
    if (error != 0) // no device on VL too!
      return false;
  }
  return true; // oled is here
}
void go_setup()
{
  Serial.begin(115200);
  Serial.println("Begin");

  if (!LittleFS.begin())
  {
    Serial.println("Formating LittleFS");
    LittleFS.format();
  }

  Wire.begin(); // default pins
  if (!FindDevicesOnWire()){    
    Serial.print("No devices");
    delay(1);
    Wire.end();
    Serial.end();
    Wire.begin(3, 1); // Serial pins
    if (!FindDevicesOnWire()){
      Wire.end();
      Wire.begin(21, 22); // Back to original pins
      Serial.begin(115200);
      delay(1);
      Serial.print("Couldn't find anything with serial pins too");
      delay(1);
    }
  }
  else{
      Serial.print("I2C devices are here.");
  }
  DisplaySetup();
  BLESetup();
  MechanicalSetup();
  if (weHaveOled)
  {
    String str = String() + BLEName;
    oled.clearDisplay();
    oled.setCursor(5, 5);
    oled.print("Bluetooth name");
    oled.setCursor(5, 15);
    oled.print(BLEName);
    oled.display();
  }
  // WebSocketsSetup();
  // NetworkSetup();
}
void go_loop()
{
  DisplayLoop();
  BLELoop();
  MechanicalLoop();
  // WebSocketsLoop();
  // NetworkLoop();

  Stream *streams[2] = {&Serial, /* &SerialBT*/};
  for (int i = 0; i < 1; i++)
  {
    Stream *stream = streams[i];
    if (!stream->available())
      continue;
    char com = stream->read();
    if (com == 'F' || com == 'f')
    {
      SetLeftMotor(1);
      SetRightMotor(1);
    }
    else if (com == 'B' || com == 'b')
    {
      SetLeftMotor(-1);
      SetRightMotor(-1);
    }
    else if (com == 'R' || com == 'r')
    {
      SetLeftMotor(-1);
      SetRightMotor(+1);
    }
    else if (com == 'L' || com == 'l')
    {
      SetLeftMotor(+1);
      SetRightMotor(-1);
    }
    else if (com == 'S' || com == 's')
    {
      int ind = stream->readStringUntil('=')[0] - '0';
      int val = stream->readStringUntil('\n').toInt();
      if (ind == 1)
        SetServoA(val);
      else
        SetServoB(val);
    }
    else
    {
      SetLeftMotor(0);
      SetRightMotor(0);
    }
  }
}