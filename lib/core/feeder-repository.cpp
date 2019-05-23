#include "feeder-repository.h"

FeederRepository::FeederRepository(Firebase *firebase, JsonFeederDeserializer *jsonFeederDeserializer)
  : m_firebase(firebase), m_jsonFeederDeserializer(jsonFeederDeserializer) {

}

bool FeederRepository::findById(String id, Feeder *feeder) {
  String result = this->m_firebase->get("/feeders/" + id);
  Serial.println(result);
  if (result == "" || result == COMMAND_STATUS_KO) {
    return false;
  }

  if (!this->m_jsonFeederDeserializer->deserialize(result, feeder)) {
    return false;
  }

  feeder->id = id;
  
  return true;
}

bool FeederRepository::updateFeeder(Feeder *feeder) {
  return false;
}