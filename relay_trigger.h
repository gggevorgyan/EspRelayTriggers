
#pragma once
#include "limits.h"
#include <Preferences.h>
#include "pin_writers.h"


// ------------ class RelayTrigger ------------- //
template<class PinWriter>
class RelayTrigger {
  uint8_t pin_number;
  short release_after;
  bool triggered;
  unsigned long triggered_time_stamp;
protected:
  PinWriter* pw;
  String name;

public:
  RelayTrigger(String _name, PinNumber _pin_number, PinWriter* _pw, short _release_after = 1000)
    : pin_number(_pin_number),
      release_after(_release_after),
      triggered(false),
      triggered_time_stamp(0),
      pw(_pw),
      name(_name) {
    pw->setOutputMode(pin_number);
    pw->setHIGH(pin_number);
  }

  void Trigger() {
    pw->setHIGH(pin_number);
    triggered_time_stamp = millis();
    triggered = true;
    Serial.printf("- TRIGGERED %s\n", name);
  }

  void checkToRelease() {
    if (!triggered) return;
    if (elapsed_time(triggered_time_stamp) > release_after) {
      pw->setLOW(pin_number);
      triggered = false;
      Serial.printf("- RELEASED %s\n", name);
    }
  }
};

// ------------ class RelayLazyTrigger ------------- //

template<class PinWriter>
class RelayLazyTrigger : public RelayTrigger<PinWriter> {
  int trigger_after;
  bool late_trigger;
  unsigned long request_time_stamp;

public:
  RelayLazyTrigger(String _name, PinNumber _pin_number, PinWriter* _pw, short _release_after = 1000)
    : RelayTrigger<PinWriter>(_name, _pin_number, _pw, _release_after),
      trigger_after(0),
      late_trigger(false),
      request_time_stamp(0) {}

  void TriggerAfter(unsigned long after_millis) {
    request_time_stamp = millis();
    trigger_after = after_millis;
    late_trigger = true;
    Serial.printf(" %s >> REQUESTED TRIGGER LATE AT %lu FOR AFTER %lu \n", this->name, request_time_stamp, after_millis);
  }

  void checkForLaterTrigger() {
    if (!late_trigger) return;

    if (elapsed_time(request_time_stamp) > trigger_after) {
      late_trigger = false;
      Serial.printf("%s >> TRIGGERED LATE\n", this->name);
      this->Trigger();
    }
  }
};
