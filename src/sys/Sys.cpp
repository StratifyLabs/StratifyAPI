/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
//Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#if !defined __link
#include <sos/sos.h>
#endif

#include <sos/link.h>

#include "hal.hpp"
#include "sys/Sys.hpp"
#include "sys/Trace.hpp"
#include "sys/Printer.hpp"

using namespace sys;

Printer& sys::operator << (Printer& printer, const TraceEvent & a){
	var::String id;
	chrono::ClockTime clock_time;
	clock_time = a.timestamp();
	switch(a.id()){
		case LINK_POSIX_TRACE_FATAL: id = "fatal"; break;
		case LINK_POSIX_TRACE_CRITICAL: id = "critical"; break;
		case LINK_POSIX_TRACE_WARNING: id = "warning"; break;
		case LINK_POSIX_TRACE_MESSAGE: id = "message"; break;
		case LINK_POSIX_TRACE_ERROR: id = "error"; break;
		default: id = "other"; break;
	}
	printer.key("timestamp", F32U ".%06ld", clock_time.seconds(), clock_time.nanoseconds()/1000UL);
	printer.key("id", id);
	printer.key("thread", "%d", a.thread_id());
	printer.key("pid", "%d", a.pid());
	printer.key("programAddress", "0x%lX", a.program_address());
	printer.key("message", a.message());
	return printer;
}

Printer & sys::operator << (Printer& printer, const sys::SysInfo & a){
	printer.key("name", a.name());
	printer.key("serialNumber", a.serial_number().to_string());
	printer.key("hardwareId",  F3208X, a.hardware_id());
	if( a.name() != "bootloader" ){
		printer.key("projectId", a.id());
		if( a.team_id().is_empty() == false ){
			printer.key("teamId", a.team_id());
		}
		printer.key("bspVersion",	a.bsp_version());
		printer.key("sosVersion",	a.sos_version());
		printer.key("cpuArchitecture", a.cpu_architecture());
		printer.key("cpuFrequency", F32D, a.cpu_frequency());
		printer.key("applicationSignature", F32X, a.application_signature());

		printer.key("bspGitHash",	a.bsp_git_hash());
		printer.key("sosGitHash",	a.sos_git_hash());
		printer.key("mcuGitHash",	a.mcu_git_hash());
	}
	return printer;
}


SerialNumber::SerialNumber(){
	memset(&m_serial_number, 0, sizeof(mcu_sn_t));
}

SerialNumber SerialNumber::from_string(const var::String & str){
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

SerialNumber::SerialNumber(const var::String & str){
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
				fs::File::IoRequest(I_SYS_GETINFO),
				fs::File::IoArgument(&result.m_info)
				);
	sys.close();
#endif
	return result;
}

Sys::Sys(
		SAPI_LINK_DRIVER
		) : File(
					#if defined __link
					link_driver
					#endif
					){

}





int Sys::launch(const var::String & path,
								Arguments args,
								enum Appfs::flags options,
								int ram_size){
#if defined __link
	return -1;
#else
	return ::launch(path.cstring(),
									0,
									args.argument().cstring(),
									options,
									ram_size,
									0, 0,
									0);
#endif
}

int Sys::launch(
		const var::String & path,
		Arguments args,
		DestinationPath exec_destination,
		enum Appfs::flags options, //run in RAM, discard on exit
		int ram_size,
		const sys::ProgressCallback * progress_callback,
		Environment envp
		){
#if defined __link
	return -1;
#else
	exec_destination.argument().resize(PATH_MAX);
	return ::launch(
				path.cstring(),
				exec_destination.argument().to_char(),
				args.argument().cstring(),
				options,
				ram_size,
				sys::ProgressCallback::update_function,
				(void*)progress_callback,
				nullptr);
#endif
}

var::String Sys::install(
		const var::String & path,
		enum Appfs::flags options, //run in RAM, discard on exit
		int ram_size
		){
	return install(path, options, ram_size, 0);
}


var::String Sys::install(
		const var::String & path,
		enum Appfs::flags options, //run in RAM, discard on exit
		int ram_size,
		const sys::ProgressCallback * progress_callback
		){
#if defined __link
	return var::String();
#else
	var::String result;
	result.resize(PATH_MAX);
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

int Sys::free_ram(
		const var::String & path
		SAPI_LINK_DRIVER_LAST
		){
	int fd;
	int ret;
#if defined __link
	if( (fd = link_open(
				 link_driver.argument(),
				 path.cstring(),
				 O_RDONLY
				 )
			 ) < 0 ){
		return -1;
	}
	ret = link_ioctl(link_driver.argument(), fd, I_APPFS_FREE_RAM);
	link_close(link_driver.argument(), fd);
#else
	if( (fd = ::open(path.cstring(), O_RDONLY)) < 0 ){
		return -1;
	}
	ret = ::ioctl(fd, I_APPFS_FREE_RAM);
	::close(fd);
#endif
	return ret;
}

int Sys::reclaim_ram(const var::String & path
										 SAPI_LINK_DRIVER_LAST
										 ){
	int fd;
	int ret;
#if defined __link
	if( (fd = link_open(link_driver.argument(), path.cstring(), O_RDONLY)) < 0 ){
		return -1;
	}
	ret = link_ioctl(link_driver.argument(), fd, I_APPFS_RECLAIM_RAM);
	link_close(link_driver.argument(), fd);
#else
	if( (fd = ::open(path.cstring(), O_RDONLY)) < 0 ){
		return -1;
	}
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
				"/dev/sys",
				fs::OpenFlags::read_write()
				) < 0 ){
		return -1;
	}

	ret = sys.ioctl(
				IoRequest(I_SYS_GETINFO),
				IoArgument(&info)
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
				"/dev/sys",
				fs::OpenFlags::read_write()
				) < 0 ){
		return var::String();
	}

	ret = sys.ioctl(
				IoRequest(I_SYS_GETINFO),
				IoArgument(&info)
				);

	sys.close();

	if( ret >= 0 ){
		return var::String(info.kernel_version);
	}
	return var::String();
}

void Sys::powerdown(const chrono::MicroTime & duration){
	::powerdown(duration.seconds());
}

int Sys::hibernate(const chrono::MicroTime & duration){
	return ::hibernate(duration.seconds());
}

int Sys::request(
		KernelRequest request,
		KernelArgument arg
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
				IoRequest(I_SYS_GETBOARDCONFIG),
				IoArgument(&config)
				);
}



#endif

SysInfo Sys::get_info(
		SAPI_LINK_DRIVER
		){
	sys_info_t sys_info;
	Sys sys;
	SAPI_LINK_SET_DRIVER(sys, link_driver.argument());
	sys.set_keep_open(false);
	if( sys.open() < 0 ){ return SysInfo(); }
	if( sys.get_info(sys_info) < 0 ){ return SysInfo(); }
	return SysInfo(sys_info);
}


bool Sys::is_authenticated(
		SAPI_LINK_DRIVER
		){
	Sys sys;
	SAPI_LINK_SET_DRIVER(sys, link_driver.argument());
	sys.set_keep_open(false);
	if( sys.open() < 0 ){ return 0; }
	return sys.ioctl(
				IoRequest(I_SYS_ISAUTHENTICATED)
				);
}

sys_secret_key_t Sys::get_secret_key(
		SAPI_LINK_DRIVER
		){
	sys_secret_key_t result = {0};
	Sys sys;
	SAPI_LINK_SET_DRIVER(sys, link_driver.argument());
	sys.set_keep_open(false);
	if( sys.open() < 0 ){
		return result;
	}
	sys.ioctl(
				IoRequest(I_SYS_GETSECRETKEY),
				IoArgument(&result)
				);
	return result;
}


SerialNumber Sys::get_serial_number(){
	return get_info().serial_number();
}





int Sys::get_info(sys_info_t & info){
	return ioctl(
				IoRequest(I_SYS_GETINFO),
				IoArgument(&info)
				);
}

int Sys::get_23_info(sys_23_info_t & attr){
	return ioctl(
				IoRequest(I_SYS_23_GETINFO),
				IoArgument(&attr)
				);
}

int Sys::get_26_info(sys_26_info_t & attr){
	return ioctl(
				IoRequest(I_SYS_26_GETINFO),
				IoArgument(&attr)
				);
}

int Sys::get_id(sys_id_t & id){
	return ioctl(
				IoRequest(I_SYS_GETID),
				IoArgument(&id)
				);
}

