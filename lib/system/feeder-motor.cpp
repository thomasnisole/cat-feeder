#include "feeder-motor.h"

FeederMotor::FeederMotor() {

}

void FeederMotor::feed() {
  digitalWrite(LED_BUILTIN, HIGH);

  delay (2000);

  digitalWrite(LED_BUILTIN, LOW);
}