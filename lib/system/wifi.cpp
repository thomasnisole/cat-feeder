#include "wifi.h"

Wifi::Wifi(ESP8266 *esp8266): m_esp8266(esp8266) {
  this->m_connected =false;
}

bool Wifi::connect(String wifiSSID, String wifiPassword) {
  this->m_connected = this->m_esp8266->connect(wifiSSID, wifiPassword);

  return this->m_connected;
}