/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
// Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#include "hal/Uart.hpp"
#include "printer/Printer.hpp"

printer::Printer &
printer::operator<<(printer::Printer &printer, const hal::UartAttributes &a) {
  printer.key("port", var::String::number(a.port()));
  printer.key("frequency", var::String::number(a.frequency()));
  printer.key("width", var::String::number(a.width()));
  return printer;
}

using namespace hal;
