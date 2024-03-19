//---------------------------------------------------------------------------------------
//--------------------------------Switch class ------------------------------------------
#include "relay_trigger.h"

class SwitchRelays {
  RelayTrigger start;
  RelayTrigger stop;
  CloudSwitch* cloud_switch;
  PinNumber status_in_pin;
  bool status;
  time_t last_status_check; 
  time_t status_check_interval;
  String name;
  Preferences* preferences;
 
  
public:  
  SwitchRelays(String _name, PinNumber start_Outpin, PinNumber stop_Outpin, CloudSwitch* _cloud_switch, PinNumber statusInpin, Preferences* _preferences, short interval  = 1000):
        start(_name+"SartRelay", start_Outpin, interval),
        stop(_name+"StopRelay", stop_Outpin, interval), 
        cloud_switch(_cloud_switch),
        status_in_pin(statusInpin),
        status(false), 
        last_status_check(millis()),
        status_check_interval( interval),
        name(_name+"SW"), 
        preferences(_preferences) {
            pinMode(status_in_pin, INPUT);
  }
  
  void Trigger() {
    Serial.printf("%s SWITCH EVENT RECEIVED CLOUD_VALUE = %hu LOCAL_STATE = %u \n", name, short(*cloud_switch), status );
    if(status == *cloud_switch) { 
      Serial.printf("%s SKIPPING... THE STATE UNCHANGED\n", name); 
      return; 
    }
    
    if(*cloud_switch){
      start.Trigger();
    } else {
      stop.Trigger();
    }

  }
  
  bool pin_status(byte in_pin){
    return (digitalRead(in_pin) == HIGH);
  }
  
  void status_check() {
    if (elapsed_time(last_status_check) >= status_check_interval) {
      status = pin_status(status_in_pin);
      
      if (*cloud_switch != status) {
          *cloud_switch = status;
          preferences->putBool(name.c_str(), status);
          Serial.printf("STATE OF %s SAVED TO FLASH MEMORY = %u\n", name, status);
      }
      last_status_check = millis();
    }
  }
  
  void checkInTheLoop() {
    start.checkToRelease(); 
    stop.checkToRelease();
    status_check();
  }
  
  void initStatus() {
    status = preferences->getBool(name.c_str());
    Serial.printf("STATE OF %s FROM FLASH MEMORY = %u\n", name, status);   
  }
  
};


