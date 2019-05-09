#include "firebase.h"

Firebase::Firebase(ESP8266 *esp8266): m_esp8266(esp8266) {
  this->m_connected = false;
}

bool Firebase::auth(String firebaseHost, String firebaseToken) {
  this->m_connected = this->m_esp8266->initFirebase(firebaseHost, firebaseToken);

  return this->m_connected;
}

String Firebase::get(String path) {
  if (!this->m_connected) {
    return "";
  }

  String result = this->m_esp8266->get(path);
  result.trim();
  
  return result;
}

String Firebase::getString(String path) {
  if (!this->m_connected) {
    return "";
  }

  String result = this->m_esp8266->getString(path);
  result.trim();
  
  return result;
}