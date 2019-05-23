#ifndef JSON_FEEDER_DESERIALIZER_H
#define JSON_FEEDER_DESERIALIZER_H

#include <Arduino.h>
#include <core.h>

class JsonFeederDeserializer {
public:
  bool deserialize(String from, Feeder *to);
};

#endif