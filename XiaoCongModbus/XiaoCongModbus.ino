#define PT_USE_TIMER
#define PT_USE_SEM

#include "tinyexpr.h"
#include <ESP8266WiFi.h>
#include "pt.h"
#include "Crypto.h"
#include "mybase64.h"
#include <Wire.h>
#include "ModbusRtu.h"
#include <ArduinoJson.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>
#include "ESPAsyncUDP.h"
#include <Ticker.h>
#include <Hash.h>
#include <FS.h>
#include "RTClib.h"
#include "EEPROM.h"
#include <multi_button.h>
#include <AsyncMqttClient.h>
#include <ESPAsyncTCP.h>
#include <ArduinoJson.h>
#include <DS3231.h>
#include <SoftwareSerial.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <time.h>

#include "I2Cdev.h"
#include <OneWire.h>

#include "main.h"
#include "function_v2.h"
#include "function.h"
#include "key.h"
#include "httpserver.h"
#include "mqtt.h"
#include "periodic.h"
#include "init.h"
#include "getdata.h"



void setup()
{
  device_init();
  PT_INIT(&thread_modbus);
  PT_INIT(&thread_periodic);
}
void loop()
{
  threadRs485_entry(&thread_modbus);
  threadPeriodic_entry(&thread_periodic);
}
















