/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
//Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#include <unistd.h>

#include "chrono/MicroTime.hpp"

void chrono::wait(const chrono::MicroTime &duration) {
  chrono::MicroTime period = duration;
  u32 seconds = period.seconds();
  if (seconds > 0) {
    ::sleep(seconds);
    period = period - Seconds(seconds);
  }
  ::usleep(period.microseconds());
}
