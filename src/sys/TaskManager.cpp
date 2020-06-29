/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "sys/TaskManager.hpp"
#include "sys/Printer.hpp"

using namespace sys;

Printer& sys::operator << (Printer& printer, const sys::TaskInfo & a){
	printer.key("name", a.name());
	printer.key("id", F32U, a.id());
	printer.key("pid", F32U, a.pid());
	printer.key("memorySize", F32U, a.memory_size());
	printer.key("stack", "0x%lX", a.stack());
	printer.key("stackSize", F32U, a.stack_size());
	printer.key("priority", F32U, a.priority());
	printer.key("priorityCeiling", F32U, a.priority_ceiling());
	printer.key("isThread", a.is_thread());
	if( a.is_thread() == false ){
		printer.key("heap", "0x%lX", a.heap());
		printer.key("heapSize", "%ld", a.heap_size());
	}
	return printer;
}

TaskManager::TaskManager(
		SAPI_LINK_DRIVER
		){
#if defined __link
	m_sys_device.set_driver(link_driver.argument());
#endif
	m_id = 0;
}

TaskManager::~TaskManager(){
	finalize();
}

void TaskManager::initialize(){
	if( (m_sys_device.fileno() < 0)
			&& (m_sys_device.open("/dev/sys")) ){
		m_id = -1;
	}
}

void TaskManager::finalize(){
	m_sys_device.close();
}


int TaskManager::count_total(){
	int idx = m_id;
	int count = 0;
	set_id( 0 );
	TaskInfo attr;
	while( get_next(attr) >= 0 ){
		count++;
	}
	set_id(idx);
	return count;
}


int TaskManager::count_free(){
	int idx = m_id;
	int count = 0;
	set_id(0);
	TaskInfo attr;
	while( get_next(attr) >= 0) {
		if( !attr.is_enabled() ){
			count++;
		}
	}
	set_id(idx);
	return count;
}


int TaskManager::get_next(TaskInfo & info){
	sys_taskattr_t task_attr;
	int ret;

	task_attr.tid = m_id;
	m_id++;

	initialize();

	ret = set_error_number_if_error(
				m_sys_device.ioctl(
					fs::File::IoRequest(I_SYS_GETTASK),
					fs::File::IoArgument(&task_attr)
					)
				);

	if( ret < 0 ){
		info = TaskInfo::invalid();
		return ret;
	}

	info = task_attr;
	return ret;
}

#if !defined __link
TaskInfo TaskManager::get_info(){
	TaskManager manager;
	return manager.get_info(Thread::self());
}
#endif

TaskInfo TaskManager::get_info(u32 id){
	sys_taskattr_t attr;
	attr.tid = id;
	initialize();
	if( set_error_number_if_error(
			 m_sys_device.ioctl(
				 fs::File::IoRequest(I_SYS_GETTASK),
				 fs::File::IoArgument(&attr)
				 ) ) < 0 ){
		return TaskInfo::invalid();
	}

	return TaskInfo(attr);
}

bool TaskManager::is_pid_running(pid_t pid){
	int tmp_id = id();
	set_id( 1 );

	TaskInfo info;
	while( get_next(info) > 0 ){
		if( (pid == info.pid()) && info.is_enabled() ){
			set_id( tmp_id );
			return true;
		}
	}

	set_id(tmp_id);
	return false;
}

int TaskManager::get_pid(const var::String & name){
	int tmp_id = id();
	set_id(1);

	TaskInfo info;

	while( get_next(info) > 0 ){
		if( name == info.name() && info.is_enabled() ){
			set_id(tmp_id);
			return info.pid();
		}
	}

	set_id(tmp_id);
	return -1;
}
