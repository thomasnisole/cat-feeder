#ifndef FEEDER_REPOSITORY_H
#define FEEDER_REPOSITORY_H

#include <firebase.h>
#include <json-feeder-deserializer.h>

#include "core.h"

class FeederRepository {
private:
  Firebase *m_firebase;
  JsonFeederDeserializer *m_jsonFeederDeserializer;
  
public:
  FeederRepository(Firebase *m_firebase, JsonFeederDeserializer *jsonFeederDeserializer);

  bool findById(String id, Feeder *feeder);
  bool updateFeeder(Feeder *feeder);
};

#endif
