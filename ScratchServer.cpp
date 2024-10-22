#include "ScratchServer.h"
#include "Mechanical.h"
#include "Display.h"

String &trimString(String &str)
{
  while (str.startsWith("\t") || str.startsWith(" "))
    str = str.substring(1);
  while (str.endsWith("\t") || str.endsWith(" "))
    str = str.substring(0, str.length() - 1);
  while (str.indexOf("  ") >= 0)
    str.replace("  ", " ");
  return str;
}
void parseScratchCommand(String &command)
{
  command = trimString(command);
  // this pair of commands can handle all continuous robot movements
  if (command.startsWith("lm") || command.startsWith("rm"))
  { // "uuuu motor nn%"
    float power = 1;
    bool isLeft = command.startsWith("l");
    command = command.substring(command.indexOf(" ")); // " NN%"
    command = trimString(command);                     // "NN%"
    if (command.endsWith("%"))
      command = command.substring(0, command.length() - 1); // "NN"
    command = trimString(command);
    if (command.length() > 0)
      power = command.toFloat() / 100.0F;
    Serial.printf("%s motor @ %f%\n", isLeft ? "left" : "right", power);
    if (isLeft)
      SetLeftMotor(power);
    else
      SetRightMotor(power);
  }
  else if (command.startsWith("servo"))
  { // "servo N nnn%" N =1/2, nnn = [0,180]
    command = command.substring(5);
    command = trimString(command);
    bool isFirst = command[0] == '1';
    int angle = 0;
    command = command.substring(1);
    command = trimString(command);
    angle = command.toInt();
    Serial.printf("servo %s @ %i%\n", isFirst ? "1" : "2", angle);
    if (isFirst)
      SetServoA(angle);
    else
      SetServoB(angle);
  }
  else if (command.startsWith("servos")) // servo speed
  { // "servo N nnn%" N =1/2, nnn = [0,180]
    command = command.substring(5);
    command = trimString(command);
    bool isFirst = command[0] == '1';
    int speed = 0;
    command = command.substring(1);
    command = trimString(command);
    speed = command.toInt();
    Serial.printf("servo speed %s @ %i%\n", isFirst ? "1" : "2", speed);
    if (isFirst)
      SetServo1Speed(speed);
    else
      SetServo2Speed(speed);
  }
  else if (command.startsWith("show"))
  { // "show Some String"
    command = command.substring(4);
    command = trimString(command);
    ShowMessage(command);
  }
  else if (command.startsWith("express"))
  { // "show Some String"
    command = command.substring(7);
    command = trimString(command);
    SetExpression(command);
  }
  else
  {
    Serial.print("Unknown command: ");
    Serial.println(command);
  }
}