/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#include "sys/TaskManager.hpp"
#include "printer/Printer.hpp"
#include "thread/Thread.hpp"

printer::Printer &
printer::operator<<(printer::Printer &printer, const sys::TaskInfo &a) {
  printer.key("name", a.name());
  printer.key("id", F32U, a.id());
  printer.key("pid", F32U, a.pid());
  printer.key("memorySize", F32U, a.memory_size());
  printer.key("stack", "0x%lX", a.stack());
  printer.key("stackSize", F32U, a.stack_size());
  printer.key("priority", F32U, a.priority());
  printer.key("priorityCeiling", F32U, a.priority_ceiling());
  printer.key("isThread", a.is_thread());
  if (a.is_thread() == false) {
    printer.key("heap", "0x%lX", a.heap());
    printer.key("heapSize", "%ld", a.heap_size());
  }
  return printer;
}

using namespace sys;

TaskManager::TaskManager(FSAPI_LINK_DECLARE_DRIVER)
  : m_sys_device(
    "/dev/sys",
    fs::OpenMode::read_write() FSAPI_LINK_INHERIT_DRIVER_LAST) {
  m_id = 0;
}

int TaskManager::count_total() {
  int idx = m_id;
  int count = 0;
  set_id(0);
  TaskInfo attr;
  while (get_next(attr) >= 0) {
    count++;
  }
  set_id(idx);
  return count;
}

int TaskManager::count_free() {
  int idx = m_id;
  int count = 0;
  set_id(0);
  TaskInfo attr;
  while (get_next(attr) >= 0) {
    if (!attr.is_enabled()) {
      count++;
    }
  }
  set_id(idx);
  return count;
}

int TaskManager::get_next(TaskInfo &info) {
  sys_taskattr_t task_attr;
  int ret;
  task_attr.tid = m_id;
  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::io_error,
    m_sys_device.ioctl(I_SYS_GETTASK, &task_attr).status().value());

  if (status().is_error()) {
    info = TaskInfo::invalid();
  } else {
    info = task_attr;
  }

  m_id++;
  return ret;
}

#if !defined __link
TaskInfo TaskManager::get_info() {
  TaskManager manager;
  return manager.get_info(thread::Thread::self());
}
#endif

TaskInfo TaskManager::get_info(u32 id) {
  sys_taskattr_t attr;
  attr.tid = id;
  initialize();
  if (
    API_ASSIGN_ERROR_CODE(
      api::ErrorCode::io_error,
      m_sys_device.ioctl(I_SYS_GETTASK, &attr).status().value())
    < 0) {
    return TaskInfo::invalid();
  }

  return TaskInfo(attr);
}

bool TaskManager::is_pid_running(pid_t pid) {
  int tmp_id = id();
  set_id(1);

  TaskInfo info;
  while (get_next(info) > 0) {
    if ((static_cast<u32>(pid) == info.pid()) && info.is_enabled()) {
      set_id(tmp_id);
      return true;
    }
  }

  set_id(tmp_id);
  return false;
}

int TaskManager::get_pid(const var::String &name) {
  int tmp_id = id();
  set_id(1);

  TaskInfo info;

  while (get_next(info) > 0) {
    if (name == info.name() && info.is_enabled()) {
      set_id(tmp_id);
      return info.pid();
    }
  }

  set_id(tmp_id);
  return -1;
}
