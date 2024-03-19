
////////// --- WINDOW Control  Pins --- /////////
#define IRRIGATION_START_OUT_PIN 22
#define IRRIGATION_STOP_OUT_PIN 23
#define IRRIGATION_STATUS_IN_PIN 26

////////// --- WINDOW Control  Pins --- /////////
#define VENTILATION_START_OUT_PIN 18
#define VENTILATION_STOP_OUT_PIN 19
#define VENTILATION_STATUS_IN_PIN 27

////////// --- WINDOW Control  Pins --- /////////
#define WINDOW_UP_OUT_PIN 22
#define WINDOW_DOWN_OUT_PIN 23

#define WINDOW_UP_STATUS_IN_PIN 32
#define WINDOW_DOWN_STATUS_IN_PIN 33

#define WINDOW_STOP_OUT_PIN 19

//////////////// --- Sensors --- ////////////////
#define TEMP_SENSOR_PIN 5


void init_pins() {
 pinMode(TEMP_SENSOR_PIN, INPUT);
}

