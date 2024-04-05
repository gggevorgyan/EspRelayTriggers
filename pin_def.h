#include "pin_writers.h"
#include "status_readers.h"

#define ES32A08_BOARD

#ifdef ES32A08_BOARD
const PinNumber SER_Pin = 13;
const PinNumber RCLK_Pin = 14;
const PinNumber SRCLK_Pin = 27;
const PinNumber OE_Pin = 4;

typedef OsrPinWriter<24> PinWriter;
typedef IsrStatusReader<8> StatusReader;

PinWriter defaultPinWriter(SER_Pin, RCLK_Pin, SRCLK_Pin, OE_Pin);
StatusReader defaultStatusReader;
////////// --- Irrigation Control  Pins --- /////////
#define IRRIGATION_START_OUT_PIN 16
#define IRRIGATION_STOP_OUT_PIN 17
////////// --- Ventilation Control  Pins --- /////////
#define VENTILATION_START_OUT_PIN 18
#define VENTILATION_STOP_OUT_PIN 19
////////// --- WINDOW Control  Pins --- /////////
#define WINDOW_UP_OUT_PIN 20
#define WINDOW_DOWN_OUT_PIN 21
#define WINDOW_STOP_OUT_PIN 22

// ------------------- STATUS INPUT INDEXES ---------------------//
#define IRRIGATION_STATUS_IN_PIN 0
#define VENTILATION_STATUS_IN_PIN 1
#define WINDOW_UP_STATUS_IN_PIN 2
#define WINDOW_DOWN_STATUS_IN_PIN 3

#else
typedef DefaultPinWriter PinWriter;
typedef DefaultStatusReader StatusReader;
PinWriter defaultPinWriter;
StatusReader defaultStatusReader;
////////// --- WINDOW Control  Pins --- /////////
#define IRRIGATION_START_OUT_PIN 22
#define IRRIGATION_STOP_OUT_PIN 23
////////// --- WINDOW Control  Pins --- /////////
#define VENTILATION_START_OUT_PIN 18
#define VENTILATION_STOP_OUT_PIN 19
////////// --- WINDOW Control  Pins --- /////////
#define WINDOW_UP_OUT_PIN 22
#define WINDOW_DOWN_OUT_PIN 23
#define WINDOW_STOP_OUT_PIN 19

// ------------------- STATUS INPUT PINS ---------------------//
#define IRRIGATION_STATUS_IN_PIN 36
#define VENTILATION_STATUS_IN_PIN 39
#define WINDOW_UP_STATUS_IN_PIN 34
#define WINDOW_DOWN_STATUS_IN_PIN 35

#endif

//////////////// --- Sensors --- ////////////////
#define TEMP_SENSOR_PIN 5

void init_pins()
{
#ifdef ES32A08_BOARD
  defaultPinWriter.clearRegisters();
  defaultPinWriter.writeRegisters();
#endif
  pinMode(TEMP_SENSOR_PIN, INPUT);
}
