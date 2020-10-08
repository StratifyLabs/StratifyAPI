/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
// Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#include "hal/Device.hpp"
#include "chrono/Timer.hpp"

using namespace sys;
using namespace hal;

Device::Device(
  var::StringView path,
  fs::OpenMode open_mode FSAPI_LINK_DECLARE_DRIVER_LAST)
  : FileAccess(path, open_mode FSAPI_LINK_INHERIT_DRIVER_LAST) {
  set_keep_open(true);
}

#ifndef __link
Device &Device::read(fs::Aio &aio) {
  aio.m_aio_var.aio_fildes = fileno();
  API_ASSIGN_ERROR_CODE(api::ErrorCode::io_error, ::aio_read(&(aio.m_aio_var)));
  return *this;
}

Device &Device::write(fs::Aio &aio) {
  aio.m_aio_var.aio_fildes = fileno();
  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::io_error,
    ::aio_write(&(aio.m_aio_var)));
  return *this;
}

Device &Device::cancel_read(int channel) {
  return cancel(channel, MCU_EVENT_FLAG_DATA_READY);
}

Device &Device::cancel_write(int channel) {
  return cancel(channel, MCU_EVENT_FLAG_WRITE_COMPLETE);
}

Device &Device::cancel(int channel, int o_events) {
  mcu_action_t action;
  memset(&action, 0, sizeof(action));
  action.channel = channel;
  action.o_events = o_events;
  return ioctl(I_MCU_SETACTION, &action);
}

Device &Device::transfer(const Transfer &options) {
  fs::Aio aio(options.destination());
  read(aio).write(options.source());
  while (aio.is_busy() && status().is_success()) {
    // aio must live until the read completes -- or big problems will happen
    chrono::wait(chrono::Microseconds(200));
  }
  return *this;
}

#endif

Device &Device::set_interrupt_priority(int priority, int request) {
  mcu_action_t action;
  memset(&action, 0, sizeof(action));
  action.o_events = MCU_EVENT_FLAG_SET_PRIORITY;
  action.prio = priority;
  return ioctl(request, &action);
}
