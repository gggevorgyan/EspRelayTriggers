#include "relay_trigger.h"

//---------------------------------------------------------------------------------------
//--------------------------------Switch class ------------------------------------------
template <class PinWriter, class StatusReader>
class Slider3Relays
{
  String name;
  short delta_threshold;
  time_t start_to_end_duration;
  short slider_range;
  short step_time;

  RelayTrigger<PinWriter> forward;
  RelayTrigger<PinWriter> back;
  RelayLazyTrigger<PinWriter> stop;

  CloudPercentage *cloud_obj;

  PinNumber forward_status_pin;
  PinNumber back_status_pin;

  short current_position;
  time_t last_status_check;
  time_t status_check_interval;

  Preferences *preferences;
  StatusReader *statusReader;

public:
  Slider3Relays(String _name,
                PinNumber forward_Outpin,
                PinNumber back_Outpin,
                PinNumber stop_Outpin,
                CloudPercentage *_cloud_obj,
                PinNumber forwardStatusInPin,
                PinNumber backStatusInPin,
                Preferences *_preferences,
                PinWriter *_pinWriter,
                StatusReader *_statusReader,
                short _delta_threshold = 2,
                time_t _start_to_end_duration = 60000,
                short _slider_range = 100,
                short interval = 1000)
      : name(_name + "SLD"),
        delta_threshold(_delta_threshold),
        start_to_end_duration(_start_to_end_duration),
        slider_range(_slider_range),
        step_time(_start_to_end_duration / _slider_range),

        forward(_name + "FWRelay", forward_Outpin, _pinWriter, interval),
        back(_name + "BCKRelay", back_Outpin, _pinWriter, interval),
        stop(_name + "STPRelay", stop_Outpin, _pinWriter, interval),

        cloud_obj(_cloud_obj),

        forward_status_pin(forwardStatusInPin),
        back_status_pin(backStatusInPin),

        current_position(0),
        last_status_check(millis()),
        status_check_interval(step_time),
        preferences(_preferences),
        statusReader(_statusReader)
  {
    statusReader->setInputPin(forwardStatusInPin);
    statusReader->setInputPin(backStatusInPin);
  }

  void Trigger()
  {
    Serial.printf("SLIDER EVENT received = %hu \n", short(*cloud_obj));
    Serial.printf("Current status %hu\n", current_position);
    short change = *cloud_obj - current_position;
    Serial.printf("Change = %hu\n", change);

    if (abs(change) <= 1)
    {
      Serial.printf("SKIPPING THE %s STATE IS UNCHANGED\n", name);
      return;
    }

    // Add your code here to act upon Slider change
    if (*cloud_obj < delta_threshold)
    {
      Serial.println("GOING ALL THE WAY BACK");
      back.Trigger();
      return;
    }

    if (100 - *cloud_obj < delta_threshold)
    {
      Serial.println("GOING ALL THE WAY FORWARD");
      forward.Trigger();
      return;
    }

    if (change > delta_threshold)
    {
      forward.Trigger();
      stop.TriggerAfter(change * step_time);
    }
    else if (change < -delta_threshold)
    {
      back.Trigger();
      stop.TriggerAfter(abs(change) * step_time);
    }
  }

  void status_check()
  {
    if (elapsed_time(last_status_check) >= status_check_interval)
    {
      if (statusReader->getStatus(forward_status_pin))
      {
        if (++current_position > 100)
          current_position = 100;
      }
      else if (statusReader->getStatus(back_status_pin))
      {
        if (--current_position < 0)
          current_position = 0;
      }
      if (short(*cloud_obj) != current_position)
      {
        *cloud_obj = current_position;
        preferences->putShort(name.c_str(), current_position);
        Serial.printf("Saved %s state into flash memory %hu\n", name, current_position);
      }

      last_status_check = millis();
    }
  }

  void checkInTheLoop()
  {
    forward.checkToRelease();
    back.checkToRelease();
    stop.checkForLaterTrigger();
    stop.checkToRelease();

    status_check();
  }

  void initStatus()
  {
    current_position = preferences->getShort(name.c_str());
    Serial.printf("POSITION of %s FROM FLASH MEMORY = %hu\n", name, current_position);
  }
};
