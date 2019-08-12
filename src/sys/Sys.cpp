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
	sys.ioctl(
				arg::IoRequest(I_SYS_GETINFO),
				arg::IoArgument(&result.m_info)
				);
	sys.close();
#endif
	return result;
}

#if defined __link
Sys::Sys(link_transport_mdriver_t * driver) : File(driver){}

#else
Sys::Sys() {
}
#endif



int Sys::launch(const var::ConstString & path,
					 const var::ConstString & args,
					 int options,
					 int ram_size){
#if defined __link
	return -1;
#else
	return ::launch(path.cstring(),
						 0,
						 args.cstring(),
						 options,
						 ram_size,
						 0, 0,
						 0);
#endif
}

int Sys::launch(const var::ConstString & path,
								const var::ConstString & args,
								var::String & exec_destination,
								int options, //run in RAM, discard on exit
								int ram_size,
								const sys::ProgressCallback * progress_callback,
								const var::ConstString & envp
								){
#if defined __link
	return -1;
#else
	exec_destination.set_size(PATH_MAX);
	return ::launch(path.cstring(),
						 exec_destination.to_char(),
						 args.cstring(),
						 options,
						 ram_size,
						 sys::ProgressCallback::update_function, (void*)progress_callback,
						 0);
#endif
}

var::String Sys::install(const var::ConstString & path,
						int options, //run in RAM, discard on exit
						int ram_size
		){
	return install(path, options, ram_size, 0);
}


var::String Sys::install(const var::ConstString & path,
								 int options, //run in RAM, discard on exit
								 int ram_size,
								 const sys::ProgressCallback * progress_callback
		){
#if defined __link
	return var::String();
#else
	var::String result;
	result.set_size(PATH_MAX);
			if( ::install(path.cstring(),
							  result.to_char(),
							  options,
							  ram_size,
							  sys::ProgressCallback::update_function,
							  progress_callback) < 0){
		return var::String();
	}
	return result;
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
	if( sys.open(
			 arg::FilePath("/dev/sys"),
			 fs::OpenFlags::read_write()
			 ) < 0 ){
		return -1;
	}

	ret = sys.ioctl(
				arg::IoRequest(I_SYS_GETINFO),
				arg::IoArgument(&info)
				);
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


var::String Sys::get_kernel_version(){
	sys_info_t info;
	Device sys;
	int ret;
	if( sys.open(
			 arg::FilePath("/dev/sys"),
			 fs::OpenFlags::read_write()
			 ) < 0 ){
		return var::String();
	}

	ret = sys.ioctl(
				arg::IoRequest(I_SYS_GETINFO),
				arg::IoArgument(&info)
				);

	sys.close();

	if( ret >= 0 ){
		return var::String(info.kernel_version);
	}
	return var::String();
}

void Sys::powerdown(const chrono::Milliseconds & milliseconds){
	::powerdown(milliseconds.milliseconds());
}

int Sys::hibernate(const chrono::Milliseconds & milliseconds){
	return ::hibernate(milliseconds.milliseconds());
}

int Sys::request(
		const arg::KernelRequest request,
		arg::DestinationBuffer arg
		){
	return kernel_request(request.argument(), arg.argument());
}

void Sys::reset(){
	int fd = ::open("/dev/core", O_RDWR);
	core_attr_t attr;
	attr.o_flags = CORE_FLAG_EXEC_RESET;
	::ioctl(fd, I_CORE_SETATTR, &attr);
	::close(fd); //incase reset fails
}

int Sys::get_board_config(sos_board_config_t & config){
	return ioctl(
				arg::IoRequest(I_SYS_GETBOARDCONFIG),
				arg::IoArgument(&config)
				);
}

SysInfo Sys::get_info(){
	sys_info_t sys_info;
	Sys sys;
	sys.set_keep_open(false);
	if( sys.open() < 0 ){ return SysInfo(); }
	if( sys.get_info(sys_info) < 0 ){ return SysInfo(); }
	return SysInfo(sys_info);
}

SerialNumber Sys::get_serial_number(){
	return get_info().serial_number();
}
#endif




int Sys::get_info(sys_info_t & info){
	return ioctl(
				arg::IoRequest(I_SYS_GETINFO),
				arg::IoArgument(&info)
				);
}

int Sys::get_23_info(sys_23_info_t & attr){
	return ioctl(
				arg::IoRequest(I_SYS_23_GETINFO),
				arg::IoArgument(&attr)
				);
}

int Sys::get_26_info(sys_26_info_t & attr){
	return ioctl(
				arg::IoRequest(I_SYS_26_GETINFO),
				arg::IoArgument(&attr)
				);
}

int Sys::get_id(sys_id_t & id){
	return ioctl(
				arg::IoRequest(I_SYS_GETID),
				arg::IoArgument(&id)
				);
}

