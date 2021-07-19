#include <Arduino.h>
#include <ArduinoLog.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <FS.h>
#include <LittleFS.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>
#include "JuraInterface.h"
#include "WiFiHelper.h"

#ifndef STASSID
#define STASSID "Pizza"
#define STAPSK "4kaese3xsalami"
#endif

AsyncWebServer server(80);
JuraInterface juraMachine;

const char *ssid = STASSID;
const char *password = STAPSK;

bool juraRUN = false;
String juraCMD;

// void printTimestamp(Print *_logOutput) {
//   char c[12];
//   int m = sprintf(c, "%10lu ", millis());
//   _logOutput->print(c);
// }

String cmd;
String result;
String cmd2jura(String outbytes);

void recvMsg(uint8_t *data, size_t len)
{
  WebSerial.println("Received Data...");
  String d = "";
  for (int i = 0; i < len; i++)
  {
    d += char(data[i]);
  }
  WebSerial.println(d);
}

String porcessorPlaceholder(const String &var)
{
  if (var == "DEVICE_NAME")
  {
    return WiFi.getHostname();
  }
  if (var == "DEVICE_SSID")
  {
    return WiFi.SSID();
  }
  if (var == "DEVICE_WIFISTATUS")
  {
    return wl_status_to_string(WiFi.status());
  }
  if (var == "DEVICE_WIFICHANNEL")
  {
    return (String)WiFi.channel();
  }
  if (var == "DEVICE_WIFISIGNALSTRENGTH")
  {
    return (String)WiFi.RSSI();
  }
  if (var == "DEVICE_IP")
  {
    return WiFi.localIP().toString().c_str();
  }
  if (var == "DEVICE_MAC")
  {
    return WiFi.macAddress();
  }
  if (var == "DEVICE_UPTIME")
  {
    int uptime_s = millis() / 1000;
    return (String)uptime_s;
  }
  return String();
}

void setup()
{

  Serial.begin(115200);
  juraMachine.init();
  //softserial.begin(9600, SWSERIAL_8N1, JURA_RX, JURA_TX);
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);
  Log.trace("Booting...");

  WiFi.mode(WIFI_STA);
  WiFi.hostname("JuraConnect");
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Log.notice("Connection Failed! Rebooting..." CR);
    delay(5000);
    ESP.restart();
  }

  LittleFS.begin();
  //server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/index.html", String(), false); });

  server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/index.html", String(), false); });

  server.on("/systemInfo.html", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/systemInfo.html", String(), false, porcessorPlaceholder); });

  server.on("/footer.html", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/footer.html", String(), false); });

  server.on("/xTechBodensee_Logo.png", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/xTechBodensee_Logo.png", String(), false); });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/style.css", "text/css"); });

  server.on("/csi.min.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/csi.min.js", String(), false); });

  server.on("/navbar.html", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/navbar.html", String(), false); });

  server.on("/jc_power_on", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              Log.notice("ONButton Pressed!" CR);
              juraMachine.juraCMD = "AN:01\r\n";
              juraMachine.juraRUN_Enable = true;
              request->send(LittleFS, "/index.html", String(), false);
            });

  server.on("/jc_power_on", HTTP_POST, [](AsyncWebServerRequest *request)
            {
              Log.notice("ONButton Pressed!" CR);
              juraMachine.juraCMD = "AN:01\r\n";
              juraMachine.juraRUN_Enable = true;
            });

  server.on("/jc_power_off", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              juraMachine.juraCMD = "AN:02\r\n";
              juraMachine.juraRUN_Enable = true;
              request->send(LittleFS, "/index.html", "", false);
            });

  server.on("/jc_water_clean", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              juraMachine.juraCMD = "FA:02\r\n";
              juraMachine.juraRUN_Enable = true;
              request->send(LittleFS, "/index.html", String(), false);
            });

  server.on("/jc_coffee_onesmall", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              juraMachine.juraCMD = "FA:04\r\n";
              juraMachine.juraRUN_Enable = true;
              request->send(LittleFS, "/index.html", String(), false);
            });

  server.on("/jc_coffee_twosmall", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              juraMachine.juraCMD = "FA:05\r\n";
              juraMachine.juraRUN_Enable = true;
              request->send(LittleFS, "/index.html", String(), false);
            });

  server.on("/jc_coffee_onelarge", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              juraMachine.juraCMD = "FA:06\r\n";
              juraMachine.juraRUN_Enable = true;
              request->send(LittleFS, "/index.html", String(), false);
            });

  server.on("/jc_coffee_twolarge", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              juraMachine.juraCMD = "FA:07\r\n";
              juraMachine.juraRUN_Enable = true;
              request->send(LittleFS, "/index.html", String(), false);
            });

  server.on("/jc_coffee_onehuge", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              juraMachine.juraCMD = "FA:0C\r\n";
              juraMachine.juraRUN_Enable = true;
              request->send(LittleFS, "/index.html", String(), false);
            });

  WebSerial.begin(&server);
  WebSerial.msgCallback(recvMsg);

  server.begin();

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]()
                     {
                       String type;
                       if (ArduinoOTA.getCommand() == U_FLASH)
                       {
                         type = "sketch";
                       }
                       else
                       { // U_FS
                         type = "filesystem";
                       }

                       // NOTE: if updating FS this would be the place to unmount FS using FS.end()
                       Serial.println("Start updating " + type);
                     });
  ArduinoOTA.onEnd([]()
                   { Serial.println("\nEnd"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                        { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });
  ArduinoOTA.onError([](ota_error_t error)
                     {
                       Serial.printf("Error[%u]: ", error);
                       if (error == OTA_AUTH_ERROR)
                       {
                         Serial.println("Auth Failed");
                       }
                       else if (error == OTA_BEGIN_ERROR)
                       {
                         Serial.println("Begin Failed");
                       }
                       else if (error == OTA_CONNECT_ERROR)
                       {
                         Serial.println("Connect Failed");
                       }
                       else if (error == OTA_RECEIVE_ERROR)
                       {
                         Serial.println("Receive Failed");
                       }
                       else if (error == OTA_END_ERROR)
                       {
                         Serial.println("End Failed");
                       }
                     });
  ArduinoOTA.begin();

  Log.trace("...Booted" CR);
  Log.notice("IP address: %p" CR, WiFi.localIP());
}

void loop()
{
  ArduinoOTA.handle();

  juraMachine.run();
}
