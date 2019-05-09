#ifndef ESP8266_H
#define ESP8266_H

#include <Arduino.h>

#define COMMAND_CONNECT       "connect"
#define COMMAND_INIT_FIREBASE "init-firebase"
#define COMMAND_GET_TIME      "get-time"
#define COMMAND_GET           "get"
#define COMMAND_GET_STRING    "get-string"
#define COMMAND_STATUS_OK     "OK"
#define COMMAND_STATUS_KO     "KO"

class ESP8266 {
private:
  Stream *m_serial;
  String m_separator;

public:
  ESP8266(Stream *serial, String separator = ";");

  bool connect(String wifiSSID, String wifiPassword);

  bool initFirebase(String host, String token);

  String getTime(String timeZone);

  String get(String path);

  String getString(String path);

private:
  String executeCommand(String cmd);
};

#endif
