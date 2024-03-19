
#pragma once
#include "limits.h"
#include <Preferences.h>

typedef byte PinNumber;

unsigned long elapsed_time( unsigned long from_timestamp ){
  long change = millis() - from_timestamp;
  if(change >= 0 ) {
    return change;
  } else {
    return ULONG_MAX + change;
  }
}
//--------------------------------------------------------------------------
//----------------------------- RelayTrigger Class -------------------------
//--------------------------------------------------------------------------
class RelayTrigger {
  PinNumber pin_number;
  short release_after;
  bool triggered;
  time_t triggered_time_stamp;
  
protected:
    String name;
    
public:
  
  RelayTrigger(String _name, PinNumber _pin_number, short _release_after=1000):
              pin_number(_pin_number),
              release_after(_release_after),
              triggered(false),
              triggered_time_stamp(0),
              name(_name) {
                  pinMode(pin_number, OUTPUT);
                  digitalWrite(pin_number, HIGH);
  }
  
  void Trigger() { 
    digitalWrite( pin_number, LOW);
    triggered_time_stamp = millis();
    triggered = true;
    Serial.printf("TRIGGERED %s\n", name);
  }
 
  void checkToRelease(){
    if(!triggered) return;
    if( elapsed_time(triggered_time_stamp) > release_after ){
      digitalWrite(pin_number, HIGH);
      triggered = false;
      Serial.printf("RELEASED %s\n", name);
    }
  }
  
};

//--------------------------------------------------------------------------
//-------------------------- RelayLazyTrigger Class ------------------------
//--------------------------------------------------------------------------
class RelayLazyTrigger : public RelayTrigger{
  time_t trigger_after;
  bool late_trigger;
  time_t request_time_stamp;
  
public:
  RelayLazyTrigger(String _name, PinNumber _pin_number, short _release_after=1000):
                  RelayTrigger(_name, _pin_number, _release_after),
                  trigger_after(0),
                  late_trigger(false),
                  request_time_stamp(0){}
  
  void TriggerAfter(time_t after_millis){
      request_time_stamp = millis();
      trigger_after = after_millis;
      late_trigger = true;
      Serial.printf("The %s REQUESTED TRIGGER LATE AT %lu FOR AFTER %lu \n", name, request_time_stamp, after_millis );
  }
  
   void checkForLaterTrigger(){
    if(!late_trigger) return;
    if( elapsed_time(request_time_stamp) > trigger_after ){
      late_trigger = false;
      Serial.printf("The %s TRIGGERED LATE\n", name);
      Trigger();
    }
  }

};

