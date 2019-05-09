#include "feeder-repository.h"

#include <ArduinoJson.h>

FeederRepository::FeederRepository(Firebase *firebase): m_firebase(firebase) {

}

bool FeederRepository::findById(String id, Feeder *feeder) {
  String result = this->m_firebase->get("/feeders/" + id);
  Serial.println(result);
  if (result == "" || result == COMMAND_STATUS_KO) {
    return false;
  }

  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, result).code();

  if (error) {
    Serial.println(error.code());
    return false;
  }

  feeder->id = id;
  feeder->name = doc["name"].as<String>();
  feeder->forceFeed = doc["forceFeed"].as<bool>();
  
  if (!doc["lastFeed"].as<JsonObject>().isNull()) {
    feeder->lastFeed = new Date();
    feeder->lastFeed->minute  = doc["lastFeed"].as<JsonObject>()["minute"];
    feeder->lastFeed->hour    = doc["lastFeed"].as<JsonObject>()["hour"];
    feeder->lastFeed->day     = doc["lastFeed"].as<JsonObject>()["day"];
    feeder->lastFeed->month   = doc["lastFeed"].as<JsonObject>()["month"];
    feeder->lastFeed->year    = doc["lastFeed"].as<JsonObject>()["year"];
  } else {
    delete feeder->lastFeed;
  }

  if (!doc["currentDateTime"].as<JsonObject>().isNull()) {
    feeder->currentDateTime = new Date();
    feeder->currentDateTime->minute = doc["currentDateTime"].as<JsonObject>()["minute"];
    feeder->currentDateTime->hour   = doc["currentDateTime"].as<JsonObject>()["hour"];
    feeder->currentDateTime->day    = doc["currentDateTime"].as<JsonObject>()["day"];
    feeder->currentDateTime->month  = doc["currentDateTime"].as<JsonObject>()["month"];
    feeder->currentDateTime->year   = doc["currentDateTime"].as<JsonObject>()["year"];
  } else {
    delete feeder->currentDateTime;
  }

  feeder->alarms.clear();
  int alarmsCount = doc["alarms"].as<JsonArray>().size();
  for (int i = 0; i < alarmsCount; i++) {
    Alarm a;
    a.name    = doc["alarms"].as<JsonArray>()[i].as<JsonObject>()["name"].as<String>();
    a.minute  = doc["alarms"].as<JsonArray>()[i].as<JsonObject>()["minute"].as<int>();
    a.hour    = doc["alarms"].as<JsonArray>()[i].as<JsonObject>()["hour"].as<int>();
    a.skip    = doc["alarms"].as<JsonArray>()[i].as<JsonObject>()["skip"].as<bool>();
    feeder->alarms.push_back(a);
  }

  return true;
}

bool FeederRepository::updateFeeder(Feeder *feeder) {
  return false;
}