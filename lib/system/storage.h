#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>
#include <EEPROM.h>
#include <json-feeder-serializer.h>
#include <json-feeder-deserializer.h>

#include <core.h>

class Storage {
private:
  JsonFeederSerializer *m_feederSerializer;
  JsonFeederDeserializer *m_feederDeserializer;

public:
  Storage(JsonFeederSerializer *feederSerializer, JsonFeederDeserializer *feederDeserializer);

  void read(Feeder &value);
  void save(Feeder &value);
  void clearAll();
};

#endif