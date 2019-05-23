#include "storage.h"

Storage::Storage(JsonFeederSerializer *feederSerializer, JsonFeederDeserializer *feederDeserializer): 
  m_feederSerializer(feederSerializer), m_feederDeserializer(feederDeserializer) {

}

void Storage::read(Feeder &value) {
  String *fromExtraction = new String();
  EEPROM.get(0, *fromExtraction);
  this->m_feederDeserializer->deserialize(*fromExtraction, &value);
  
}

void Storage::save(Feeder &value) {
  String *toSave = new String();
  this->m_feederSerializer->serialize(value, toSave);
  EEPROM.put(0, *toSave);
}

void Storage::clearAll() {
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
}