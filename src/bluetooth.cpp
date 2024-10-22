#include "bluetooth.h"
#include <ArduinoBLE.h>
#include "Common.h"
#include "Mechanical.h"
#include "Display.h"
#include <vector>
String BLEName;

// create service and characteristics:
BLEService m3dScratchLinkService("8e088cd2-8000-11ee-b9d1-0242ac120002");
BLEByteCharacteristic proximityACharactristic("8e088cd2-7001-11ee-b9d1-0242ac120002", BLERead | BLENotify);
BLEByteCharacteristic proximityBCharactristic("8e088cd2-7002-11ee-b9d1-0242ac120002", BLERead | BLENotify);
BLEByteCharacteristic distanceCharactristic("8e088cd2-7003-11ee-b9d1-0242ac120002", BLERead | BLENotify);
BLEByteCharacteristic leftMotorCharactristic("8e088cd2-6001-11ee-b9d1-0242ac120002", BLEWriteWithoutResponse);
BLEByteCharacteristic rightMotorCharactristic("8e088cd2-6002-11ee-b9d1-0242ac120002", BLEWriteWithoutResponse);
BLEByteCharacteristic servoAAngleCharactristic("8e088cd2-6003-11ee-b9d1-0242ac120002", BLEWriteWithoutResponse);
BLEByteCharacteristic servoBAngleCharactristic("8e088cd2-6004-11ee-b9d1-0242ac120002", BLEWriteWithoutResponse);
BLEByteCharacteristic servoASpeedCharactristic("8e088cd2-6005-11ee-b9d1-0242ac120002", BLEWriteWithoutResponse);
BLEByteCharacteristic servoBSpeedCharactristic("8e088cd2-6006-11ee-b9d1-0242ac120002", BLEWriteWithoutResponse);
BLEStringCharacteristic expressionCharactristic("8e088cd2-6007-11ee-b9d1-0242ac120002", BLEWriteWithoutResponse, 16);
BLEStringCharacteristic textMessageCharactristic("8e088cd2-6008-11ee-b9d1-0242ac120002", BLEWriteWithoutResponse, 16);

void connectHandler(BLEDevice central);
void disconnectHandler(BLEDevice central);
bool isDeviceNameTaken(String name);

void leftMotorIncomingHander(BLEDevice central, BLECharacteristic characteristic);
void rightMotorIncomingHander(BLEDevice central, BLECharacteristic characteristic);
void servoAAngleIncomingHander(BLEDevice central, BLECharacteristic characteristic);
void servoBAngleIncomingHander(BLEDevice central, BLECharacteristic characteristic);
void servoASpeedIncomingHander(BLEDevice central, BLECharacteristic characteristic);
void servoBSpeedIncomingHander(BLEDevice central, BLECharacteristic characteristic);
void expressionIncomingHander(BLEDevice central, BLECharacteristic characteristic);
void textMessageIncomingHander(BLEDevice central, BLECharacteristic characteristic);

void BLESetup()
{

    BLE.begin();
    BLE.scan(true);
    String deviceNameSeed = "M3D Go ";

    std::vector<String> nameList;
    for (int i = 0; i < 20; i++)
    {
        BLE.scan();
        delay(50);
        BLEDevice peripheral = BLE.available();
        while (peripheral)
        {
            //Serial.printf("Found peripheral: [%s], ", peripheral.localName().c_str());

            // Access strings in the list
            bool found = false;
            for (const auto &newName : nameList)
            {
                if (newName == peripheral.localName())
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                //Serial.println("Added");
                nameList.push_back(peripheral.localName());
            }
            // else
            //     Serial.println("skipped");
            peripheral = BLE.available();
        }
    }
    // Now we have a reliable list. Lets find options for device name:

    for (int i = 1; i <= 100; i++)
    {
        BLEName = deviceNameSeed + String(i);
        bool matchesExisting = false;
        //Serial.printf("Looking for availability of: [%s]", BLEName.c_str());
        for (const auto &existingName : nameList)
        {
            if (existingName == BLEName)
            {
                matchesExisting = true;
                //Serial.printf("Argh! already exists");
                break;
            }
        }
        if (!matchesExisting) // found it!
        {
            //Serial.printf("Found our name: [%s]", BLEName.c_str());
            break;
        }
    }
    BLE.setLocalName(BLEName.c_str());
    // set the UUID for the service this peripheral advertises:
    BLE.setAdvertisedService(m3dScratchLinkService);

    // add the characteristics to the service:
    m3dScratchLinkService.addCharacteristic(proximityACharactristic);
    m3dScratchLinkService.addCharacteristic(proximityBCharactristic);
    m3dScratchLinkService.addCharacteristic(distanceCharactristic);
    m3dScratchLinkService.addCharacteristic(leftMotorCharactristic);
    m3dScratchLinkService.addCharacteristic(rightMotorCharactristic);
    m3dScratchLinkService.addCharacteristic(servoAAngleCharactristic);
    m3dScratchLinkService.addCharacteristic(servoBAngleCharactristic);
    m3dScratchLinkService.addCharacteristic(servoASpeedCharactristic);
    m3dScratchLinkService.addCharacteristic(servoBSpeedCharactristic);
    m3dScratchLinkService.addCharacteristic(expressionCharactristic);
    m3dScratchLinkService.addCharacteristic(textMessageCharactristic);

    // assign event handlers for connected, disconnected to peripheral
    BLE.setEventHandler(BLEConnected, connectHandler);
    BLE.setEventHandler(BLEDisconnected, disconnectHandler);

    // assign event handler for characteristics:

    leftMotorCharactristic.setEventHandler(BLEWritten, leftMotorIncomingHander);
    rightMotorCharactristic.setEventHandler(BLEWritten, rightMotorIncomingHander);
    servoAAngleCharactristic.setEventHandler(BLEWritten, servoAAngleIncomingHander);
    servoBAngleCharactristic.setEventHandler(BLEWritten, servoBAngleIncomingHander);
    servoASpeedCharactristic.setEventHandler(BLEWritten, servoASpeedIncomingHander);
    servoBSpeedCharactristic.setEventHandler(BLEWritten, servoBSpeedIncomingHander);
    expressionCharactristic.setEventHandler(BLEWritten, expressionIncomingHander);
    textMessageCharactristic.setEventHandler(BLEWritten, textMessageIncomingHander);

    // add the service and set a value for the characteristic:
    BLE.addService(m3dScratchLinkService);
    // start advertising
    BLE.advertise();
}
// Function to check if the device name is taken
bool isDeviceNameTaken(String name)
{
    Serial.println("Scanning BLE");
    for (int i = 0; i < 20; i++)
    {
        Serial.printf("Scan try: %d\n", i + 1);
        BLE.scan(true);
        delay(100);
        BLEDevice peripheral = BLE.available();
        while (peripheral)
        {
            Serial.printf("Found peripheral: [%s]\n", peripheral.localName().c_str());
            if (peripheral.localName() == name)
            {
                Serial.printf("[%s] = [%s]\n", peripheral.localName().c_str(), name.c_str());
                Serial.print("Device Name ");
                Serial.print(name);
                Serial.println(" is already taken.");
                return true;
            }
            else
                Serial.printf("[%s] != [%s]\n", peripheral.localName().c_str(), name.c_str());
            peripheral = BLE.available();
        }
    }
    Serial.println("End of scan");
    return false;
}
long lastSensorsSentBLE = 0;
long lastForcedSensorsSentBLE = 0;
float ProximityASent = -1000;
float ProximityBSent = -1000;
float DistanceSent = -1000;

byte floatPercentToByte(float f){
    if (f < -100)
        f = -100;
    else if (f > 100)
        f = 100;
    return (byte)round(f + 100);
}
float byteToFloatPercent(byte b){
    return (float)(b - 100);
}
void BLELoop()
{
    // Listen for events:
    BLE.poll();
    if (millis() - lastSensorsSentBLE > 20 && CurrentCommunicationChannel == CommunicationType::BluetoothLE)
    {
        bool forcedSend = millis() - lastForcedSensorsSentBLE > 2000;
        if (ProximityASent != Sensor_ProximityA || forcedSend)
            proximityACharactristic.setValue(floatPercentToByte(Sensor_ProximityA));
        if (ProximityBSent != Sensor_ProximityB || forcedSend)
            proximityBCharactristic.setValue(floatPercentToByte(Sensor_ProximityB));
        if (DistanceSent != Sensor_Distance || forcedSend)
            distanceCharactristic.setValue(floatPercentToByte(Sensor_Distance));
        ProximityASent = Sensor_ProximityA;
        ProximityBSent = Sensor_ProximityB;
        DistanceSent = Sensor_Distance;
        if (forcedSend)
            lastForcedSensorsSentBLE = millis();
        lastSensorsSentBLE = millis();
    }
}

void connectHandler(BLEDevice central)
{
    CurrentCommunicationChannel = CommunicationType::BluetoothLE;
    // central connected event handler
    Serial.print("BLE Connected: ");
    Serial.println(central.address());
}

void disconnectHandler(BLEDevice central)
{
    // central disconnected event handler
    Serial.print("BLE Disconncted");
    Serial.println(central.address());
}

void leftMotorIncomingHander(BLEDevice central, BLECharacteristic characteristic)
{
    // central wrote new value to characteristic, update LED
    Serial.print("Left Motor characteristic event, written: ");
    Serial.println(byteToFloatPercent(leftMotorCharactristic.value()) / 100);
    SetLeftMotor(byteToFloatPercent(leftMotorCharactristic.value()) / 100);
}
void rightMotorIncomingHander(BLEDevice central, BLECharacteristic characteristic)
{
    // central wrote new value to characteristic, update LED
    Serial.print("Right Motor characteristic event, written: ");
    Serial.println(byteToFloatPercent(rightMotorCharactristic.value()) / 100);
    SetRightMotor(byteToFloatPercent(rightMotorCharactristic.value()) / 100);
}
void servoAAngleIncomingHander(BLEDevice central, BLECharacteristic characteristic)
{
    // central wrote new value to characteristic, update LED
    Serial.print("Servo A Angle characteristic event, written: ");
    Serial.println(byteToFloatPercent(servoAAngleCharactristic.value()));
    SetServoA(byteToFloatPercent(servoAAngleCharactristic.value()));
}
void servoBAngleIncomingHander(BLEDevice central, BLECharacteristic characteristic)
{
    // central wrote new value to characteristic, update LED
    Serial.print("Servo B Angle characteristic event, written: ");
    Serial.println(byteToFloatPercent(servoBAngleCharactristic.value()));
    SetServoB(byteToFloatPercent(servoBAngleCharactristic.value()));
}
void servoASpeedIncomingHander(BLEDevice central, BLECharacteristic characteristic)
{
    // central wrote new value to characteristic, update LED
    Serial.print("Servo A Speed characteristic event, written: ");
    Serial.println(byteToFloatPercent(servoASpeedCharactristic.value()));
    SetServo1Speed(byteToFloatPercent(servoASpeedCharactristic.value()));
}
void servoBSpeedIncomingHander(BLEDevice central, BLECharacteristic characteristic)
{
    // central wrote new value to characteristic, update LED
    Serial.print("Servo B Speed characteristic event, written: ");
    Serial.println(byteToFloatPercent(servoBSpeedCharactristic.value()));
    SetServo2Speed(byteToFloatPercent(servoBSpeedCharactristic.value()));
}
void expressionIncomingHander(BLEDevice central, BLECharacteristic characteristic)
{
    // central wrote new value to characteristic, update LED
    Serial.print("Expression characteristic event, written: ");
    Serial.println(expressionCharactristic.value());
    String str = expressionCharactristic.value();
    SetExpression(str);
}
void textMessageIncomingHander(BLEDevice central, BLECharacteristic characteristic)
{
    // central wrote new value to characteristic, update LED
    Serial.print("Text Message characteristic event, written: ");
    Serial.println(textMessageCharactristic.value());
    String str = textMessageCharactristic.value();
    ShowMessage(str);
}