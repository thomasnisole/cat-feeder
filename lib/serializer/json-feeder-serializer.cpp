#include "json-feeder-serializer.h"

#include <ArduinoJson.h>

bool JsonFeederSerializer::serialize(Feeder from, String *to) {
  DynamicJsonDocument doc(2048);

  doc["name"] = from.name;
  doc["forceFeed"] = from.forceFeed;

  if (from.lastFeed != NULL) {
    JsonObject object = doc.createNestedObject("lastFeed");
    object["minute"] = from.lastFeed->minute;
    object["hour"] = from.lastFeed->hour;
    object["day"] = from.lastFeed->day;
    object["month"] = from.lastFeed->month;
    object["year"] = from.lastFeed->year;
  }

  if (from.currentDateTime != NULL) {
    JsonObject object = doc.createNestedObject("currentDateTime");
    object["minute"] = from.currentDateTime->minute;
    object["hour"] = from.currentDateTime->hour;
    object["day"] = from.currentDateTime->day;
    object["month"] = from.currentDateTime->month;
    object["year"] = from.currentDateTime->year;
  }

  JsonArray alarms = doc.createNestedArray("alarms");
  for (int i = 0; i < from.alarms.size(); ++i) {
    JsonObject object = alarms.createNestedObject();
    object["minute"] = from.alarms[i].minute;
    object["hour"] = from.alarms[i].hour;
    object["skip"] = from.alarms[i].skip;
    object["name"] = from.alarms[i].name;
  }

  serializeJson(doc, *to);

  return true;
}