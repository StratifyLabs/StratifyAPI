#include "sys/TaskManager.hpp"


using namespace sys;

#if defined __link
TaskManager::TaskManager(link_transport_mdriver_t * driver){
	m_sys_device.set_driver(driver);
#else
TaskManager::TaskManager(){
#endif
	m_id = 0;
}

TaskManager::~TaskManager(){
	finalize();
}

void TaskManager::initialize(){
	if( m_sys_device.fileno() < 0 ){
		m_sys_device.open(arg::FilePath("/dev/sys"));
	}
}

void TaskManager::finalize(){
	m_sys_device.close();
}


int TaskManager::count_total(){
	int idx = m_id;
	int count = 0;
	set_id(0);
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
					arg::IoRequest(I_SYS_GETTASK),
					arg::IoArgument(&task_attr)
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
	return manager.get_info( Thread::self() );
}
#endif

TaskInfo TaskManager::get_info(int id){
	sys_taskattr_t attr;
	attr.tid = id;
	initialize();
	if( set_error_number_if_error(
			 m_sys_device.ioctl(
				 arg::IoRequest(I_SYS_GETTASK),
				 arg::IoArgument(&attr)
				 ) ) < 0 ){
		return TaskInfo::invalid();
	}

	return TaskInfo(attr);
}

bool TaskManager::is_pid_running(int pid){
	int tmp_id = id();
	set_id(1);

	TaskInfo info;
	while( get_next(info) > 0 ){
		if( (pid == (int)info.pid()) && info.is_enabled() ){
			set_id(tmp_id);
			return true;
		}
	}

	set_id(tmp_id);
	return false;
}

int TaskManager::get_pid(const var::ConstString & name){
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

void TaskManager::print(int pid){
	int count = count_total();
	TaskInfo info;
	TaskInfo::print_header();
	for(int i = 0; i < count; i++){
		info = get_info(i);
		if( (pid < 0 || (pid == (int)info.pid())) && info.is_enabled() ){
			info.print();
		}
	}
}

void TaskInfo::print_header(){
	printf("name(pid,id): prio:value/ceiling mem:total (heap,stack)\n");
}


void TaskInfo::print() const {
	if( is_valid() ){
		if( is_thread() ){
			printf("%s(" F32D "," F32D "): prio:%d/%d memory:" F32D " (NA," F32D ")\n",
					 name().cstring(),
					 pid(),
					 id(),
					 priority(), priority_ceiling(),
					 memory_size(), stack_size());
		} else {
			printf("%s(" F32D "," F32D "): prio:%d/%d memory:" F32D " (" F32D "," F32D ")\n",
					 name().cstring(),
					 pid(),
					 id(),
					 priority(), priority_ceiling(),
					 memory_size(),
					 heap_size(), stack_size());
		}
	}
}
