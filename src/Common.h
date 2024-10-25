#pragma once
#include <Arduino.h>

typedef enum {
NotConnected,
WiFiWebSockets,
BluetoothLE
} CommunicationType;
extern CommunicationType CurrentCommunicationChannel;

String getChipNumber();
String getChipNumberShort();