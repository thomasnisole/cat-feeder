#include "json-feeder-deserializer.h"

#include <ArduinoJson.h>

bool JsonFeederDeserializer::deserialize(String from, Feeder *to) {
  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, from).code();

  if (error) {
    Serial.println(error.code());
    return false;
  }

  to->name = doc["name"].as<String>();
  to->forceFeed = doc["forceFeed"].as<bool>();
  
  if (!doc["lastFeed"].as<JsonObject>().isNull()) {
    to->lastFeed = new Date();
    to->lastFeed->minute  = doc["lastFeed"].as<JsonObject>()["minute"];
    to->lastFeed->hour    = doc["lastFeed"].as<JsonObject>()["hour"];
    to->lastFeed->day     = doc["lastFeed"].as<JsonObject>()["day"];
    to->lastFeed->month   = doc["lastFeed"].as<JsonObject>()["month"];
    to->lastFeed->year    = doc["lastFeed"].as<JsonObject>()["year"];
  } else {
    delete to->lastFeed;
  }

  if (!doc["currentDateTime"].as<JsonObject>().isNull()) {
    to->currentDateTime = new Date();
    to->currentDateTime->minute = doc["currentDateTime"].as<JsonObject>()["minute"];
    to->currentDateTime->hour   = doc["currentDateTime"].as<JsonObject>()["hour"];
    to->currentDateTime->day    = doc["currentDateTime"].as<JsonObject>()["day"];
    to->currentDateTime->month  = doc["currentDateTime"].as<JsonObject>()["month"];
    to->currentDateTime->year   = doc["currentDateTime"].as<JsonObject>()["year"];
  } else {
    delete to->currentDateTime;
  }

  to->alarms.clear();
  int alarmsCount = doc["alarms"].as<JsonArray>().size();
  for (int i = 0; i < alarmsCount; i++) {
    Alarm a;
    a.name    = doc["alarms"].as<JsonArray>()[i].as<JsonObject>()["name"].as<String>();
    a.minute  = doc["alarms"].as<JsonArray>()[i].as<JsonObject>()["minute"].as<int>();
    a.hour    = doc["alarms"].as<JsonArray>()[i].as<JsonObject>()["hour"].as<int>();
    a.skip    = doc["alarms"].as<JsonArray>()[i].as<JsonObject>()["skip"].as<bool>();
    to->alarms.push_back(a);
  }

  return true;
}