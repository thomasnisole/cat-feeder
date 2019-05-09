#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>
#include "esp8266.h"

class Wifi {
private:
  ESP8266 *m_esp8266;
  bool m_connected;
public:
  /**
   * Class constructor
   */
  Wifi(ESP8266 *esp8266);

  bool connect(String wifiSSID, String wifiPassword);
};

#endif
