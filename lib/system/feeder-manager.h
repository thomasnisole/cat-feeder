#ifndef FEEDER_MANAGER_H
#define FEEDER_MANAGER_H

#include <Arduino.h>
#include <feeder-repository.h>

#include "wifi.h"
#include "firebase.h"
#include "time-api.h"
#include "feeder-motor.h"
#include "alarm-manager.h"
#include "storage.h"

#define WIFI_SSID       "kereon"
#define WIFI_PASSWORD   "camerappelleunevieillecopine"
#define FIREBASE_HOST   "cat-feeder-66c26.firebaseio.com"
#define FIREBASE_TOKEN  "1OMWIJPzgentPSNAEoTYPZTzLwrlfCWI8ht0MqtL"

#define WIFI_CONNECT_TIMEOUT      5
#define FIREBASE_AUTH_TIMEOUT     5
#define FIREBASE_COMMAND_TIMEOUT  5

#define TIME_ZONE "Europe/Paris"

#define ALARM_FEEDER_NUMBER 1

class FeederManager {
private:
  Wifi *m_wifi;
  Firebase *m_firebase;
  FeederRepository *m_feederRepository;
  TimeApi *m_timeApi;
  FeederMotor *m_feederMotor;
  AlarmManager *m_alarmManager;
  Storage *m_storage;

  Feeder *m_feeder;

public:
  FeederManager(Wifi *wifi, Firebase *firebase, FeederRepository *feederRepository, TimeApi *timeApi, FeederMotor *feederMotor, AlarmManager *alarmManager, Storage *storage);
  ~FeederManager();

  void synchronize();
  void feed();

private:
  bool initConnections();
  bool initWifiConnection();
  bool initFirebaseConnection();
  bool initDateTime();
  bool getFeeder(String id);
  void initFeederAlarm();
};

#endif