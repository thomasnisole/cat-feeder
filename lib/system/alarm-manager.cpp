#include "alarm-manager.h"

AlarmManager::AlarmManager() {
  Wire.begin();
  this->rtc.begin();
}

void AlarmManager::initDate(Date date) {
  DateTime dt(date.year, date.month, date.day, date.hour, date.minute, 0);
  this->rtc.adjust(dt);
  this->rtc.writeSqwPinMode(DS3231_OFF);
}

Date AlarmManager::getDate() {
  DateTime dt = this->rtc.now();
  
  Date d;
  d.year = dt.year();
  d.month = dt.month();
  d.day = dt.day();
  d.hour = dt.hour();
  d.minute = dt.minute();

  return d;
}

void AlarmManager::setTimeAlarm(int alarmNumber, Time time) {
  switch (alarmNumber) {
    case 1:
      this->rtc.setAlarm(ALM1_MATCH_HOURS, 0, time.minute, time.hour, 0);
      this->rtc.alarmInterrupt(1, true);
      break;

    case 2:
      Serial.println(time.c_str());
      this->rtc.setAlarm(ALM2_MATCH_HOURS, time.minute, time.hour, 0);
      this->rtc.alarmInterrupt(2, true);
      break;
  }
}

void AlarmManager::clearAlarm(int alarmNumber) {
  this->rtc.armAlarm(alarmNumber, false);
  this->rtc.clearAlarm(alarmNumber);
  this->rtc.alarmInterrupt(alarmNumber, false);
}

void AlarmManager::clearAlarms() {
  this->clearAlarm(1);
  this->clearAlarm(2);
}