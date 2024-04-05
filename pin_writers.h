#pragma once
#include "utils.h"

// ------------ Class DefaultPinWriter  ----------------------//
class DefaultPinWriter
{

public:
  void setOutputMode(PinNumber pin_number)
  {
    pinMode(pin_number, OUTPUT);
  }
  void setHIGH(PinNumber index)
  {
    digitalWrite(index, LOW);
  }
  void setLOW(PinNumber index)
  {
    digitalWrite(index, HIGH);
  }
};

// -------------- Class RegisterPinWriteHandler ---------------------//
template <int N>
class OsrPinWriter
{
private:
  // Constants
  const short numOfRegisterPins;
  const PinNumber serial;               // SER (Serial Input)
  const PinNumber register_clock_latch; // RCLK (Register Clock / Latch)
  const PinNumber shift_register_clock; // SRCLK (Shift Register Clock)
  const PinNumber enableOutput;         // OE (Output Enable)
  bool registers[N];
  time_t last_checked;

public:
  OsrPinWriter(const PinNumber SER,
               const PinNumber RCLK,
               const PinNumber SRCLK,
               const PinNumber OE)
      : numOfRegisterPins(N),
        serial(SER),
        register_clock_latch(RCLK),
        shift_register_clock(SRCLK),
        enableOutput(OE),
        last_checked(millis())
  {
    pinMode(serial, OUTPUT);
    pinMode(register_clock_latch, OUTPUT);
    pinMode(shift_register_clock, OUTPUT);
    pinMode(enableOutput, OUTPUT);
    digitalWrite(enableOutput, LOW);
    clearRegisters();
    writeRegisters();
  }

  void setOutputMode(short pin_number) {}

  void setHIGH(short index)
  {
    registers[index] = HIGH;
    writeRegisters();
    Serial.print("AfterSettingHigh \n");
    for (int i = 0; i < numOfRegisterPins; ++i)
    {
      Serial.print(int(registers[i]));
    }
    Serial.print("\n");
  }

  void setLOW(short index)
  {
    registers[index] = LOW;
    writeRegisters();
    Serial.print("AfterSettingLow \n");
    for (int i = 0; i < numOfRegisterPins; ++i)
    {
      Serial.print(int(registers[i]));
    }
    Serial.print("\n");
  }

  void clearRegisters()
  { //// Clear registers variables
    for (int i = 0; i < numOfRegisterPins; ++i)
    {
      registers[i] = LOW; // LOW;
    }

    Serial.print("AfterClear \n");
    for (int i = 0; i < numOfRegisterPins; ++i)
    {
      Serial.print(int(registers[i]));
    }
  }

  void writeRegisters()
  { //// Write register after being set, will be called in the loop
    if (elapsed_time(last_checked) > 200)
    {
      last_checked = millis();
      digitalWrite(register_clock_latch, LOW);
      for (int i = numOfRegisterPins - 1; i >= 0; i--)
      {
        digitalWrite(shift_register_clock, LOW);
        digitalWrite(serial, int(registers[i]));
        digitalWrite(shift_register_clock, HIGH);
      }
      digitalWrite(register_clock_latch, HIGH);
    }
  }
};
