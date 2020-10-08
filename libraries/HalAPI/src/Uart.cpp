/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
// Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#include "hal/Uart.hpp"
#include "printer/Printer.hpp"

printer::Printer &
printer::operator<<(printer::Printer &printer, const hal::Uart::Attributes &a) {
  printer.key("frequency", var::String::number(a.frequency()));
  printer.key("width", var::String::number(a.width()));
  return printer;
}

printer::Printer &
printer::operator<<(printer::Printer &printer, const hal::Uart::Info &a) {
  printer.key("valid", a.is_valid());
  printer.key("sizeReady", var::String::number(a.size_ready()));
  printer.key("size", var::String::number(a.size()));
  return printer;
}

