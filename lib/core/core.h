#ifndef CORE_H
#define CORE_H

#include <Arduino.h>
#include <vector>

using namespace std; 

struct Time {
  int hour;
  int minute;

  const String c_str() {
    return (hour < 10 ? "0" : "") + String(hour) + ":" + 
      (minute < 10 ? "0" : "") + String(minute);
  }
};

struct Date: Time {
  int day;
  int month;
  int year;
  int dayOfWeek;

  const String c_str() {
    return String(year) + "-" + 
      (month < 10 ? "0" : "") + String(month) + "-" + 
      (day < 10 ? "0" : "") + String(day) + " " + 
      Time::c_str();
  }
};

struct Alarm: Time {
  String name;
  bool skip;

  const String c_str() {
    return name + ",\r\n" +
      (skip ? "skip" : "not skip") + ",\r\n" +
      Time::c_str();
  }
};

struct Feeder {
  String id;
  String name;
  bool forceFeed;
  Date *lastFeed;
  Date *currentDateTime;
  vector<Alarm> alarms;

  Feeder() {
    lastFeed = NULL;
    currentDateTime = NULL;
  }

  ~Feeder() {
    delete lastFeed;
    delete currentDateTime;
  }

  Alarm findNextFeedAlarm(Time currentTime) {
    int index = -1;
    unsigned int i = 0;

    while (i < this->alarms.size() && index == -1) {
      if (this->alarms[i].hour < currentTime.hour) {
        i++;
        continue;
      }

      if (this->alarms[i].hour == currentTime.hour) {
        if (this->alarms[i].minute > currentTime.minute) {
          index = i;
          continue;
        } else {
          i++;
          continue;
        }
      }

      if (this->alarms[i].hour > currentTime.hour) {
        index = i;
        continue;
      }
    }

    if (index == -1) {
      index = 0;
    }

    return this->alarms[index];
  }

  const String c_str() {
    String str = id + ",\r\n" +
      name + ",\r\n" +
      (forceFeed ? "forceFeed" : "not forceFeed") + ",\r\n";

    if (lastFeed) {
      str += lastFeed->c_str() + ",\r\n";
    }

    if (currentDateTime) {
      str += currentDateTime->c_str() + ",\r\n";
    }

    for (size_t i = 0; i < this->alarms.size(); ++i) {
      str += "Alarm ";
      str += i; 
      str += " :\r\n " + this->alarms[i].c_str() + "\r\n";
    }

    return str;
  }
};

#endif