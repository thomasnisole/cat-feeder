#include "esp8266.h"

ESP8266::ESP8266(Stream *serial, String separator = ";"): m_serial(serial), m_separator(separator) {

}

bool ESP8266::connect(String wifiSSID, String wifiPassword) {
  return this->executeCommand(
    COMMAND_CONNECT + this->m_separator + wifiSSID + this->m_separator + wifiPassword
    ) == COMMAND_STATUS_OK;
}

bool ESP8266::initFirebase(String host, String token) {
  return this->executeCommand(
    COMMAND_INIT_FIREBASE + this->m_separator + host + this->m_separator + token
    ) == COMMAND_STATUS_OK;
}

String ESP8266::getTime(String timeZone) {
  this->m_serial->println(COMMAND_GET_TIME + this->m_separator + timeZone);
  String result;
  do {
    result = this->m_serial->readString();
    result.trim();
  } while (result == "");

  return result;
}

String ESP8266::get(String path) {
  this->m_serial->println(COMMAND_GET + this->m_separator + path);
  String result;
  do {
    result = this->m_serial->readString();
    result.trim();
  } while (result == "");

  return result;
}

String ESP8266::getString(String path) {
  this->m_serial->println(COMMAND_GET_STRING + this->m_separator + path);
  String result;
  do {
    result = this->m_serial->readString();
    result.trim();
  } while (result == "");

  return result;
}

String ESP8266::executeCommand(String cmd) {
  this->m_serial->println(cmd);
  String result;
  do {
    result = this->m_serial->readString();
    result.trim();
  } while (result != COMMAND_STATUS_OK && result != COMMAND_STATUS_KO);

  return result;
}