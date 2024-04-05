#pragma once

#include "utils.h"

class DefaultStatusReader
{
public:
  void setInputPin(PinNumber pin)
  {
    pinMode(pin, INPUT);
  }
  bool getStatus(PinNumber in_pin)
  {
    return (digitalRead(in_pin) == HIGH);
  }
};

template <int N>
class IsrStatusReader
{
private:
  const uint8_t dataPin;  // connected to 74HC165 QH (5) pin
  const uint8_t latchPin; // connected to 74HC165 SH/LD (17) pin
  const uint8_t clockPin; // connected to 74HC165 CLK (16) pin
  const short numOfInputRegisterPins;
  bool inputRegisters[N];
  time_t last_checked;

public:
  IsrStatusReader(PinNumber _dataPin = 5,
                  PinNumber _latchPin = 16,
                  PinNumber _clockPin = 17)
      : dataPin(_dataPin),
        latchPin(_latchPin),
        clockPin(_clockPin),
        numOfInputRegisterPins(N),
        last_checked(millis())
  {
    pinMode(dataPin, INPUT);
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    for (short i = 0; i < numOfInputRegisterPins; i++)
    {
      inputRegisters[i] = 0;
    }
  }

  void setInputPin(PinNumber pin) {}

  bool getStatus(short index)
  {
    readInputsWithDigitalRead();
    return inputRegisters[index];
  }

  void readInputsWithDigitalRead()
  {
    if (elapsed_time(last_checked) > 200)
    {
      byte inputs = 0;
      digitalWrite(latchPin, HIGH);                  // disable input latching and enable shifting
      digitalWrite(clockPin, LOW);                   // preset clock to retrieve first bit
      inputs = shiftIn(dataPin, clockPin, LSBFIRST); // capture input values
      digitalWrite(clockPin, HIGH);                  // preset clock to retrieve first bit
      digitalWrite(latchPin, LOW);

      // Read and print individual inputs
      for (short i = 0; i < numOfInputRegisterPins; i++)
      {
        inputRegisters[numOfInputRegisterPins - 1 - i] = ((inputs >> i) & 1) <= 0;
      }
    }
  }
};
