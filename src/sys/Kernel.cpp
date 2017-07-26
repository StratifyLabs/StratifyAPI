//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#if !defined __link
#include <sos/sos.h>
#endif

#include <sos/link.h>

#include "hal.hpp"
#include "sys/Kernel.hpp"
using namespace sys;

Kernel::Kernel() {
	// TODO Auto-generated constructor stub
	m_current_task = 0;
}

int Kernel::launch(const char * path, char * exec_path, const char * args, int options, int ram_size, int (*update_progress)(int, int), char *const envp[]){
#if defined __link
	return -1;
#else
	return ::launch(path, exec_path, args, options, ram_size, update_progress, envp);
#endif
}

int Kernel::free_ram(const char * path, link_transport_mdriver_t * driver){
	int fd;
	int ret;
#if defined __link
	if( (fd = link_open(driver, path, O_RDONLY)) < 0 ){
		return -1;
	}
	ret = link_ioctl(driver, fd, I_APPFS_FREE_RAM);
	link_close(driver, fd);
#else
	if( (fd = ::open(path, O_RDONLY)) < 0 ){
		return -1;
	}
	driver = 0;
	ret = ::ioctl(fd, I_APPFS_FREE_RAM);
	::close(fd);
#endif
	return ret;
}

int Kernel::reclaim_ram(const char * path, link_transport_mdriver_t * driver){
	int fd;
	int ret;
#if defined __link
	if( (fd = link_open(driver, path, O_RDONLY)) < 0 ){
		return -1;
	}
	ret = link_ioctl(driver, fd, I_APPFS_RECLAIM_RAM);
	link_close(driver, fd);
#else
	if( (fd = ::open(path, O_RDONLY)) < 0 ){
		return -1;
	}
	driver = 0;
	ret = ::ioctl(fd, I_APPFS_RECLAIM_RAM);
	::close(fd);
#endif
	return ret;
}

#if !defined __link
void Kernel::powerdown(int count){
	powerdown(count);
}

int Kernel::hibernate(int count){
	return hibernate(count);
}

int Kernel::request(int req, void * arg){
	return kernel_request(req, arg);
}

void Kernel::reset(){
	Core core(0);
	core.open();
	core.reset();
}

int Kernel::get_board_config(sos_board_config_t & config){
	return ioctl(I_SYS_GETBOARDCONFIG, &config);
}
#endif

int Kernel::get_info(sys_info_t & attr){
	return ioctl(I_SYS_GETINFO, &attr);
}

int Kernel::get_23_info(sys_23_info_t & attr){
	return ioctl(I_SYS_23_GETINFO, &attr);
}

int Kernel::get_26_info(sys_26_info_t & attr){
	return ioctl(I_SYS_26_GETINFO, &attr);
}

int Kernel::get_taskattr(sys_taskattr_t & attr, int task){
	if( task == -1 ){
		task = m_current_task;
	} else {
		m_current_task = task;
	}

	attr.tid = m_current_task;
	m_current_task++;
	return ioctl(I_SYS_GETTASK, &attr);
}

int Kernel::get_id(sys_id_t & id){
	return ioctl(I_SYS_GETID, &id);
}

