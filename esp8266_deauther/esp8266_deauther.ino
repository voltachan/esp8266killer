/*
   ===========================================
      Copyright (c) 2018 Stefan Kremser
             github.com/spacehuhn
   ===========================================
*/

extern "C" {
  // Please follow this tutorial:
  // https://github.com/spacehuhn/esp8266_deauther/wiki/Installation#compiling-using-arduino-ide
  // And be sure to have the right board selected
#include "user_interface.h"
}
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <WiFiClient.h>
#if ARDUINOJSON_VERSION_MAJOR != 5
// The software was build using ArduinoJson v5.x
// version 6 is still in beta at the time of writing
// go to tools -> manage libraries, search for ArduinoJSON and install the latest version 5
#endif

#include "oui.h"
#include "language.h"
#include "functions.h"
#include "Settings.h"
#include "Names.h"
#include "SSIDs.h"
#include "Scan.h"
#include "Attack.h"
#include "CLI.h"
#include "DisplayUI.h"
#include "A_config.h"
#include "webfiles.h"

#include "LED.h"

// Run-Time Variables //
LED led;
Settings settings;
Names    names;
SSIDs    ssids;
Accesspoints accesspoints;
Stations     stations;
Scan   scan;
Attack attack;
CLI    cli;
DisplayUI displayUI;

#include "wifi.h"

uint32_t autosaveTime = 0;
uint32_t currentTime  = 0;

bool booted = false;

void setup() {
  randomSeed(os_random());

  // 启动串行接口（波特率：115200）
  Serial.begin(115200);
  Serial.println();

  // SPIFFS 文件系统
  prnt(SETUP_MOUNT_SPIFFS);
  prntln(SPIFFS.begin() ? SETUP_OK : SETUP_ERROR);

  // EEPROM 初始化
  EEPROM.begin(4096);

  // 自动修复？？？
  uint8_t bootCounter = EEPROM.read(0);

  if (bootCounter >= 3) {
    prnt(SETUP_FORMAT_SPIFFS);
    SPIFFS.format();
    prntln(SETUP_OK);
  } else {
    EEPROM.write(0, bootCounter + 1); // Boot失败超过3次
    EEPROM.commit();
  }

  // 获取当前时间
  currentTime = millis();

  // 加载设置
  settings.load();

  // 设置AP的MAC地址？？？
  wifi_set_macaddr(SOFTAP_IF, settings.getMacAP());

  // 启动Wi-Fi
  WiFi.mode(WIFI_OFF);
  wifi_set_opmode(STATION_MODE);//默认以Station模式启动Wi-Fi
  wifi_set_promiscuous_rx_cb([](uint8_t* buf, uint16_t len) {
    scan.sniffer(buf, len);
  });

  // 设置Station的MAC地址
  wifi_set_macaddr(STATION_IF, settings.getMacSt());

  // start display
  if (settings.getDisplayInterface()) {
    displayUI.setup();
    displayUI.mode = displayUI.DISPLAY_MODE::INTRO;
  }

  // 复制网页文件到SPIFFS
  copyWebFiles(false);

  // 加载配置项
  names.load();
  ssids.load();
  cli.load();

  // 创建 scan.json
  scan.setup();

  // 设置Wi-Fi信道
  setWifiChannel(settings.getChannel());

  // 加载Wi-Fi的配置SSID, password,...
#ifdef DEFAULT_SSID
  if (settings.getSSID() == "pwned") settings.setSSID(DEFAULT_SSID);
#endif // ifdef DEFAULT_SSID
  loadWifiConfigDefaults();

  // 串行接口开/关切换
  if (settings.getCLI()) {
    cli.enable();
  } else {
    prntln(SETUP_SERIAL_WARNING);
    Serial.flush();
    Serial.end();
  }

  // 启动Web界面
  if (settings.getWebInterface()) startAP();
  // STARTED
  //prntln(SETUP_STARTED);
  // version
  //prntln(settings.getVersion());

  // 亮灯
  led.setup();
  //delay(10000);
  //cli.exec("attack -b");
}

void loop() {
  currentTime = millis();

  //各种各样的状态更新
  led.update();    // update LED color
  wifiUpdate();    // manage access point
  attack.update(); // run attacks
  displayUI.update();
  cli.update();    // read and run serial input
  scan.update();   // run scan
  ssids.update();  // run random mode, if enabled

  // 自动保存
  if (settings.getAutosave() && (currentTime - autosaveTime > settings.getAutosaveTime())) {
    autosaveTime = currentTime;
    names.save(false);
    ssids.save(false);
    settings.save(false);
  }

  if (!booted) {
    // 重置Boot次数
    EEPROM.write(0, 0);
    EEPROM.commit();
    booted = true;
#ifdef HIGHLIGHT_LED
    displayUI.setupLED();
#endif // ifdef HIGHLIGHT_LED
  }
}
