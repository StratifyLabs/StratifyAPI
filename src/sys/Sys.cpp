//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#if !defined __link
#include <sos/sos.h>
#endif

#include <sos/link.h>

#include "hal.hpp"
#include "sys/Sys.hpp"
using namespace sys;

SerialNumber::SerialNumber(){
	memset(&m_serial_number, 0, sizeof(mcu_sn_t));
}

SerialNumber SerialNumber::from_string(const var::ConstString & str){
	SerialNumber ret;
	u32 len = strnlen(str.cstring(), 8*4);
	if( len == 8*4 ){
#if defined __link
		sscanf(str.cstring(), "%08X%08X%08X%08X",
#else
		sscanf(str.cstring(), "%08lX%08lX%08lX%08lX",
#endif
				 &ret.m_serial_number.sn[3],
				&ret.m_serial_number.sn[2],
				&ret.m_serial_number.sn[1],
				&ret.m_serial_number.sn[0]);
	}
	return ret;
}

SerialNumber::SerialNumber(const var::ConstString & str){
	SerialNumber serial_number = from_string(str);
	memcpy(&m_serial_number, &serial_number.m_serial_number, sizeof(mcu_sn_t));
}

bool SerialNumber::operator == (const SerialNumber & serial_number){
	return memcmp(&serial_number.m_serial_number, &m_serial_number, sizeof(mcu_sn_t)) == 0;
}

SerialNumber SerialNumber::get(){
	SerialNumber result;
	Core core(0);
	core_info_t info;
	if( core.open() >= 0 ){
		if( core.get_info(info) >= 0 ){
			memcpy(&result.m_serial_number, &info.serial, sizeof(mcu_sn_t));
		}
		core.close();
	}
	return result;
}

var::String SerialNumber::to_string() const {
	var::String ret;
	ret.format(F3208X F3208X F3208X F3208X,
				  m_serial_number.sn[3],
			m_serial_number.sn[2],
			m_serial_number.sn[1],
			m_serial_number.sn[0]);
	return ret;
}


SysInfo SysInfo::get(){
	SysInfo result;
#if !defined __link
	Sys sys;
	if( sys.open() < 0 ){ return result; }
	sys.ioctl(I_SYS_GETINFO, &result.m_info);
	sys.close();
#endif
	return result;
}

#if defined __link
Sys::Sys(link_transport_mdriver_t * driver) : File(driver){
	m_current_task = 0;
}

#else
Sys::Sys() {
	// TODO Auto-generated constructor stub
	m_current_task = 0;
}
#endif



int Sys::launch(const char * path, char * exec_path, const char * args, int options, int ram_size, int (*update_progress)(int, int), char *const envp[]){
#if defined __link
	return -1;
#else
	return ::launch(path, exec_path, args, options, ram_size, update_progress, envp);
#endif
}

int Sys::free_ram(const char * path, link_transport_mdriver_t * driver){
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

void Sys::assign_zero_sum32(void * data, int size){
	u32 sum = 0;
	u32 * ptr = (u32*)data;
	int i;
	int count;
	if( size & 0x03 ){ return; }
	count = size / sizeof(u32);
	for(i=0; i < count-1; i++){
		sum += ptr[i];
	}
	ptr[i] = (u32)(0 - sum);
}

int Sys::verify_zero_sum32(void * data, int size){
	u32 sum = 0;
	u32 * ptr = (u32*)data;
	int i;
	int count;
	if( size & 0x03 ){ return 1; }
	count = size / sizeof(u32);
	for(i=0; i < count; i++){
		sum += ptr[i];
	}
	return sum == 0;
}

int Sys::reclaim_ram(const char * path, link_transport_mdriver_t * driver){
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
int Sys::get_version(var::String & version){
	sys_info_t info;
	Device sys;
	int ret;
	if( sys.open("/dev/sys", Device::RDWR) < 0 ){
		return -1;
	}

	ret = sys.ioctl(I_SYS_GETINFO, &info);
	sys.close();
	if( ret >= 0 ){
		version.assign(info.sys_version);
	}
	return ret;

}

var::String Sys::get_version(){
	var::String version;
	get_version(version);
	return version;
}


int Sys::get_kernel_version(var::String & version){
	sys_info_t info;
	Device sys;
	int ret;
	if( sys.open("/dev/sys", Device::RDWR) < 0 ){
		return -1;
	}

	ret = sys.ioctl(I_SYS_GETINFO, &info);
	sys.close();
	if( ret >= 0 ){
		version.assign(info.kernel_version);
	}
	return ret;
}

void Sys::powerdown(int count){
	::powerdown(count);
}

int Sys::hibernate(int count){
	return ::hibernate(count);
}

int Sys::request(int request, void * arg){
	return kernel_request(request, arg);
}

void Sys::reset(){
	Core core(0);
	core.open();
	core.reset();
	core.close(); //in case the operation fails
}

int Sys::get_board_config(sos_board_config_t & config){
	return ioctl(I_SYS_GETBOARDCONFIG, &config);
}
#endif

int Sys::get_info(sys_info_t & attr){
	return ioctl(I_SYS_GETINFO, &attr);
}

int Sys::get_23_info(sys_23_info_t & attr){
	return ioctl(I_SYS_23_GETINFO, &attr);
}

int Sys::get_26_info(sys_26_info_t & attr){
	return ioctl(I_SYS_26_GETINFO, &attr);
}

int Sys::get_taskattr(sys_taskattr_t & attr, int task){
	if( task == -1 ){
		task = m_current_task;
	} else {
		m_current_task = task;
	}

	attr.tid = m_current_task;
	m_current_task++;
	return ioctl(I_SYS_GETTASK, &attr);
}

int Sys::get_id(sys_id_t & id){
	return ioctl(I_SYS_GETID, &id);
}

