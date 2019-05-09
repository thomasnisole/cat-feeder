#ifndef FIREBASE_H
#define FIREBASE_H

#include <Arduino.h>
#include "esp8266.h"

class Firebase {
private:
  ESP8266 *m_esp8266;
  bool m_connected;
public:
  /**
   * Class constructor
   */
  Firebase(ESP8266 *esp8266);

  bool auth(String firebaseHost, String firebaseToken);

  String get(String path);

  String getString(String path);
};

#endif
