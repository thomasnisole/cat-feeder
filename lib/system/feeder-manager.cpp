#include "feeder-manager.h"

FeederManager::FeederManager(Wifi *wifi, Firebase *firebase, FeederRepository *feederRepository, TimeApi *timeApi, FeederMotor *feederMotor, AlarmManager *alarmManager, Storage *storage):
  m_wifi(wifi), m_firebase(firebase), m_feederRepository(feederRepository), m_timeApi(timeApi), m_feederMotor(feederMotor), m_alarmManager(alarmManager), m_storage(storage) {
  this->m_feeder = new Feeder();
}

FeederManager::~FeederManager() {
  this->m_wifi              = NULL;
  this->m_firebase          = NULL;
  this->m_feederRepository  = NULL;
  this->m_timeApi           = NULL;
  this->m_feederMotor       = NULL;
  this->m_alarmManager      = NULL;
  this->m_storage      = NULL;
  
  delete this->m_feeder;
  this->m_feeder            = NULL;
}

void FeederManager::synchronize() {
  digitalWrite(LED_BUILTIN, HIGH);

  /*this->m_storage->read(*this->m_feeder);
  Serial.println(this->m_feeder->c_str());*/

  if (!this->initConnections()) return;
  Serial.println("Successfully connected.");

  if (!this->initDateTime()) return;
  Serial.println("Date time initialized.");

  if (!this->getFeeder("0")) return;
  Serial.println((*this->m_feeder).c_str());

  /*this->m_storage->clearAll();
  this->m_storage->save(*this->m_feeder);*/

  this->m_alarmManager->clearAlarms();
  Serial.println("Alarms cleared.");

  this->initFeederAlarm();
  Serial.println("Alarm feeder armed.");

  digitalWrite(LED_BUILTIN, LOW);
}

void FeederManager::feed() {
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.print("Feed at : ");
  Serial.println(this->m_alarmManager->getDate().c_str());

  this->initFeederAlarm();

  digitalWrite(LED_BUILTIN, LOW);
}

bool FeederManager::initConnections() {
  if (!this->initWifiConnection()) {
    Serial.println("An error occurred during Wifi connection.");

    return false;
  }

  if (!this->initFirebaseConnection()) {
    Serial.println("An error occurred during Firebase connection.");

    return false;
  }

  return true;
}

bool FeederManager::initWifiConnection() {
  Serial.println("Launch wifi connection");

  int timeout = 0;
  while (!this->m_wifi->connect(WIFI_SSID, WIFI_PASSWORD) && timeout < WIFI_CONNECT_TIMEOUT) timeout++;

  return timeout < WIFI_CONNECT_TIMEOUT;
}

bool FeederManager::initFirebaseConnection() {
  Serial.println("Launch firebase connection");

  int timeout = 0;
  while (!this->m_firebase->auth(FIREBASE_HOST, FIREBASE_TOKEN) && timeout < FIREBASE_AUTH_TIMEOUT) timeout++;
  
  return timeout < FIREBASE_AUTH_TIMEOUT;
}

bool FeederManager::initDateTime() {
  int timeout = 0;
  Date d;

  while (!this->m_timeApi->getDateTime(TIME_ZONE, &d) && timeout < FIREBASE_COMMAND_TIMEOUT) timeout++;

  if (timeout >= FIREBASE_COMMAND_TIMEOUT) {
    return false;
  }

  this->m_alarmManager->initDate(d);

  Serial.println(d.c_str());

  return true;
}

bool FeederManager::getFeeder(String id) {
  int timeout = 0;

  while (!this->m_feederRepository->findById(id, this->m_feeder) && timeout < FIREBASE_COMMAND_TIMEOUT) timeout++;

  return timeout < FIREBASE_COMMAND_TIMEOUT;
}

void FeederManager::initFeederAlarm() {
  Date d = this->m_alarmManager->getDate();
  Serial.print("Current Date : ");
  Serial.println(d.c_str());
  Alarm alarm = this->m_feeder->findNextFeedAlarm(d);
  Serial.println("Next alarm : ");
  Serial.println(alarm.c_str());
  this->m_alarmManager->clearAlarm(ALARM_FEEDER_NUMBER);
  this->m_alarmManager->setTimeAlarm(ALARM_FEEDER_NUMBER, alarm);
}