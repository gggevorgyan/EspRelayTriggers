#include "relay_trigger.h"

//---------------------------------------------------------------------------------------
//-------------------------------- Slider3Relays class ----------------------------------
//---------------------------------------------------------------------------------------
class Slider3Relays {
  String name;
  short delta_threshold;
  time_t start_to_end_duration; 
  short slider_range;
  short step_time;

  RelayTrigger forward;
  RelayTrigger back;
  RelayLazyTrigger stop;
  
  CloudPercentage* cloud_obj;
  
  PinNumber forward_status_pin;
  PinNumber back_status_pin;
  
  short current_position;
  time_t last_status_check; 
  time_t status_check_interval;
  bool allWayBack;
  bool allWayForward;
  
  Preferences* preferences;
 
public:  
  Slider3Relays( String _name, 
                  PinNumber forward_Outpin, 
                  PinNumber back_Outpin, 
                  PinNumber stop_Outpin, 
                  CloudPercentage* _cloud_obj, 
                  PinNumber forwardStatusInPin, 
                  PinNumber backStatusInPin, 
                  Preferences* _preferences, 
                  short _delta_threshold = 2,
                  time_t _start_to_end_duration = 3000,
                  short _slider_range = 100,
                  short interval = 1000):
                                        name( _name+"SLD" ), 
                                        delta_threshold( _delta_threshold ),
                                        start_to_end_duration( _start_to_end_duration ),
                                        slider_range( _slider_range ),
                                        step_time( _start_to_end_duration/_slider_range ),
                                        
                                        forward( _name+"FWDRelay", forward_Outpin, interval ),
                                        back( _name+"BCKRelay", back_Outpin, interval ), 
                                        stop( _name+"STPRelay", stop_Outpin, interval ),
                                        
                                        cloud_obj( _cloud_obj ),
                                        
                                        forward_status_pin( forwardStatusInPin ),
                                        back_status_pin( backStatusInPin ),
                                        
                                        current_position(0), 
                                        last_status_check(millis()),
                                        status_check_interval( step_time ),
                                        allWayBack(false),
                                        allWayForward(false),
                                        preferences(_preferences) {
      pinMode(forward_status_pin, INPUT);
      pinMode(back_status_pin, INPUT);
  }
  
  void Trigger() {
    Serial.printf( "SLIDER EVENT received = %hu \n", short(*cloud_obj) );
    Serial.printf("Current status %hu\n", current_position);
    short change = (*cloud_obj) - current_position;
    Serial.printf( "Change = %hu\n", change );

    if( abs(change) <= 1 ) { 
      Serial.printf("SKIPPING THE %s STATE IS UNCHANGED\n", name); 
      return; 
    }

// Add your code here to act upon Slider change
    if( (*cloud_obj) < delta_threshold ) {
      Serial.println("GOING ALL THE WAY BACK");
      allWayBack = true;
      back.Trigger();
      return;
    }
  
    if( 100 - (*cloud_obj) < delta_threshold ) {
      Serial.println("GOING ALL THE WAY FORWARD");
      allWayForward = true;
      forward.Trigger();
      return;
    }

    if( change > delta_threshold ) {
      forward.Trigger();
      stop.TriggerAfter( change*step_time );
      
    } else if (change < -delta_threshold ) {
      back.Trigger();
      stop.TriggerAfter( abs(change)*step_time );
    }

  }
  
  bool pin_status(byte in_pin) {
    return (digitalRead(in_pin) == HIGH);
  }

  void setStartToEnd(time_t value) {
    start_to_end_duration = value;
    preferences->putLong( (name+"Dur").c_str(), start_to_end_duration );
    Serial.printf("Saved %s Start to End duration in flash memory ", name);
    Serial.println(start_to_end_duration);
    step_time = start_to_end_duration/slider_range;
    status_check_interval = step_time;
  }
  
  void status_check() {
    if ( elapsed_time(last_status_check) >= status_check_interval) {
      if( pin_status(forward_status_pin) ) {
        if ( ++current_position > slider_range ) {
            setStartToEnd(start_to_end_duration + status_check_interval);
            current_position = slider_range;
          }
      } 
      else if( !pin_status(forward_status_pin)  && allWayForward) {
        setStartToEnd(current_position*step_time);
        current_position = slider_range;
        allWayForward = false;
      }
      else if ( pin_status(back_status_pin) ) {
        if ( --current_position < 0) current_position = 0;
      } else if ( !pin_status(back_status_pin) && allWayBack ) {
        current_position = 0;
        allWayBack = false;
      }

      if (short(*cloud_obj) != current_position ) {
          *cloud_obj = current_position;
          preferences->putShort( name.c_str(), current_position );
          Serial.printf("Saved %s state in flash memory %hu\n", name, current_position);
      }

      last_status_check = millis();
    }
  }
  
  void checkInTheLoop() {
    forward.checkToRelease();
    back.checkToRelease();
    stop.checkForLaterTrigger();
    stop.checkToRelease();
    
    status_check();
  }
  
  void initStatus() {
    current_position = preferences->getShort(name.c_str());
    Serial.printf("POSITION of %s FROM FLASH MEMORY = %hu\n", name, current_position);   
    start_to_end_duration = preferences->getLong((name+"Dur").c_str(), 3000);
    Serial.printf("Start to End Duration of %s FROM FLASH MEMORY = ", name);
    Serial.println(start_to_end_duration);
    step_time = start_to_end_duration/slider_range;
    status_check_interval = step_time;
  }
  
};


