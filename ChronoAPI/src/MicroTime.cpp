/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
//Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#include <unistd.h>

#include "ChronoAPI/MicroTime.hpp"
#include "ChronoAPI/Timer.hpp"

void chrono::wait(const chrono::Microseconds &duration) {
  chrono::Microseconds period = duration;
  u32 seconds = period.seconds();
  if (seconds > 0) {
    ::sleep(seconds);
    period = period - Seconds(seconds);
  }
  ::usleep(period.microseconds());
}
