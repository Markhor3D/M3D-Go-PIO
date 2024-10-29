#include "bluetooth.h"
#include <ArduinoBLE.h>
#include "Common.h"
#include "Mechanical.h"
#include "Display.h"
#include <vector>
String BLEName;

// create service and characteristics:
BLEService m3dScratchLinkService("8e088cd2-8000-11ee-b9d1-0242ac120002");
BLEByteCharacteristic proximityACharacteristic("8e088cd2-7001-11ee-b9d1-0242ac120002", BLERead | BLENotify);
BLEByteCharacteristic proximityBCharacteristic("8e088cd2-7002-11ee-b9d1-0242ac120002", BLERead | BLENotify);
BLEByteCharacteristic distanceCharacteristic("8e088cd2-7003-11ee-b9d1-0242ac120002", BLERead | BLENotify);
BLEByteCharacteristic leftMotorCharacteristic("8e088cd2-6001-11ee-b9d1-0242ac120002", BLEWriteWithoutResponse);
BLEByteCharacteristic rightMotorCharacteristic("8e088cd2-6002-11ee-b9d1-0242ac120002", BLEWriteWithoutResponse);
BLEByteCharacteristic servoAAngleCharacteristic("8e088cd2-6003-11ee-b9d1-0242ac120002", BLEWriteWithoutResponse);
BLEByteCharacteristic servoBAngleCharacteristic("8e088cd2-6004-11ee-b9d1-0242ac120002", BLEWriteWithoutResponse);
BLEByteCharacteristic servoASpeedCharacteristic("8e088cd2-6005-11ee-b9d1-0242ac120002", BLEWriteWithoutResponse);
BLEByteCharacteristic servoBSpeedCharacteristic("8e088cd2-6006-11ee-b9d1-0242ac120002", BLEWriteWithoutResponse);
BLEStringCharacteristic expressionCharacteristic("8e088cd2-6007-11ee-b9d1-0242ac120002", BLEWriteWithoutResponse, 16);
BLEStringCharacteristic textMessageCharacteristic("8e088cd2-6008-11ee-b9d1-0242ac120002", BLEWriteWithoutResponse, 16);

// PIO Core characteristics
BLEService pioRemoteLinkService("8e088cd2-5000-11ee-b9d1-0242ac120002");
BLEStringCharacteristic remoteNotifyString0Characteristic("8e088cd2-4001-11ee-b9d1-0242ac120002", BLERead | BLENotify, 64);  // Title
BLEStringCharacteristic remoteNotifyString1Characteristic("8e088cd2-4002-11ee-b9d1-0242ac120002", BLERead | BLENotify, 32);  // String value/ Button 1
BLEStringCharacteristic remoteNotifyString2Characteristic("8e088cd2-4003-11ee-b9d1-0242ac120002", BLERead | BLENotify, 16);  // Button 2
BLEStringCharacteristic remoteNotifyString3Characteristic("8e088cd2-4004-11ee-b9d1-0242ac120002", BLERead | BLENotify, 16);  // Button 3
BLEStringCharacteristic remoteNotifyString4Characteristic("8e088cd2-4005-11ee-b9d1-0242ac120002", BLERead | BLENotify, 16);  // Button 4
BLEStringCharacteristic remoteNotifyString5Characteristic("8e088cd2-4006-11ee-b9d1-0242ac120002", BLERead | BLENotify, 16);  // Button 5
BLEFloatCharacteristic remoteNotifyFloatCharacteristic("8e088cd2-4007-11ee-b9d1-0242ac120002", BLERead | BLENotify); // Float Value
BLEByteCharacteristic remoteProcessNotifyRequestCharacteristic("8e088cd2-4008-11ee-b9d1-0242ac120002",  BLERead | BLENotify);
BLEFloatCharacteristic remoteReturnFloatCharacteristic("8e088cd2-4009-11ee-b9d1-0242ac120002", BLEWriteWithoutResponse); // returning float
BLEStringCharacteristic remoteReturnStringCharacteristic("8e088cd2-4010-11ee-b9d1-0242ac120002", BLEWriteWithoutResponse, 32); // returning string



void connectHandler(BLEDevice central);
void disconnectHandler(BLEDevice central);
bool isDeviceNameTaken(String name);

void leftMotorIncomingHandler(BLEDevice central, BLECharacteristic characteristic);
void rightMotorIncomingHandler(BLEDevice central, BLECharacteristic characteristic);
void servoAAngleIncomingHandler(BLEDevice central, BLECharacteristic characteristic);
void servoBAngleIncomingHandler(BLEDevice central, BLECharacteristic characteristic);
void servoASpeedIncomingHandler(BLEDevice central, BLECharacteristic characteristic);
void servoBSpeedIncomingHandler(BLEDevice central, BLECharacteristic characteristic);
void expressionIncomingHandler(BLEDevice central, BLECharacteristic characteristic);
void textMessageIncomingHandler(BLEDevice central, BLECharacteristic characteristic);
void remoteReturnFloatIncomingHandler(BLEDevice central, BLECharacteristic characteristic);
void remoteReturnStringIncomingHandler(BLEDevice central, BLECharacteristic characteristic);

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
    BLE.setAdvertisedService(pioRemoteLinkService);

    // add the characteristics to the service:
    m3dScratchLinkService.addCharacteristic(proximityACharacteristic);
    m3dScratchLinkService.addCharacteristic(proximityBCharacteristic);
    m3dScratchLinkService.addCharacteristic(distanceCharacteristic);
    m3dScratchLinkService.addCharacteristic(leftMotorCharacteristic);
    m3dScratchLinkService.addCharacteristic(rightMotorCharacteristic);
    m3dScratchLinkService.addCharacteristic(servoAAngleCharacteristic);
    m3dScratchLinkService.addCharacteristic(servoBAngleCharacteristic);
    m3dScratchLinkService.addCharacteristic(servoASpeedCharacteristic);
    m3dScratchLinkService.addCharacteristic(servoBSpeedCharacteristic);
    m3dScratchLinkService.addCharacteristic(expressionCharacteristic);
    m3dScratchLinkService.addCharacteristic(textMessageCharacteristic);
    
    pioRemoteLinkService.addCharacteristic(remoteNotifyString0Characteristic);
    pioRemoteLinkService.addCharacteristic(remoteNotifyString1Characteristic);
    pioRemoteLinkService.addCharacteristic(remoteNotifyString2Characteristic);
    pioRemoteLinkService.addCharacteristic(remoteNotifyString3Characteristic);
    pioRemoteLinkService.addCharacteristic(remoteNotifyString4Characteristic);
    pioRemoteLinkService.addCharacteristic(remoteNotifyString5Characteristic);
    pioRemoteLinkService.addCharacteristic(remoteNotifyFloatCharacteristic);
    pioRemoteLinkService.addCharacteristic(remoteProcessNotifyRequestCharacteristic);
    pioRemoteLinkService.addCharacteristic(remoteReturnFloatCharacteristic);
    pioRemoteLinkService.addCharacteristic(remoteReturnStringCharacteristic);

    // assign event handlers for connected, disconnected to peripheral
    BLE.setEventHandler(BLEConnected, connectHandler);
    BLE.setEventHandler(BLEDisconnected, disconnectHandler);

    // assign event handler for characteristics:

    leftMotorCharacteristic.setEventHandler(BLEWritten, leftMotorIncomingHandler);
    rightMotorCharacteristic.setEventHandler(BLEWritten, rightMotorIncomingHandler);
    servoAAngleCharacteristic.setEventHandler(BLEWritten, servoAAngleIncomingHandler);
    servoBAngleCharacteristic.setEventHandler(BLEWritten, servoBAngleIncomingHandler);
    servoASpeedCharacteristic.setEventHandler(BLEWritten, servoASpeedIncomingHandler);
    servoBSpeedCharacteristic.setEventHandler(BLEWritten, servoBSpeedIncomingHandler);
    expressionCharacteristic.setEventHandler(BLEWritten, expressionIncomingHandler);
    textMessageCharacteristic.setEventHandler(BLEWritten, textMessageIncomingHandler);

    remoteReturnFloatCharacteristic.setEventHandler(BLEWritten, remoteReturnFloatIncomingHandler);
    remoteReturnStringCharacteristic.setEventHandler(BLEWritten, remoteReturnStringIncomingHandler);

    // add the service and set a value for the characteristic:
    BLE.addService(m3dScratchLinkService);
    BLE.addService(pioRemoteLinkService);
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
            proximityACharacteristic.setValue(floatPercentToByte(Sensor_ProximityA));
        if (ProximityBSent != Sensor_ProximityB || forcedSend)
            proximityBCharacteristic.setValue(floatPercentToByte(Sensor_ProximityB));
        if (DistanceSent != Sensor_Distance || forcedSend)
            distanceCharacteristic.setValue(floatPercentToByte(round(Sensor_Distance)));
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
    CurrentCommunicationChannel = CommunicationType::NotConnected;
}

void leftMotorIncomingHandler(BLEDevice central, BLECharacteristic characteristic)
{
    // central wrote new value to characteristic, update LED
    Serial.print("Left Motor characteristic event, written: ");
    Serial.println(byteToFloatPercent(leftMotorCharacteristic.value()) / 100);
    SetLeftMotor(byteToFloatPercent(leftMotorCharacteristic.value()) / 100);
}
void rightMotorIncomingHandler(BLEDevice central, BLECharacteristic characteristic)
{
    // central wrote new value to characteristic, update LED
    Serial.print("Right Motor characteristic event, written: ");
    Serial.println(byteToFloatPercent(rightMotorCharacteristic.value()) / 100);
    SetRightMotor(byteToFloatPercent(rightMotorCharacteristic.value()) / 100);
}
void servoAAngleIncomingHandler(BLEDevice central, BLECharacteristic characteristic)
{
    // central wrote new value to characteristic, update LED
    Serial.print("Servo A Angle characteristic event, written: ");
    Serial.println(byteToFloatPercent(servoAAngleCharacteristic.value()));
    SetServoA(byteToFloatPercent(servoAAngleCharacteristic.value()));
}
void servoBAngleIncomingHandler(BLEDevice central, BLECharacteristic characteristic)
{
    // central wrote new value to characteristic, update LED
    Serial.print("Servo B Angle characteristic event, written: ");
    Serial.println(byteToFloatPercent(servoBAngleCharacteristic.value()));
    SetServoB(byteToFloatPercent(servoBAngleCharacteristic.value()));
}
void servoASpeedIncomingHandler(BLEDevice central, BLECharacteristic characteristic)
{
    // central wrote new value to characteristic, update LED
    Serial.print("Servo A Speed characteristic event, written: ");
    Serial.println(byteToFloatPercent(servoASpeedCharacteristic.value()));
    SetServo1Speed(byteToFloatPercent(servoASpeedCharacteristic.value()));
}
void servoBSpeedIncomingHandler(BLEDevice central, BLECharacteristic characteristic)
{
    // central wrote new value to characteristic, update LED
    Serial.print("Servo B Speed characteristic event, written: ");
    Serial.println(byteToFloatPercent(servoBSpeedCharacteristic.value()));
    SetServo2Speed(byteToFloatPercent(servoBSpeedCharacteristic.value()));
}
void expressionIncomingHandler(BLEDevice central, BLECharacteristic characteristic)
{
    // central wrote new value to characteristic, update LED
    Serial.print("Expression characteristic event, written: ");
    Serial.println(expressionCharacteristic.value());
    String str = expressionCharacteristic.value();
    SetExpression(str);
}
void textMessageIncomingHandler(BLEDevice central, BLECharacteristic characteristic)
{
    // central wrote new value to characteristic, update LED
    Serial.print("Text Message characteristic event, written: ");
    Serial.println(textMessageCharacteristic.value());
    String str = textMessageCharacteristic.value();
    ShowMessage(str);
}
bool aRemoteValueHasReturned = false;
float remoteFloatValueReturned;
String remoteStringValueReturned;
void remoteReturnFloatIncomingHandler(BLEDevice central, BLECharacteristic characteristic){
    // central wrote new value to characteristic, update LED
    Serial.print("Remote Float returned: ");
    Serial.println(remoteReturnFloatCharacteristic.value());
    remoteFloatValueReturned = remoteReturnFloatCharacteristic.value();
    aRemoteValueHasReturned = true;
}
void remoteReturnStringIncomingHandler(BLEDevice central, BLECharacteristic characteristic){
    // central wrote new value to characteristic, update LED
    Serial.print("Remote String returned: ");
    Serial.println(remoteReturnStringCharacteristic.value());
    remoteStringValueReturned = remoteReturnStringCharacteristic.value();
    aRemoteValueHasReturned = true;
}