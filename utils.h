#pragma once

typedef byte PinNumber;

unsigned long elapsed_time(unsigned long from_timestamp) {
  long change = millis() - from_timestamp;
  if (change >= 0) {
    return change;
  } else {
    return ULONG_MAX + change;
  }
}
