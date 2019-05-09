#ifndef ALARM_MANAGER_H
#define ALARM_MANAGER_H

#include <Wire.h>
#include <RTClibExtended.h>
#include <core.h>

class AlarmManager {
private:
  RTC_DS3231 rtc;

public:
  AlarmManager();

  void initDate(Date date);
  Date getDate();
  void setTimeAlarm(int alarmNumber, Time time);
  void clearAlarm(int alarmNumber);
  void clearAlarms();
};

#endif