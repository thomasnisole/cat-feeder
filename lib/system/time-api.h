#ifndef TIME_API_H
#define TIME_API_H

#include <Arduino.h>
#include <core.h>
#include "esp8266.h"

class TimeApi {
private:
  ESP8266 *m_esp8266;

public:
  TimeApi(ESP8266 *esp8266);

  bool getDateTime(String timeZone, Date *date);
};

#endif