#include <Arduino.h>
#include <LowPower.h>

#include <esp8266.h>
#include <feeder-manager.h>
#include <alarm-manager.h>

#define PIN_INTERRUPT_SYNC 2
#define PIN_INTERRUPT_FEED 3
#define PIN_INTERRUPT_FEED_ALARM 18
#define SERIAL_COMMUNICATION Serial3
#define COMMAND_SYNCHRONIZE "synchronize"
#define COMMAND_FEED "feed"

ESP8266 esp8266(&SERIAL_COMMUNICATION);
Wifi wifi(&esp8266);
Firebase firebase(&esp8266);
FeederRepository feederRepository(&firebase);
TimeApi timeApi(&esp8266);
FeederMotor feederMotor;
FeederManager feederManager(&wifi, &firebase, &feederRepository, &timeApi, &feederMotor);
String currentCommand = "";

void goingToSleep();
void synchronizeRequest();
void feedRequest();

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  while (!Serial);
  
  Serial.println("========== Begin ==========");

  SERIAL_COMMUNICATION.begin(9600);
  while (!SERIAL_COMMUNICATION);

  pinMode(PIN_INTERRUPT_SYNC, INPUT_PULLUP);
  pinMode(PIN_INTERRUPT_FEED_ALARM, INPUT);

  feederManager.synchronize();
}

void loop() {
  Serial.println("========== Loop ==========");
  Serial.print("Current command : ");
  Serial.println(currentCommand);
  if (currentCommand == COMMAND_SYNCHRONIZE) {
    currentCommand = "";
    feederManager.synchronize();
  } else if(currentCommand == COMMAND_FEED) {
    currentCommand = COMMAND_SYNCHRONIZE;
    feederManager.feed();
  }

  if (currentCommand == "") {
    goingToSleep();
  }
}

void goingToSleep() {
  Serial.println("Going to sleep");
  delay(500);

  attachInterrupt(digitalPinToInterrupt(PIN_INTERRUPT_SYNC), synchronizeRequest, LOW);
  attachInterrupt(digitalPinToInterrupt(PIN_INTERRUPT_FEED_ALARM), feedRequest, FALLING);

  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);

  Serial.println("Wake up");
  detachInterrupt(digitalPinToInterrupt(PIN_INTERRUPT_SYNC));
  detachInterrupt(digitalPinToInterrupt(PIN_INTERRUPT_FEED_ALARM));
}

void synchronizeRequest() {
  currentCommand = COMMAND_SYNCHRONIZE;
}

void feedRequest() {
  currentCommand = COMMAND_FEED;
}