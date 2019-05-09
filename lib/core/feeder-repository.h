#ifndef FEEDER_REPOSITORY_H
#define FEEDER_REPOSITORY_H

#include <firebase.h>

#include "core.h"

class FeederRepository {
private:
  Firebase *m_firebase;
  
public:
  FeederRepository(Firebase *m_firebase);

  bool findById(String id, Feeder *feeder);

  bool updateFeeder(Feeder *feeder);
};

#endif
