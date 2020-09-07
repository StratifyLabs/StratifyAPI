/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
// Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#include "hal/Uart.hpp"
#include "sys/Printer.hpp"

sys::Printer &
sys::operator<<(sys::Printer &printer, const hal::UartAttributes &a) {
  printer.key("port", a.port());
  printer.key("frequency", var::String::number(a.frequency()));
  printer.key("width", var::String::number(a.width()));
  return printer;
}

using namespace hal;

Uart::Uart(port_t port) : Periph(CORE_PERIPH_UART, port) {}

int Uart::get(char &c) { return ioctl(IoRequest(I_UART_GET), IoArgument(&c)); }

int Uart::put(char c) { return ioctl(IoRequest(I_UART_PUT), IoIntArgument(c)); }

int Uart::flush() { return ioctl(IoRequest(I_UART_FLUSH)); }

UartInfo Uart::get_info() const {
  uart_info_t info;
  if (get_info(info) < 0) {
    return UartInfo();
  }
  return UartInfo(info);
}
