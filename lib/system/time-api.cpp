#include "time-api.h"

#include <ArduinoJson.h>

TimeApi::TimeApi(ESP8266 *esp8266): m_esp8266(esp8266) {

}

bool TimeApi::getDateTime(String timeZone, Date *date) {
  String result = this->m_esp8266->getTime(timeZone);
  result.trim();

  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, result).code();

  if (error) {
    Serial.println(error.code());
    return false;
  }

  String dateTime = doc["datetime"].as<String>();

  date->year    = dateTime.substring(0, 4).toInt();
  date->month   = dateTime.substring(5, 7).toInt();
  date->day     = dateTime.substring(8, 10).toInt();
  date->hour    = dateTime.substring(11, 13).toInt();
  date->minute  = dateTime.substring(14, 16).toInt();

  date->dayOfWeek = doc["day_of_week"].as<String>().toInt();

  return true;
}