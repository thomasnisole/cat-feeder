#ifndef JSON_FEEDER_SERIALIZER_H
#define JSON_FEEDER_SERIALIZER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <core.h>

class JsonFeederSerializer {
public:
  bool serialize(Feeder from, String *to);
};

#endif