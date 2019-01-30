#ifndef WifiManager_h
#define WifiManager_h

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>
extern "C" {
#include "user_interface.h"
}

#define WIFI_MODE_OFF 0
#define WIFI_MODE_AP 1
#define WIFI_MODE_STATION 2

/*
   This file contains all necessary functions for hosting and connecting to an access point.
   For compatibility and simplicity, all those functions are global.
 */

// Important strings
const char W_DEAUTHER[] PROGMEM = "captive.apple.com"; // captive portal domain (alternative to 192.168.4.1)
const char W_WEBINTERFACE[] PROGMEM = "/web";  // default folder containing the web files
const char W_ERROR_PASSWORD[] PROGMEM = "ERROR: Password must have at least 8 characters!";
const char W_DEFAULT_LANG[] PROGMEM = "/lang/default.lang";

const char W_HTML[] PROGMEM = "text/html";
const char W_CSS[] PROGMEM = "text/css";
const char W_JS[] PROGMEM = "application/javascript";
const char W_PNG[] PROGMEM = "image/png";
const char W_GIF[] PROGMEM = "image/gif";
const char W_JPG[] PROGMEM = "image/jpeg";
const char W_ICON[] PROGMEM = "image/x-icon";
const char W_XML[] PROGMEM = "text/xml";
const char W_XPDF[] PROGMEM = "application/x-pdf";
const char W_XZIP[] PROGMEM = "application/x-zip";
const char W_GZIP[] PROGMEM = "application/x-gzip";
const char W_JSON[] PROGMEM = "application/json";
const char W_TXT[] PROGMEM = "text/plain";

const char W_DOT_HTM[] PROGMEM = ".htm";
const char W_DOT_HTML[] PROGMEM = ".html";
const char W_DOT_CSS[] PROGMEM = ".css";
const char W_DOT_JS[] PROGMEM = ".js";
const char W_DOT_PNG[] PROGMEM = ".png";
const char W_DOT_GIF[] PROGMEM = ".gif";
const char W_DOT_JPG[] PROGMEM = ".jpg";
const char W_DOT_ICON[] PROGMEM = ".ico";
const char W_DOT_XML[] PROGMEM = ".xml";
const char W_DOT_PDF[] PROGMEM = ".pdf";
const char W_DOT_ZIP[] PROGMEM = ".zip";
const char W_DOT_GZIP[] PROGMEM = ".gz";
const char W_DOT_JSON[] PROGMEM = ".json";

// Server and other global objects
ESP8266WebServer server(80);
DNSServer dnsServer;
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);
File fsUploadFile;

// current WiFi mode and config
uint8_t wifiMode = WIFI_MODE_OFF;

bool   wifi_config_hidden        = false;
bool   wifi_config_captivePortal = false;
String wifi_config_ssid;
String wifi_config_password;
String wifi_config_path;

void stopAP() {
    if (wifiMode == WIFI_MODE_AP) {
        wifi_promiscuous_enable(0);
        WiFi.persistent(false);
        WiFi.disconnect(true);
        wifi_set_opmode(STATION_MODE);
        prntln(W_STOPPED_AP);
        wifiMode = WIFI_MODE_STATION;
    }
}

void wifiUpdate() {
    if ((wifiMode != WIFI_MODE_OFF) && !scan.isScanning()) {
        server.handleClient();
        dnsServer.processNextRequest();
    }
}

String getWifiMode() {
    switch (wifiMode) {
    case WIFI_MODE_OFF:
        return W_MODE_OFF;

        break;

    case WIFI_MODE_AP:
        return W_MODE_AP;

        break;

    case WIFI_MODE_STATION:
        return W_MODE_ST;

        break;

    default:
        return String();
    }
}

String getContentType(String filename) {
    if (server.hasArg("download")) return String(F("application/octet-stream"));
    else if (filename.endsWith(str(W_DOT_GZIP))) filename = filename.substring(0, filename.length() - 3);
    else if (filename.endsWith(str(W_DOT_HTM))) return str(W_HTML);
    else if (filename.endsWith(str(W_DOT_HTML))) return str(W_HTML);
    else if (filename.endsWith(str(W_DOT_CSS))) return str(W_CSS);
    else if (filename.endsWith(str(W_DOT_JS))) return str(W_JS);
    else if (filename.endsWith(str(W_DOT_PNG))) return str(W_PNG);
    else if (filename.endsWith(str(W_DOT_GIF))) return str(W_GIF);
    else if (filename.endsWith(str(W_DOT_JPG))) return str(W_JPG);
    else if (filename.endsWith(str(W_DOT_ICON))) return str(W_ICON);
    else if (filename.endsWith(str(W_DOT_XML))) return str(W_XML);
    else if (filename.endsWith(str(W_DOT_PDF))) return str(W_XPDF);
    else if (filename.endsWith(str(W_DOT_ZIP))) return str(W_XZIP);
    else if (filename.endsWith(str(W_DOT_JSON))) return str(W_JSON);
    else return str(W_TXT);
}

bool handleFileRead(String path) {
    //prnt(W_AP_REQUEST);
    //prnt(path);

    if (!path.charAt(0) == SLASH) path = String(SLASH) + path;

    if (path.charAt(path.length() - 1) == SLASH) path += String(F("index.html"));

    String contentType = getContentType(path);

    if (!SPIFFS.exists(path)) {
        if (SPIFFS.exists(path + str(W_DOT_GZIP))) path += str(W_DOT_GZIP);
        else if (SPIFFS.exists(wifi_config_path + path)) path = wifi_config_path + path;
        else if (SPIFFS.exists(wifi_config_path + path + str(W_DOT_GZIP))) path = wifi_config_path + path + str(
                W_DOT_GZIP);
        else {
            // prntln(W_NOT_FOUND);
            return false;
        }
    }

    File file = SPIFFS.open(path, "r");
    server.streamFile(file, contentType);
    file.close();
    //prnt(SPACE);
    //prntln(W_OK);

    return true;
}

void handleFileList() {
    if (!server.hasArg("dir")) {
        server.send(500, str(W_TXT), str(W_BAD_ARGS));
        return;
    }

    String path = server.arg("dir");
    // Serial.println("handleFileList: " + path);
    Dir dir = SPIFFS.openDir(path);

    String output = String(OPEN_BRACKET); // {
    File   entry;
    bool   first = true;

    while (dir.next()) {
        entry = dir.openFile("r");

        if (first) first = false;
        else output += COMMA;                                                 // ,

        output += OPEN_BRACKET;                                               // [
        output += String(DOUBLEQUOTES) + entry.name() + String(DOUBLEQUOTES); // "filename"
        output += CLOSE_BRACKET;                                              // ]

        entry.close();
    }

    output += CLOSE_BRACKET;
    server.send(200, str(W_JSON).c_str(), output);
}

void sendProgmem(const char* ptr, size_t size, const char* type) {
    server.sendHeader("Content-Encoding", "gzip");
    server.sendHeader("Cache-Control", "max-age=86400");
    server.send_P(200, str(type).c_str(), ptr, size);
}

// path = folder of web files, ssid = name of network, password = password ("0" => no password), hidden = if the network
// is visible, captivePortal = enable a captive portal
void startAP(String path, String ssid, String password, uint8_t ch, bool hidden, bool captivePortal) {
    if (password.length() < 8) {
        prntln(W_ERROR_PASSWORD);
        return;
    }

    if (!path.charAt(0) == SLASH) path = String(SLASH) + path;

    if (password == String(ZERO)) password = String(NEWLINE);

    wifi_config_path     = path;
    wifi_config_ssid     = ssid;
    wifi_config_password = password;
    setWifiChannel(ch);
    wifi_config_hidden        = hidden;
    wifi_config_captivePortal = captivePortal;

    WiFi.softAPConfig(apIP, apIP, netMsk);
    WiFi.softAP(ssid.c_str(), password.c_str(), wifi_channel, hidden);

    dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer.start(53, String(ASTERIX), apIP);

    MDNS.begin(str(W_DEAUTHER).c_str());

    server.on(String(F("/list")).c_str(), HTTP_GET, handleFileList); // list directory


    // ================================================================
    // post here the output of the webConverter.py
#ifdef USE_PROGMEM_WEB_FILES
if(!settings.getWebSpiffs()){
  server.on(String(SLASH).c_str(), HTTP_GET, [](){
  sendProgmem(indexhtml, sizeof(indexhtml), W_HTML);
});
server.on(String(F("/attack.html")).c_str(), HTTP_GET, [](){
  sendProgmem(attackhtml, sizeof(attackhtml), W_HTML);
});
server.on(String(F("/index.html")).c_str(), HTTP_GET, [](){
  sendProgmem(indexhtml, sizeof(indexhtml), W_HTML);
});
server.on(String(F("/scan.html")).c_str(), HTTP_GET, [](){
  sendProgmem(scanhtml, sizeof(scanhtml), W_HTML);
});
server.on(String(F("/settings.html")).c_str(), HTTP_GET, [](){
  sendProgmem(settingshtml, sizeof(settingshtml), W_HTML);
});
server.on(String(F("/ssids.html")).c_str(), HTTP_GET, [](){
  sendProgmem(ssidshtml, sizeof(ssidshtml), W_HTML);
});
server.on(String(F("/base.css")).c_str(), HTTP_GET, [](){
  sendProgmem(basecss, sizeof(basecss), W_CSS);
});
server.on(String(F("/bootstrap.css")).c_str(), HTTP_GET, [](){
  sendProgmem(bootstrapcss, sizeof(bootstrapcss), W_CSS);
});
server.on(String(F("/dashbord.css")).c_str(), HTTP_GET, [](){
  sendProgmem(dashbordcss, sizeof(dashbordcss), W_CSS);
});
server.on(String(F("/font.css")).c_str(), HTTP_GET, [](){
  sendProgmem(fontcss, sizeof(fontcss), W_CSS);
});
server.on(String(F("/material.css")).c_str(), HTTP_GET, [](){
  sendProgmem(materialcss, sizeof(materialcss), W_CSS);
});
server.on(String(F("/project.css")).c_str(), HTTP_GET, [](){
  sendProgmem(projectcss, sizeof(projectcss), W_CSS);
});
server.on(String(F("/js/attack.js")).c_str(), HTTP_GET, [](){
  sendProgmem(attackjs, sizeof(attackjs), W_JS);
});
server.on(String(F("/js/base.js")).c_str(), HTTP_GET, [](){
  sendProgmem(basejs, sizeof(basejs), W_JS);
});
server.on(String(F("/js/canvasnest.js")).c_str(), HTTP_GET, [](){
  sendProgmem(canvasnestjs, sizeof(canvasnestjs), W_JS);
});
server.on(String(F("/js/jquery.js")).c_str(), HTTP_GET, [](){
  sendProgmem(jqueryjs, sizeof(jqueryjs), W_JS);
});
server.on(String(F("/js/jqueryvalidate.js")).c_str(), HTTP_GET, [](){
  sendProgmem(jqueryvalidatejs, sizeof(jqueryvalidatejs), W_JS);
});
server.on(String(F("/js/material.js")).c_str(), HTTP_GET, [](){
  sendProgmem(materialjs, sizeof(materialjs), W_JS);
});
server.on(String(F("/js/materialdashboard.js")).c_str(), HTTP_GET, [](){
  sendProgmem(materialdashboardjs, sizeof(materialdashboardjs), W_JS);
});
server.on(String(F("/js/project.js")).c_str(), HTTP_GET, [](){
  sendProgmem(projectjs, sizeof(projectjs), W_JS);
});
server.on(String(F("/js/scan.js")).c_str(), HTTP_GET, [](){
  sendProgmem(scanjs, sizeof(scanjs), W_JS);
});
server.on(String(F("/js/scroll.js")).c_str(), HTTP_GET, [](){
  sendProgmem(scrolljs, sizeof(scrolljs), W_JS);
});
server.on(String(F("/js/settings.js")).c_str(), HTTP_GET, [](){
  sendProgmem(settingsjs, sizeof(settingsjs), W_JS);
});
server.on(String(F("/js/site.js")).c_str(), HTTP_GET, [](){
  sendProgmem(sitejs, sizeof(sitejs), W_JS);
});
server.on(String(F("/js/ssids.js")).c_str(), HTTP_GET, [](){
  sendProgmem(ssidsjs, sizeof(ssidsjs), W_JS);
});
};
#endif
    // ================================================================

    server.on(String(F("/run")).c_str(), HTTP_GET, []() {
        server.send(200, str(W_TXT), str(W_OK).c_str());
        String input = server.arg("cmd");
        cli.exec(input);
    });

    server.on(String(F("/attack.json")).c_str(), HTTP_GET, []() {
        server.send(200, str(W_JSON), attack.getStatusJSON());
    });

    // aggressively caching static assets
    server.serveStatic("/js", SPIFFS, String(wifi_config_path + "/js").c_str(), "max-age=86400");

    // called when the url is not defined here
    // use it to load content from SPIFFS
    server.onNotFound([]() {
        if (!handleFileRead(server.uri())) {
            server.send(404, str(W_TXT), str(W_FILE_NOT_FOUND));
        }
    });

    server.begin();
    wifiMode = WIFI_MODE_AP;

    prntln(W_STARTED_AP);
    printWifiStatus();
}

void printWifiStatus() {
    prnt(String(F("[WiFi] Path: '")));
    prnt(wifi_config_path);
    prnt(String(F("', Mode: '")));

    switch (wifiMode) {
    case WIFI_MODE_OFF:
        prnt(W_MODE_OFF);
        break;

    case WIFI_MODE_AP:
        prnt(W_AP);
        break;

    case WIFI_MODE_STATION:
        prnt(W_STATION);
        break;
    }
    prnt(String(F("', SSID: '")));
    prnt(wifi_config_ssid);
    prnt(String(F("', password: '")));
    prnt(wifi_config_password);
    prnt(String(F("', channel: '")));
    prnt(wifi_channel);
    prnt(String(F("', hidden: ")));
    prnt(b2s(wifi_config_hidden));
    prnt(String(F(", captive-portal: ")));
    prntln(b2s(wifi_config_captivePortal));
}

void startAP() {
    startAP(wifi_config_path.c_str(), wifi_config_ssid.c_str(),
            wifi_config_password.c_str(), wifi_channel, wifi_config_hidden, wifi_config_captivePortal);
}

void startAP(String path) {
    wifi_config_path = path;
    startAP();
}

void loadWifiConfigDefaults() {
    wifi_config_hidden        = settings.getHidden();
    wifi_config_ssid          = settings.getSSID();
    wifi_config_password      = settings.getPassword();
    wifi_config_captivePortal = settings.getCaptivePortal();
    wifi_config_path          = str(W_WEBINTERFACE);
}

void resumeAP() {
    if (wifiMode != WIFI_MODE_AP) {
        wifiMode = WIFI_MODE_AP;
        wifi_promiscuous_enable(0);
        WiFi.softAPConfig(apIP, apIP, netMsk);
        WiFi.softAP(wifi_config_ssid.c_str(), wifi_config_password.c_str(), wifi_channel, wifi_config_hidden);
        prntln(W_STARTED_AP);
    }
}

#endif // ifndef WifiManager_h
