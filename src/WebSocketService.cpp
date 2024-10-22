#include "WebSocketService.h"
#include "ScratchServer.h"
#include <ESPAsyncWebServer.h>
#include "Display.h"
#include "Common.h"
#include "Mechanical.h"

AsyncWebSocket ws("/ws");
void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  if (type == WS_EVT_CONNECT)
  {
    CurrentCommunicationChannel = CommunicationType::WiFiWebSockets;
    Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
    client->printf("Hello Client %u :)", client->id());
    client->ping();
  }
  else if (type == WS_EVT_DISCONNECT)
  {
    Serial.printf("ws[%s][%u] disconnect\n", server->url(), client->id());
  }
  else if (type == WS_EVT_ERROR)
  {
    Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t *)arg), (char *)data);
  }
  else if (type == WS_EVT_PONG)
  {
    Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len) ? (char *)data : "");
  }
  else if (type == WS_EVT_DATA)
  {
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    String msg = "";
    if (info->final && info->index == 0 && info->len == len)
    {
      // the whole message is in a single frame and we got all of it's data
      Serial.printf("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT) ? "text" : "binary", info->len);

      if (info->opcode == WS_TEXT)
      {
        for (size_t i = 0; i < info->len; i++)
        {
          msg += (char)data[i];
        }
      }
      else
      {
        char buff[3];
        for (size_t i = 0; i < info->len; i++)
        {
          sprintf(buff, "%02x ", (uint8_t)data[i]);
          msg += buff;
        }
      }
      Serial.printf("%s\n", msg.c_str());
      parseScratchCommand(msg);
    }
    else
    { // not used at the moment
      // message is comprised of multiple frames or the frame is split into multiple packets
      if (info->index == 0)
      {
        if (info->num == 0)
          Serial.printf("ws[%s][%u] %s-message start\n", server->url(), client->id(), (info->message_opcode == WS_TEXT) ? "text" : "binary");
        Serial.printf("ws[%s][%u] frame[%u] start[%llu]\n", server->url(), client->id(), info->num, info->len);
      }

      Serial.printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT) ? "text" : "binary", info->index, info->index + len);

      if (info->opcode == WS_TEXT)
      {
        for (size_t i = 0; i < len; i++)
        {
          msg += (char)data[i];
        }
      }
      else
      {
        char buff[3];
        for (size_t i = 0; i < len; i++)
        {
          sprintf(buff, "%02x ", (uint8_t)data[i]);
          msg += buff;
        }
      }
      Serial.printf("%s\n", msg.c_str());

      if ((info->index + len) == info->len)
      {
        Serial.printf("ws[%s][%u] frame[%u] end[%llu]\n", server->url(), client->id(), info->num, info->len);
        if (info->final)
        {
          Serial.printf("ws[%s][%u] %s-message end\n", server->url(), client->id(), (info->message_opcode == WS_TEXT) ? "text" : "binary");
          if (info->message_opcode == WS_TEXT)
            client->text("I got your text message");
          else
            client->binary("I got your binary message");
        }
      }
    }
  }
}

void WebSocketsSetup()
{
  ws.onEvent(onWsEvent);
}
long lastSensorsSent = 0;
float SentSensors[8] = {-1};
void WebSocketsLoop()
{
  ws.cleanupClients();
  // if (WiFi.status() == WL_CONNECTED)
  {
    if (millis() - lastSensorsSent > 20 && CurrentCommunicationChannel == CommunicationType::WiFiWebSockets)
    {
      bool atleast1Changed = false;
      float* Sensors[] = {&Sensor_ProximityA, &Sensor_ProximityB, &Sensor_Distance};
      for (int i = 0; i < 3; i++)
      {
        if (*Sensors[i] != SentSensors[i])
          atleast1Changed = true;
        SentSensors[i] = *Sensors[i];
      }
      if (atleast1Changed)
      {
        // if(ws.count() > 0){
        //if (ws.availableForWriteAll()) // this stops everything
        {
          // ws.textAll("sensor 0 = " + String(Sensors[0]));
          String all = "s";
          for (int i = 0; i < 3; i++)
            all += String(":") + String(*Sensors[i]);
          ws.textAll(all);
          lastSensorsSent = millis();
        }
      }
    }
  }
}