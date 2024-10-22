#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <Preferences.h>
#include <ESPmDNS.h>
#include <AsyncJson.h>
#include <Thread.h>
#include "Network.h"
#include "Common.h"
#include "WebSocketService.h"
#include "Display.h"

AsyncWebServer server(80);
Preferences prefs;
String SoftAPSSID;

int numNetworks = 0;
void saveSSIDPassword(String &ssid, String &password)
{
  prefs.begin("settings.sys", false);
  prefs.putString("ssid", ssid);
  prefs.putString("pwd", password);
  prefs.end();
}
// Tries to connect to the provided station with SSID and pwd. if the connection takes longer than 10 seconds, reverts back to softap with systematic SSID
// if ssid is empty, reverts to SoftAP instantaneously.
void tryConnectToStation()
{
  String ssid, password;
  prefs.begin("settings.sys", false);
  ssid = prefs.getString("ssid");
  password = prefs.getString("pwd");
  prefs.end();

  if (ssid == "")
  {
    WiFi.mode(wifi_mode_t::WIFI_MODE_AP);
    Serial.println("No SSID provided, using SoftAP instead");
    WiFi.softAP(SoftAPSSID.c_str(), "" /*pwd.c_str()*/);
    IPAddress Ip(10, 0, 0, 1); // the device assumes this to be the server IP
    IPAddress NMask(255, 255, 255, 0);
    WiFi.softAPConfig(Ip, Ip, NMask);
    oled.clearDisplay();
    oled.setCursor(0, 5);
    oled.setTextSize(1);
    oled.println(SoftAPSSID);
    oled.println();
    oled.setTextSize(2);
    oled.println(Ip.toString());
    oled.display();
    return;
  }
  WiFi.mode(wifi_mode_t::WIFI_MODE_STA);
  WiFi.begin(ssid.c_str(), password.c_str());
  Thread([]()
         {
    long st = millis();
    prefs.begin("settings.sys", false);
    String ssid2 = prefs.getString("ssid");
    prefs.end();
    oled.clearDisplay();
    oled.setCursor(0, 4);
    oled.setTextSize(2);
    oled.print("Connecting WiFi ");
    oled.print(ssid2);
    oled.display();
    while (!WiFi.isConnected() && millis() - st < 10000)
    {
      oled.print(".");
      oled.display();
      Serial.print(".");
      delay(200);
    }
    if (WiFi.isConnected()){
      oled.clearDisplay();
      oled.setCursor(0, 5);
      oled.setTextSize(1);
      oled.println(ssid2);
      oled.println();
      oled.setTextSize(2);
      oled.println(WiFi.localIP().toString());
      oled.display();

      Serial.print("WiFi connected! IP: "); 
      Serial.println(WiFi.localIP().toString());
    } else {

      Serial.println("WiFi connection failed...!");
      Serial.println("Reverting to SoftAP");
      WiFi.mode(wifi_mode_t::WIFI_MODE_AP);
      WiFi.softAP(SoftAPSSID.c_str(), "" /*pwd.c_str()*/);
      IPAddress Ip(10, 0, 0, 1); // the device assumes this to be the server IP
      IPAddress NMask(255, 255, 255, 0);
      WiFi.softAPConfig(Ip, Ip, NMask);

    oled.clearDisplay();
    oled.setCursor(0, 5);
    oled.setTextSize(2);
    oled.println("Connection\nfailed");
    oled.display();
    delay(2000);

    oled.clearDisplay();
    oled.setCursor(0, 5);
    oled.setTextSize(1);
    oled.println(SoftAPSSID);
    oled.println();
    oled.setTextSize(2);
    oled.println(Ip.toString());
    oled.display();

    } })
      .Start();
}
void NetworkSetup()
{
  SoftAPSSID = String("M3D Go ") + String(getChipNumberShort());

  // Uncomment to stop connecting at startup
  // String clear;
  // saveSSIDPassword(clear, clear);
  tryConnectToStation();
  // Pre-scan for first scan
  numNetworks = WiFi.scanNetworks(false);

  server.on("/scan", WebRequestMethod::HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("Scan Started");
    DynamicJsonDocument doc(16);
    doc["success"] = true;
    String json_string;
    serializeJson(doc, json_string);
    request->send(200, "application/json", json_string);

    if (WiFi.scanComplete() != -1)
    {
      //log_error(1, "AP Mode: No Wifi Networks Found");
      WiFi.scanDelete();
      WiFi.scanNetworks(true);
    } });

  server.on("/results", WebRequestMethod::HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("Sending Results");
    numNetworks = WiFi.scanComplete();
    if (numNetworks > -1)
    {
      AsyncJsonResponse *response = new AsyncJsonResponse(false, 1024);
      JsonObject root = response->getRoot();
      JsonArray networks = root.createNestedArray("networks");
      for (int i = 0; i < numNetworks; i++)
      {
        Serial.print("SSID: ");
        Serial.println(WiFi.SSID(i));
        JsonObject network = networks.createNestedObject();
        network["rssi"] = WiFi.RSSI(i);
        network["ssid"] = WiFi.SSID(i);
        network["bssid"] = WiFi.BSSIDstr(i);
        network["channel"] = WiFi.channel(i);
        network["encryption_type"] = (uint8_t)WiFi.encryptionType(i);
      }
      root["success"] = true;
      root["rescan"] = false;
      response->setLength();
      request->send(response);
    }
    else
    {
      Serial.println("No SSIDs");
      DynamicJsonDocument doc(32);
      doc["success"] = true;
      doc["rescan"] = true;
      String json_string;
      serializeJson(doc, json_string);
      request->send(200, "application/json", json_string);
    } });

  // Connect to Internet - Version 2
  server.on("/connect", HTTP_PUT, [](AsyncWebServerRequest *request)
            {
    String ssid;
    String password;
    //Check SSID
    if (request->hasParam("ssid", true))
    {
      ssid = request->getParam("ssid", true)->value();
    }
    else
    {
      DynamicJsonDocument doc(128);
      doc["success"] = false;
      doc["error"] = "No SSID Found";
      String json_string;
      serializeJson(doc, json_string);
      request->send(200, "application/json", json_string);
      return;
    }

    if (request->hasParam("password", true))
    {
      password = request->getParam("password", true)->value();
    }

    //Print SSID and Password
    Serial.print("SSID: ");
    Serial.println(ssid);
    Serial.print("Password: ");
    Serial.println(password);

    //Send Response Back
    AsyncJsonResponse *response = new AsyncJsonResponse(false, 1024);
    JsonObject root = response->getRoot();
    root["success"] = true;
    response->setLength();
    request->send(response);
    Serial.println("Connect response sent");
  saveSSIDPassword(ssid, password);
  tryConnectToStation();
  Serial.println("Connect call returned"); });

  server.on("/connection_test", WebRequestMethod::HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("connection_test");
      DynamicJsonDocument doc(128);
      doc["success"] = true;
      doc["IsConnected"] = WiFi.isConnected();
      String json_string;
      serializeJson(doc, json_string);
      request->send(200, "application/json", json_string); });

  server.serveStatic("/", LittleFS, "/www").setDefaultFile("index.htm");
  server.addHandler(&ws);

  // server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
  //   int params = request->params();
  //   String ssid, pass, ip, gateway;
  //   for(int i=0;i<params;i++){
  //     AsyncWebParameter* p = request->getParam(i);
  //     if(p->isPost()){
  //       // HTTP POST ssid value
  //       if (p->name() == "ssid") {
  //         ssid = p->value().c_str();
  //         Serial.print("SSID set to: ");
  //         Serial.println(ssid);
  //         prefs.begin("settings.sys");
  //         prefs.putString("ssid", ssid);
  //         prefs.end();
  //       }
  //       // HTTP POST pass value
  //       if (p->name() == "pass") {
  //         pass = p->value().c_str();
  //         Serial.print("Password set to: ");
  //         Serial.println(pass);
  //         prefs.begin("settings.sys");
  //         prefs.putString("ssid", ssid);
  //         prefs.putString("pass", pass);
  //         prefs.end();
  //       }
  //       Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
  //     }
  //   }
  //   WiFi.begin(ssid.c_str(), pass.c_str());
  //   request->redirect("scratch.markhor3d.com");
  // });
  server.begin();
}

void NetworkLoop()
{
}
