#include "arduino_secrets.h"
/*
  Sketch generated by the Arduino IoT Cloud Thing "Untitled"
  https://create.arduino.cc/cloud/things/68273403-031d-4179-be15-31a533afd898

  Arduino IoT Cloud Variables description

  The following variables are aut omatically generated and updated when changes are made to the Thing

  CloudSwitch irrigation_switch;
  CloudSwitch ventilation_switch;
  CloudPercentage windowSlider;
  CloudRelativeHumidity humidity;
  CloudTemperature temperature;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/

#include "thingProperties.h"
#include "pin_def.h"
#include "switch.h"
#include "slider.h"
#include <Preferences.h>

Preferences preferences;

///////////////////////// --- VENTILATION --- ////////////////////////////
SwitchRelays<PinWriter, StatusReader> ventilationSwitch("vent", VENTILATION_START_OUT_PIN,
                                                        VENTILATION_STOP_OUT_PIN,
                                                        &ventilation_switch,
                                                        VENTILATION_STATUS_IN_PIN,
                                                        &preferences,
                                                        &defaultPinWriter,
                                                        &defaultStatusReader);
///////////////////////// --- IRRIGATION --- /////////////////////////////
SwitchRelays<PinWriter, StatusReader> irrigationSwitch("irrg", IRRIGATION_START_OUT_PIN,
                                                       IRRIGATION_STOP_OUT_PIN,
                                                       &irrigation_switch,
                                                       IRRIGATION_STATUS_IN_PIN,
                                                       &preferences,
                                                       &defaultPinWriter,
                                                       &defaultStatusReader);

/////////////////////////// --- WINDOW --- ////////////////////////////////
Slider3Relays<PinWriter, StatusReader> window("window",
                                              WINDOW_UP_OUT_PIN,
                                              WINDOW_DOWN_OUT_PIN,
                                              WINDOW_STOP_OUT_PIN,
                                              &windowSlider,
                                              WINDOW_UP_STATUS_IN_PIN,
                                              WINDOW_DOWN_STATUS_IN_PIN,
                                              &preferences,
                                              &defaultPinWriter,
                                              &defaultStatusReader);

void setup()
{
  // Initialize serial and wait for port to open:
  Serial.begin(19200);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500);

  preferences.begin("RelayApp", false);

  init_pins();

  // Defined in thingProperties.h
  initProperties();

  window.initStatus();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(2);
  // ArduinoCloud.printDebugInfo();
}

void loop()
{
  ArduinoCloud.update();
  // Your code here
  ventilationSwitch.checkInTheLoop();
  irrigationSwitch.checkInTheLoop();
  window.checkInTheLoop();
}

/*
  Since Slider is READ_WRITE variable, onSliderChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onWindowSliderChange()
{
  window.Trigger();
}

/*
  Since VentilationSwitch is READ_WRITE variable, onVentilationSwitchChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onVentilationSwitchChange()
{
  ventilationSwitch.Trigger();
}

/*
  Since IrrigationSwitch is READ_WRITE variable, onIrrigationSwitchChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onIrrigationSwitchChange()
{
  irrigationSwitch.Trigger();
}

/*
  Since Temperature is READ_WRITE variable, onTemperatureChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onTemperatureChange()
{
  // Add your code here to act upon Temperature change
}