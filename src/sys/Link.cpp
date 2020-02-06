/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
/* Copyright 2016-2018 Tyler Gilbert ALl Rights Reserved */


#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <sstream>
#include <string>
#include <sos/dev/sys.h>
#include <sos/fs/sysfs.h>

#define MAX_TRIES 3

#include "fs/File.hpp"
#include "sys/Link.hpp"
#include "sys/Appfs.hpp"
#include "chrono/Timer.hpp"

using namespace sys;
using namespace fs;

static var::String gen_error(const var::String & msg, int err_number){
	var::String s;
	s.sprintf("%s (%d)", msg.cstring(), err_number);
	return s;
}

Link::Link(){
	//check to see if the device is connected -- if it is not connected, connect to it
	m_stdout_fd = -1;
	m_stdin_fd = -1;
	m_lock = 0;
	m_is_bootloader = false;
	m_error_message = "";
	m_driver = &m_driver_instance;
	link_load_default_driver(m_driver);
}

Link::~Link(){}

int Link::check_error(int err){
	switch(err){
		case LINK_PHY_ERROR:
			m_error_message = "Physical Connection Error";
			this->disconnect();
			return LINK_PHY_ERROR;
		case LINK_PROT_ERROR:
			m_error_message = "Protocol Error";
			return LINK_PROT_ERROR;
	}
	return err;
}


void Link::reset_progress(){
	m_progress = 0;
	m_progress_max = 0;
}

var::Vector<var::String> Link::get_port_list(){
	var::Vector<var::String> result;
	var::Data device_name(256);
	var::String last_device;


	while( driver()->getname(
					 device_name.to_char(),
					 last_device.cstring(),
					 static_cast<int>(device_name.capacity())
					 ) == 0 ){
		var::String device_string(
					device_name.to_const_char(),
					var::String::Length(
						strnlen(
							device_name.to_const_char(),
							device_name.size()
							)
						)
					);
		result.push_back( device_string ); //this will make a copy of device name and put it on the list
		last_device = device_string;
	}

	return result;
}

var::Vector<LinkInfo> Link::get_info_list(){
	var::Vector<LinkInfo> result;
	var::Vector<var::String> port_list = get_port_list();

	//disconnect if already connected
	disconnect();

	for(u32 i = 0; i < port_list.count(); i++){
		//ping and grab the info
		if( connect(
					port_list.at(i)
					) < 0 ){
			//couldn't connect
		} else {
			result.push_back(LinkInfo(port_list.at(i), sys_info()));
			disconnect();
		}
	}

	return result;
}

int Link::connect(
		const var::String & path,
		IsLegacy is_legacy
		){
	int err = -1;

	reset_progress();


	if ( m_driver->phy_driver.handle == LINK_PHY_OPEN_ERROR ){

		m_driver->transport_version = 0;
		m_driver->phy_driver.handle =
				m_driver->phy_driver.open(
					path.cstring(),
					m_driver->options
					);
		if( m_driver->phy_driver.handle == LINK_PHY_OPEN_ERROR ){
			m_error_message = "Failed to Connect to Device";
			return -1;
		}

	} else {
		m_error_message.format("Already Connected (%d)", 1);
		return -1;
	}

	m_is_legacy = is_legacy.argument();

	if( m_is_legacy ){
		err = link_isbootloader_legacy(m_driver);
	} else {
		err = link_isbootloader(m_driver);
	}

	if ( err > 0 ){
		m_is_bootloader = true;
	} else if ( err == 0){
		m_is_bootloader = false;
	} else {
		m_error_message.sprintf("Failed to check for Bootloader status (%d)", link_errno);
		m_driver->phy_driver.close(&m_driver->phy_driver.handle);
		return -1;
	}

	sys_info_t sys_info;
	if( m_is_bootloader == false ){
		link_get_sys_info(driver(), &sys_info);
	} else {
		get_bootloader_attr(m_bootloader_attributes);
		memset(&sys_info, 0, sizeof(sys_info));
		strcpy(sys_info.name, "bootloader");
		sys_info.hardware_id = m_bootloader_attributes.hardware_id;
		memcpy(&sys_info.serial, m_bootloader_attributes.serialno, sizeof(mcu_sn_t));
	}

	m_link_info.set_port(path).set_info(sys_info);

	if( File::default_driver() == nullptr ){
		File::set_default_driver( File::LinkDriver(driver()) );
	}

	return 0;
}


int Link::reconnect(
		RetryCount retries,
		RetryDelay delay
		){
	int result;
	LinkInfo last_info(info());
	for(u32 i = 0; i < retries.argument(); i++){
		result = connect(last_info.port());
		if( result >= 0 ){
			if( last_info.serial_number() == info().serial_number() ){
				return 0;
			}
			disconnect();
		}

		var::Vector<var::String> port_list = get_port_list();
		for(u32 j=0; j < port_list.count(); j++){
			result = connect(port_list.at(j));
			if( result < 0 ){
				//didn't connect
			} else {
				if( last_info.serial_number() == info().serial_number() ){
					return 0;
				}
				disconnect();
			}
		}

		delay.argument().wait();
	}

	//restore the last known information on failure
	m_link_info = last_info;

	return -1;
}

int Link::open(
		const var::String & path,
		const OpenFlags & flags,
		const Permissions & permissions
		){
	int err = -1;
	if ( m_is_bootloader ){
		return -1;
	}

	for(int tries = 0; tries < MAX_TRIES; tries++){

		err = link_open(
					m_driver,
					path.cstring(),
					flags.o_flags(),
					permissions.permissions()
					);
		if(err != LINK_PROT_ERROR) break;
	}

	if ( err < 0 ){
		m_error_message.format(
					"Failed to open file: %s  (%d)",
					path.cstring(),
					link_errno
					);
	}
	return check_error(err);
}

int Link::close(int fd){
	int err = -1;
	if ( m_is_bootloader ){ return -1; }

	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_close(m_driver, fd);
		if(err != LINK_PROT_ERROR) break;
	}

	if ( err < 0 ){
		m_error_message.sprintf("Failed to close file", link_errno);
	}
	return check_error(err);
}


int Link::read(
		FileDescriptor fd,
		void * buf,
		File::Size nbyte
		){
	int err = -1;
	if ( m_is_bootloader ){
		return -1;
	}

	for(int tries = 0; tries < MAX_TRIES; tries++){
		err =  link_read(
					m_driver,
					fd.argument(),
					buf,
					static_cast<int>(nbyte.argument())
					);
		if(err != LINK_PROT_ERROR) break;
	}

	if ( err < 0 ){
		m_error_message.sprintf("Failed to read", link_errno);
	}
	return check_error(err);
}

int Link::write(
		FileDescriptor fd,
		const void * buf,
		fs::File::Size nbyte){
	int err = -1;
	if ( m_is_bootloader ){
		return -1;
	}

	for(int tries = 0; tries < MAX_TRIES; tries++){
		err =  link_write(
					m_driver,
					fd.argument(),
					buf,
					static_cast<int>(nbyte.argument())
					);
		if(err != LINK_PROT_ERROR) break;
	}
	if ( err < 0 ){
		m_error_message.sprintf("Failed to write", link_errno);
	}

	return check_error(err);

}

int Link::read_flash(int addr, void * buf, int nbyte){
	int err = -1;

	for(int tries = 0; tries < MAX_TRIES; tries++){
		err =  link_readflash(m_driver, addr, buf, nbyte);
		if(err != LINK_PROT_ERROR) break;
	}
	if ( err < 0 ){
		m_error_message.sprintf("Failed to read flash", link_errno);
	}

	return check_error(err);

}

int Link::get_is_executing(
		const var::String & name
		){
	sys_taskattr_t task;
	u32 id;
	int err;
	int fd;

	if ( m_is_bootloader ){
		return -1;
	}

	if ( this->get_is_connected() == false ){
		return -1;
	}

	fd = open(
				"/dev/sys",
				fs::OpenFlags::read_write()
				);

	if( fd < 0 ){
		this->m_error_message = "Failed to Open /dev/sys";
		return -1;
	}

	id = 0;

	do {
		task.is_enabled = 0;
		task.tid = id;
		memset(task.name, 0, 24);
		err = this->ioctl(
					FileDescriptor(fd),
					fs::File::IoRequest(I_SYS_GETTASK),
					fs::File::IoArgument(&task)
					);

		if( err > 0 ){
			if( task.is_enabled != 0 ){
				if( var::String(task.name) == name ){
					close(fd);
					return static_cast<int>(task.pid);
				}
			}
		}
		id++;
	} while( err >= 0 );

	if( close(fd) < 0 ){
		return -1;
	}

	return -1;

}

int Link::write_flash(int addr, const void * buf, int nbyte){
	int err = -1;

	for(int tries = 0; tries < MAX_TRIES; tries++){
		err =  link_writeflash(m_driver, addr, buf, nbyte);
		if(err != LINK_PROT_ERROR) break;
	}
	if ( err < 0 ){
		m_error_message.sprintf("Failed to write flash", link_errno);
	}

	return check_error(err);
}


int Link::lseek(
		FileDescriptor fd,
		fs::File::Location offset,
		int whence
		){
	int err = -1;
	if ( m_is_bootloader ){
		return -1;
	}


	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_lseek(
					m_driver,
					fd.argument(),
					offset.argument(),
					whence
					);
		if(err != LINK_PROT_ERROR) break;
	}
	if ( err < 0 ){
		m_error_message.sprintf("Failed to lseek", link_errno);
	}

	return check_error(err);

}


int Link::ioctl(
		FileDescriptor fd,
		fs::File::IoRequest request,
		fs::File::IoArgument arg
		){
	int err = -1;
	if ( m_is_bootloader ){
		return -1;
	}

	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_ioctl(
					m_driver,
					fd.argument(),
					request.argument(),
					arg.argument()
					);
		if(err != LINK_PROT_ERROR) break;
	}

	if ( err < 0 ){
		m_error_message.format("Failed to ioctl", link_errno);
	}

	return check_error(err);

}

int Link::disconnect(){

	if ( m_driver->phy_driver.handle != LINK_PHY_OPEN_ERROR ){
		link_disconnect(m_driver);

		if ( m_driver->phy_driver.handle != LINK_PHY_OPEN_ERROR ){
			//can't unlock the device if it has been destroyed
		}
	}

	if( File::default_driver() == driver() ){
		File::set_default_driver(File::LinkDriver(nullptr));
	}

	m_is_bootloader = false;
	m_error_message = "";
	m_stdout_fd = -1;
	m_stdin_fd = -1;
	return 0;

}

void Link::set_disconnected(){
	m_driver->transport_version = 0;
	m_driver->phy_driver.handle = LINK_PHY_OPEN_ERROR;
}

bool Link::is_connected() const {
	if( m_driver->phy_driver.handle == LINK_PHY_OPEN_ERROR ){
		return false;
	}

	return true;
}


int Link::stat(
		const var::String & path,
		struct link_stat & st
		){
	int err = -1;
	if ( m_is_bootloader ){
		return -1;
	}

	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_stat(
					m_driver,
					path.cstring(),
					&st
					);
		if(err != LINK_PROT_ERROR) break;
	}

	if( err < 0 ){
		m_error_message.sprintf("Failed to Get Stat", link_errno);
	}
	return check_error(err);
}

int Link::get_time(struct tm * gt){
	int err = -1;
	struct link_tm ltm;
	if ( m_is_bootloader ){
		return -1;
	}

	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_gettime(m_driver, &ltm);
		if(err != LINK_PROT_ERROR) break;
	}

	if ( err < 0 ){
		m_error_message.sprintf("Failed to Get Time", link_errno);
	} else {
		gt->tm_hour = ltm.tm_hour;
		gt->tm_isdst = ltm.tm_isdst;
		gt->tm_mday = ltm.tm_mday;
		gt->tm_min = ltm.tm_min;
		gt->tm_mon = ltm.tm_mon;
		gt->tm_sec = ltm.tm_sec;
		gt->tm_wday = ltm.tm_wday;
		gt->tm_yday = ltm.tm_yday;
		gt->tm_year = ltm.tm_year;
	}
	return check_error(err);
}

int Link::set_time(struct tm * gt){
	int err = -1;
	struct link_tm ltm;

	ltm.tm_hour = gt->tm_hour;
	ltm.tm_isdst = gt->tm_isdst;
	ltm.tm_mday = gt->tm_mday;
	ltm.tm_min = gt->tm_min;
	ltm.tm_mon = gt->tm_mon;
	ltm.tm_sec = gt->tm_sec;
	ltm.tm_wday = gt->tm_wday;
	ltm.tm_yday = gt->tm_yday;
	ltm.tm_year = gt->tm_year;

	if ( m_is_bootloader ){
		m_error_message.format("can't set time for bootloader");
		return -1;
	}

	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_settime(m_driver, &ltm);
		if(err != LINK_PROT_ERROR) break;
	}

	if ( err < 0 ){
		m_error_message.format("failed to set time with device errno %d", link_errno);
	}
	return check_error(err);
}

//Operations on the device
int Link::mkdir(
		const var::String & path,
		const Permissions permissions
		){
	int err = -1;
	if ( m_is_bootloader ){
		return -1;
	}

	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_mkdir(
					m_driver,
					path.cstring(),
					permissions.permissions()
					);
		if(err != LINK_PROT_ERROR) break;
	}

	if ( err < 0 ){
		m_error_message.format(
					"Failed to create %s (%d)",
					path.cstring(),
					link_errno
					);
	}
	return check_error(err);
}

int Link::rmdir(
		const var::String & path
		){
	int err = -1;
	if ( m_is_bootloader ){
		return -1;
	}

	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_rmdir(m_driver, path.cstring());
		if(err != LINK_PROT_ERROR) break;
	}

	if ( err < 0 ){
		m_error_message.format(
					"Failed to remove %s (%d)",
					path.cstring(),
					link_errno
					);
	}
	return check_error(err);
}

var::String Link::convert_permissions(link_mode_t mode){
	var::String result;

	link_mode_t type;
	type = mode & LINK_S_IFMT;
	switch(type){
		case LINK_S_IFDIR:
			result = "d";
			break;
		case LINK_S_IFCHR:
			result = "c";
			break;
		case LINK_S_IFBLK:
			result = "b";
			break;
		case LINK_S_IFLNK:
			result = "l";
			break;
		case LINK_S_IFREG:
			result = "-";
			break;
		default:
			result = "x";
	}

	if( mode & LINK_S_IROTH ){
		result += "r";
	} else {
		result += "-";
	}

	if( mode & LINK_S_IWOTH ){
		result += "w";
	} else {
		result += "-";
	}

	if( mode & LINK_S_IXOTH ){
		result += "x";
	} else {
		result += "-";
	}

	if( mode & LINK_S_IRGRP ){
		result += "r";
	} else {
		result += "-";
	}

	if( mode & LINK_S_IWGRP ){
		result += "w";
	} else {
		result += "-";
	}

	if( mode & LINK_S_IXGRP ){
		result += "x";
	} else {
		result += "-";
	}

	if( mode & LINK_S_IRUSR ){
		result += "r";
	} else {
		result += "-";
	}

	if( mode & LINK_S_IWUSR ){
		result += "w";
	} else {
		result += "-";
	}

	if( mode & LINK_S_IXUSR ){
		result += "x";
	} else {
		result += "-";
	}

	return result;

}


int Link::opendir(const var::String & path){
	int err = -1;
	if ( m_is_bootloader ){
		return 0;
	}

	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_opendir(m_driver, path.cstring());
		if(err != LINK_PROT_ERROR) break;
	}

	if ( err < 0 ){
		m_error_message.format(
					"Failed to open %s (%d)",
					path.cstring(),
					link_errno
					);
	}
	return check_error(err);
}

int Link::readdir_r(
		int dirp, struct link_dirent * entry, struct link_dirent ** result){
	int err = -1;
	if ( m_is_bootloader ){
		return -1;
	}

	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_readdir_r(m_driver, dirp, entry, result);
		if(err != LINK_PROT_ERROR) break;
	}

	if ( err < 0 ){
		m_error_message.sprintf("Failed to read directory", link_errno);
		return -1;
	}
	return check_error(err);
}

int Link::closedir(int dirp){
	int err = -1;
	if ( m_is_bootloader ){
		return -1;
	}

	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_closedir(m_driver, dirp);
		if(err != LINK_PROT_ERROR) break;
	}

	if ( err < 0 ){
		m_error_message.sprintf("Failed to close directory (%d)", link_errno);
	}
	return check_error(err);
}

int Link::symlink(
		SourcePath old_path,
		DestinationPath new_path
		){
	int err = -1;
	if ( m_is_bootloader ){
		return -1;
	}

	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_symlink(
					m_driver,
					old_path.argument().cstring(),
					new_path.argument().cstring()
					);
		if(err != LINK_PROT_ERROR) break;
	}

	if ( err < 0 ){
		m_error_message.sprintf("Failed to create symlink %s (%d)",
														new_path.argument().cstring(),
														link_errno);
	}
	return check_error(err);
}

int Link::unlink(const var::String & path){
	int err = -1;
	if ( m_is_bootloader ){
		return -1;
	}

	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_unlink(m_driver, path.cstring());
		if(err != LINK_PROT_ERROR) break;
	}

	if ( err < 0 ){
		m_error_message.format(
					"Failed to remove %s (%d)",
					path.cstring(),
					link_errno
					);
	}
	return check_error(err);
}

int Link::copy(
		SourcePath src,
		DestinationPath dest,
		const Permissions & permissions,
		IsCopyToDevice is_to_device,
		const ProgressCallback * progress_callback
		){
	int err;
	struct link_stat st;
	File host_file;
	File device_file = File(
				fs::File::LinkDriver(driver())
				);
	int copy_page_size = 4096;

	if ( m_is_bootloader ){
		return -1;
	}

	err = 0;

	if ( is_to_device.argument() == true ){

		m_progress_max = 0;
		m_progress = 0;

		//Open the host file
		if( host_file.open(
					src.argument(),
					OpenFlags::read_only()
					) < 0 ){
			m_error_message.sprintf("Could not find file %s on host", src.argument().cstring());
			return -1;
		}

		m_progress_max =
				static_cast<int>(host_file.size());
		if( host_file.size() > 256*50 ){
			copy_page_size = host_file.size() / 50;
			copy_page_size -= (copy_page_size % 1024);
		} else {
			copy_page_size = 256;
		}

		var::String dest_file = dest.argument();
		if( dest_file.create_sub_string(
					var::String::Position(0),
					var::String::Length(4)
					) == "/app" ){
			int result;
			var::String dest_name = File::name(dest.argument());

			result =
					Appfs::create(
						Appfs::Name(dest_name),
						host_file,
						Appfs::MountPath("/app"),
						progress_callback,
						fs::File::LinkDriver(driver())
						);
			if( result < 0 ){
				m_error_message.format("Failed to write file %s on device", dest.argument().cstring());
			}

		} else {

			//Create the device file

			if( device_file.create(
						dest.argument(),
						fs::File::IsOverwrite(true),
						permissions) < 0 ){
				m_error_message.sprintf("Failed to create file %s on device", dest.argument().cstring(), link_errno);

				return -1;
			}

			m_error_message = "";
			int result = device_file.write(
						host_file,
						fs::File::PageSize(
							static_cast<u32>(copy_page_size)
							),
						fs::File::Size(host_file.size()),
						progress_callback
						);
			if( result < 0 ){

			}
			if( device_file.close() < 0 ){
				m_error_message.sprintf("Failed to close Link device file (%d)", link_errno);

				return -1;
			}

		}


		return err;

	} else {


		if ( link_stat(m_driver, src.argument().cstring(), &st) < 0 ){
			m_error_message = "Failed to get target file size";
			return -1;
		}

		m_progress = 0;
		m_progress_max = 0;
		//Copy the source file from the device to the host
		if( host_file.create(
					dest.argument(),
					fs::File::IsOverwrite(true),
					permissions
					) < 0 ){
			m_error_message.sprintf("Failed to open file %s on host", dest.argument().cstring());
			return -1;
		}


		//Open the device file
		if( device_file.open(
					src.argument(),
					fs::OpenFlags::read_only()
					) < 0 ){
			m_error_message.format(
						"Failed to open file %s on device (%d)",
						src.argument().cstring(),
						link_errno
						);
			return -1;
		}

		m_progress_max = st.st_size;
		if( device_file.size() > 256*50 ){
			copy_page_size = device_file.size() / 50;
		} else {
			copy_page_size = 256;
		}

		if( host_file.write(
					device_file,
					fs::File::PageSize(copy_page_size),
					fs::File::Size(device_file.size()),
					progress_callback
					) < 0 ){
			m_error_message.format(
						"failed to write to host file"
						);
		}

		if( device_file.close() < 0 ){
			m_error_message.format(
						"Failed to close file %s on device (%d)",
						src.argument().cstring(),
						link_errno
						);
			return -1;
		}

		if( host_file.close() < 0 ){
			m_error_message.format(
						"Failed to close file %s on host",
						dest.argument().cstring()
						);
			return -1;
		}

	}
	return 0;
}

int Link::run_app(const var::String & path){
	int err = -1;
	if ( m_is_bootloader ){
		return -1;
	}

	if( path.length() >= LINK_PATH_ARG_MAX-1 ){
		m_error_message = "Path argument exceeds max";
		return -1;
	}


	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_exec(m_driver, path.cstring());
		if(err != LINK_PROT_ERROR) break;
	}

	if ( err < 0 ){
		if ( err == LINK_TRANSFER_ERR ){
			m_error_message = "Connection Failed";
			this->disconnect();
			return -2;
		} else {
			m_error_message.format(
						"Failed to run program -> %s (%d, %d)",
						path.cstring(),
						err,
						link_errno
						);
			return -1;
		}
	}
	return err;
}

int Link::format(const var::String & path){
	int err = -1;
	if ( m_is_bootloader ){
		return -1;
	}
	m_error_message = "";
	//Format the filesystem

	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_mkfs(
					m_driver,
					path.cstring()
					);
		if(err != LINK_PROT_ERROR) break;
	}

	if ( err < 0 ){
		m_error_message.format(
					"failed to format filesystem with device errno %d",
					link_errno
					);
	}
	return check_error(err);
}

int Link::kill_pid(int pid, int signo){
	int err = -1;
	if ( m_is_bootloader ){
		return -1;
	}

	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_kill_pid(m_driver, pid, signo);
		if(err != LINK_PROT_ERROR) break;
	}

	if ( err < 0 ){
		m_error_message.sprintf("Failed to kill process %d with device errno %d", pid, link_errno);
	}
	return check_error(err);
}

int Link::reset(){
	link_reset(m_driver);
	m_driver->transport_version = 0;
	m_driver->phy_driver.handle = LINK_PHY_OPEN_ERROR;
	return 0;
}

int Link::reset_bootloader(){

	link_resetbootloader(m_driver);

	m_driver->transport_version = 0;
	m_driver->phy_driver.handle = LINK_PHY_OPEN_ERROR;
	return 0;
}

int Link::rename(
		SourcePath old_path,
		DestinationPath new_path
		){
	int err = -1;
	if ( m_is_bootloader ){
		return -1;
	}

	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_rename(
					m_driver,
					old_path.argument().cstring(),
					new_path.argument().cstring()
					);
		if(err != LINK_PROT_ERROR) break;
	}

	if( err < 0 ){
		m_error_message.sprintf("failed to rename file with device errno %d", link_errno);
	}
	return check_error(err);
}

int Link::chown(
		const var::String & path,
		File::OwnerId owner,
		File::GroupId group
		){
	int err = -1;
	if ( m_is_bootloader ){
		return -1;
	}

	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_chown(
					m_driver,
					path.cstring(),
					owner.argument(),
					group.argument()
					);
		if(err != LINK_PROT_ERROR) break;
	}

	if ( err < 0 ){
		m_error_message.sprintf("Failed to chown file with device errno %d", link_errno);
	}
	return check_error(err);
}

int Link::chmod(
		const var::String & path,
		const Permissions & permissions
		){
	int err = -1;
	if ( m_is_bootloader ){
		m_error_message = "target is a bootloader";
		return -1;
	}

	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_chmod(
					m_driver,
					path.cstring(),
					permissions.permissions()
					);
		if(err != LINK_PROT_ERROR) break;
	}

	if ( err < 0 ){
		m_error_message.format(
					"Failed to chmod file with device errno %d",
					link_errno
					);
	}
	return check_error(err);

}

int Link::get_bootloader_attr(bootloader_attr_t & attr){
	int err = -1;
	if ( m_is_bootloader == false ){
		m_error_message = "Target is not a bootloader";
		return -1;
	}

	if( m_is_legacy ){
		err = link_bootloader_attr_legacy(m_driver, &attr, 0);
	} else {
		err = link_bootloader_attr(m_driver, &attr, 0);
	}
	if( err < 0 ){
		m_error_message = "Failed to read attributes";
		return -1;
	}

	return 0;
}

u32 Link::validate_os_image_id_with_connected_bootloader(
		const File & source_image
		){
	int err = -1;
	u32 image_id;

	if ( m_is_bootloader == false ){
		m_error_message = "Target is not a bootloader";
		return 0;
	}

	//now write the OS to the device using link_writeflash()
	m_progress_max = 0;
	m_progress = 0;

	if( source_image.seek(
				fs::File::Location(BOOTLOADER_HARDWARE_ID_OFFSET),
				File::SET
				) < 0 ){
		m_error_message = "Failed to seek to bootloader image hardware ID value";
		return 0;
	}

	if( source_image.read(
				&image_id,
				fs::File::Size(sizeof(u32))
				) != sizeof(u32) ){
		m_error_message = "Failed to read bootloader image id";
		return 0;
	}

	if( source_image.seek(fs::File::Location(0), File::SET) < 0 ){
		m_error_message = "Failed to seek bootloader image start";
		return 0;
	}

	m_progress_max = static_cast<int>(source_image.size());

	if( (image_id & ~0x01) != (m_bootloader_attributes.hardware_id & ~0x01) ){
		err = -1;
		m_error_message.format("Kernel Image ID (0x%X) does not match Bootloader ID (0x%X)",
													 image_id,
													 m_bootloader_attributes.hardware_id);
		return 0;
	}

	return image_id;
}

int Link::erase_os(
		Printer & progress_printer,
		BootloaderRetryCount bootloader_retry_total
		){
	int err;

	if ( m_is_bootloader == false ){
		m_error_message = "Target is not a bootloader";
		return -1;
	}

	const sys::ProgressCallback * progress_callback =
			progress_printer.progress_callback();

	progress_printer.progress_key() = "erasing";


	if( progress_callback ){
		progress_callback->update(0, ProgressCallback::indeterminate_progress_total());
	}
	//first erase the flash
	err = link_eraseflash(m_driver);

	if ( err < 0 ){
		if( progress_callback ){ progress_callback->update(0, 0); }
		m_error_message = "Failed to erase flash";
		return check_error(err);
	}

	bootloader_attr_t attr;
	memset(&attr, 0, sizeof(attr));
	int retry = 0;
	do {
		chrono::wait(chrono::Milliseconds(500));
		err = get_bootloader_attr(attr);
		if( progress_callback ){
			progress_callback->update(
						retry,
						ProgressCallback::indeterminate_progress_total()
						);
		}
	} while (
					 (err < 0) &&
					 (retry++ < bootloader_retry_total.argument())
					 );

	chrono::wait(chrono::Milliseconds(250));

	//flush just incase the protocol gets filled with get attr requests
	driver()->phy_driver.flush( driver()->phy_driver.handle );

	if( progress_callback ){ progress_callback->update(0, 0); }


	if( err < 0 ){
		var::String error = m_error_message;
		m_error_message.format("Failed to ping bootloader after erase -> %s",
													 error.cstring());
		return err;
	}

	return 0;
}

int Link::install_os(const fs::File & image,
										 IsVerify is_verify,
										 HardwareId image_id,
										 Printer & progress_printer
										 ){

	//must be connected to the bootloader with an erased OS
	int err = -1;
	int bytes_read;
	const int buffer_size = 1024;

	const sys::ProgressCallback * progress_callback =
			progress_printer.progress_callback();

	var::Data start_address_buffer(256);
	var::Data buffer(buffer_size);
	var::Data compare_buffer(buffer_size);


	if ( m_is_bootloader == false ){
		m_error_message = "Target is not a bootloader";
		return -1;
	}


	if( image.seek(File::Location(0), File::SET) < 0 ){
		m_error_message = "Failed to seek bootloader image start";
		return -1;
	}

	u32 start_address = m_bootloader_attributes.startaddr;
	u32 loc = start_address;


	progress_printer.progress_key() = "installing";
	m_error_message = "";

	if( progress_callback ){ progress_callback->update(0, 100); }

	buffer.fill<u8>(0xff);

	bootloader_attr_t attr;
	if( get_bootloader_attr(attr) < 0 ){
		m_error_message = "failed to get bootloader attributes before write";
		return -1;
	}

	while(
				(bytes_read = image.read(buffer) ) > 0
				){

		if( loc == start_address ){
			//we want to write the first 256 bytes last because the bootloader checks this for a valid image
			start_address_buffer.copy_contents(
						buffer,
						File::Size(start_address_buffer.size())
						);

			//memcpy(stackaddr, buffer, 256);

			//for now write 0xff to the first 256 bytes telling the bootloader the image isn't valid yet
			//memset(buffer, 0xFF, 256);
			buffer.fill(
						(u8)0xff,
						var::Reference::Count(start_address_buffer.size())
						);

		}

		if ( (err = link_writeflash(
						m_driver,
						loc,
						buffer.to_const_void(),
						bytes_read
						)
					) != bytes_read ){
			m_error_message.format("Failed to write to link flash at 0x%x (%d, %d) -> try the operation again",
														 loc,
														 err, link_errno
														 );
			if ( err < 0 ){
				err = -1;
			}
			break;
		}


		loc += bytes_read;
		m_progress += bytes_read;
		if( progress_callback && (progress_callback->update(m_progress, m_progress_max) == true)){
			break;
		}
		err = 0;
	}

	if ( err == 0 ){

		if ( is_verify.argument() == true ){

			image.seek(File::Location(0), File::SET);
			loc = start_address;
			m_progress = 0;

			progress_printer.progress_key() = "verifying";

			while(
						(bytes_read = image.read(buffer)) > 0
						){

				if ( (err = link_readflash(
								m_driver,
								loc,
								compare_buffer.to_void(),
								bytes_read)
							) != bytes_read ){
					m_error_message.sprintf("Failed to read flash memory", link_errno);
					if ( err > 0 ){
						err = -1;
					}
					break;

				} else {

					if( loc == start_address ){
						buffer.fill(
									(u8)0xff,
									var::Reference::Count(start_address_buffer.size())
									);
					}

					compare_buffer.resize(bytes_read);
					buffer.resize(bytes_read);

					if( compare_buffer != buffer ){
						//if ( memcmp((void*)cmpBuffer, buffer, bytes_read) != 0 ){
						m_error_message.sprintf("Failed to verify program installation", link_errno);

						return -1;
					}

					loc += bytes_read;
					m_progress += bytes_read;
					if( progress_callback && (progress_callback->update(m_progress, m_progress_max) == true)){
						break;
					}
					err = 0;
				}
			}
		}

		if( image_id.argument() != m_bootloader_attributes.hardware_id ){
			//if the LSb of the image_id doesn't match the bootloader HW ID, this will correct it
			var::Reference::memory_copy(
						var::Reference::SourceBuffer(&m_bootloader_attributes.hardware_id),
						var::Reference::DestinationBuffer(start_address_buffer.to_u8() + BOOTLOADER_HARDWARE_ID_OFFSET),
						File::Size( sizeof(u32) )
						);
		}

		//write the start block
		if( (err = link_writeflash(
					 m_driver,
					 start_address,
					 start_address_buffer.to_const_void(),
					 start_address_buffer.size())
				 ) != start_address_buffer.size() ){

			if( progress_callback ){
				progress_callback->update(0,0);
			}


			m_error_message.format(
						"Failed to write %d bytes to first block 0x%x (%d, %d)",
						start_address_buffer.size(),
						start_address,
						SYSFS_GET_RETURN(err),
						SYSFS_GET_RETURN_ERRNO(err)
						);

			link_eraseflash(m_driver);
			return -1;
		}


		if( is_verify.argument() == true ){
			//verify the stack address
			buffer.resize( start_address_buffer.size() );
			if( (err = link_readflash(
						 m_driver,
						 start_address,
						 buffer.to_void(),
						 start_address_buffer.size())
					 ) != start_address_buffer.size() ){
				m_error_message.format("Failed to write stack addr %d", err);
				if( progress_callback ){ progress_callback->update(0,0); }

				return -1;
			}

			if( buffer != start_address_buffer ){
				m_error_message = "Failed to verify stack address block";
				if( progress_callback ){ progress_callback->update(0,0); }
				link_eraseflash(m_driver);
				return -1;
			}
		}
	}

	if( progress_callback ){ progress_callback->update(0,0); }

	return check_error(err);
}

int Link::update_os(
		const fs::File & image,
		IsVerify is_verify,
		sys::Printer & progress_printer,
		BootloaderRetryCount bootloader_retry_total
		){

	u32 image_id = validate_os_image_id_with_connected_bootloader(
				image
				);

	if( image_id == 0 ){
		progress_printer.error(
					"failed to validate id '%s'",
					error_message().cstring()
					);
		return -1;
	}

	var::String progress_key = progress_printer.progress_key();

	if( erase_os(
				progress_printer,
				bootloader_retry_total
				)
			){
		progress_printer.error("failed to erase os '%s'",
													 error_message().cstring()
													 );
		progress_printer.progress_key() = progress_key;
		return -1;
	}

	if( install_os(
				image,
				is_verify,
				HardwareId(image_id),
				progress_printer
				) < 0 ){
		progress_printer.error("failed to install os '%s'",
													 error_message().cstring()
													 );
		progress_printer.progress_key() = progress_key;
		return -1;
	}

	progress_printer.progress_key() = progress_key;
	return 0;
}

int Link::update_os(
		const fs::File & image,
		IsVerify is_verify,
		const ProgressCallback * progress_callback,
		BootloaderRetryCount bootloader_retry_total
		){
	int err;
	uint32_t loc;
	int bytesRead;
	char stackaddr[256];
	const int buffer_size = 1024;
	unsigned char buffer[buffer_size];
	unsigned char cmpBuffer[buffer_size];
	int i;
	uint32_t startAddr;
	uint32_t image_id;

	if ( m_is_bootloader == false ){
		m_error_message = "Target is not a bootloader";
		return -1;
	}

	//now write the OS to the device using link_writeflash()
	m_progress_max = 0;
	m_progress = 0;

	if( image.seek(
				File::Location(BOOTLOADER_HARDWARE_ID_OFFSET),
				File::SET
				) < 0 ){
		m_error_message = "Failed to seek to bootloader image hardware ID value";
		return -1;
	}

	if( image.read(
				&image_id,
				File::Size(sizeof(u32))
				) != sizeof(u32) ){
		m_error_message = "Failed to read bootloader image id";
		return -1;
	}

	if( image.seek(File::Location(0), File::SET) < 0 ){
		m_error_message = "Failed to seek bootloader image start";
		return -1;
	}

	m_progress_max = static_cast<int>(image.size());

#if 0
	err = get_bootloader_attr(attr);
	if( err < 0 ){
		return check_error(err);
	}
#endif

	startAddr = m_bootloader_attributes.startaddr;
	loc = startAddr;

	if( (image_id & ~0x01) !=
			(m_bootloader_attributes.hardware_id & ~0x01) ){
		err = -1;
		m_error_message.format("Kernel Image ID (0x%X) does not match Bootloader ID (0x%X)",
													 image_id,
													 m_bootloader_attributes.hardware_id);
		return check_error(err);
	}

	if( progress_callback ){ progress_callback->update(0, 100); }
	//first erase the flash
	err = link_eraseflash(m_driver);

	if ( err < 0 ){

		if( progress_callback ){ progress_callback->update(0, 0); }
		m_error_message = "Failed to erase flash";
		return check_error(err);
	}

	bootloader_attr_t attr;
	int retry = 0;
	do {
		err = get_bootloader_attr(attr);
	} while ( err < 0 && retry++ < bootloader_retry_total.argument());
	if( err < 0 ){
		m_error_message = "Failed to ping bootloader after erase (try the operation again)";
		return err;
	}

	m_error_message = "";

	while( (bytesRead = image.read(
						buffer,
						File::Size(buffer_size)
						)) > 0 ){

		if( loc == startAddr ){
			//we want to write the first 256 bytes last because the bootloader checks this for a valid image
			memcpy(stackaddr, buffer, 256);

			//for now write 0xff to the first 256 bytes telling the bootloader the image isn't valid yet
			memset(buffer, 0xFF, 256);
		}

		if ( (err = link_writeflash(m_driver, loc, buffer, bytesRead)) != bytesRead ){
			m_error_message.format("Failed to write to link flash (%d) -> try the operation again", link_errno);
			if ( err < 0 ){
				err = -1;
			}
			break;
		}

		loc += bytesRead;
		m_progress += bytesRead;
		if( progress_callback && (progress_callback->update(m_progress, m_progress_max) == true)){
			break;
		}
		err = 0;
	}

	if ( err == 0 ){

		if ( is_verify.argument() == true ){

			image.seek(File::Location(0), File::SET);
			loc = startAddr;
			m_progress = 0;

			while( (bytesRead = image.read(
								buffer,
								File::Size(buffer_size)
								)) > 0 ){

				if ( (err = link_readflash(m_driver, loc, cmpBuffer, bytesRead)) != bytesRead ){
					m_error_message.sprintf("Failed to read flash memory", link_errno);
					if ( err > 0 ){
						err = -1;
					}
					break;
				} else {

					if( loc == startAddr ){
						memset(buffer, 0xFF, 256);
					}

					if ( memcmp((void*)cmpBuffer, buffer, bytesRead) != 0 ){
						for(i=0; i < bytesRead; i++){
							if( buffer[i] != cmpBuffer[i] ){
								//printf("0x%X targ:0x%02X actual:0x%02X", loc + i, buffer[i], cmpBuffer[i]);
							}
						}
						m_error_message.sprintf("Failed to verify program installation", link_errno);

						return -1;
					}

					loc += bytesRead;
					m_progress += bytesRead;
					if( progress_callback && (progress_callback->update(m_progress, m_progress_max) == true)){
						break;
					}
					err = 0;
				}
			}
		}

		if( image_id != m_bootloader_attributes.hardware_id ){
			//if the LSb of the image_id doesn't match the bootloader HW ID, this will correct it
			memcpy(stackaddr + BOOTLOADER_HARDWARE_ID_OFFSET, &m_bootloader_attributes.hardware_id, sizeof(u32));
		}

		//write the stack address
		if( (err = link_writeflash(m_driver, startAddr, stackaddr, 256)) != 256 ){
			if( progress_callback ){ progress_callback->update(0,0); }
			m_error_message.format("Failed to write stack addr %d", err);
			return -1;
		}


		if( is_verify.argument() == true ){
			//verify the stack address
			if( (err = link_readflash(m_driver, startAddr, buffer, 256)) != 256 ){
				m_error_message.format("Failed to write stack addr %d", err);
				if( progress_callback ){ progress_callback->update(0,0); }

				return -1;
			}

			if( memcmp(buffer, stackaddr, 256) != 0 ){
				link_eraseflash(m_driver);
				m_error_message = "Failed to verify stack address";
				if( progress_callback ){ progress_callback->update(0,0); }

				return -1;
			}
		}

	}




	if( err < 0 ){
		link_eraseflash(m_driver);
	}

	if( progress_callback ){ progress_callback->update(0,0); }

	return check_error(err);
}

int Link::update_binary_install_options(
		const fs::File & file,
		const AppfsFileAttributes & attributes
		){
	return attributes.apply(file);
}

int Link::install_app(
		const fs::File & application_image,
		Path path,
		ApplicationName name,
		const ProgressCallback * progress_callback
		){
	int bytes_read;
	int fd;
	appfs_installattr_t attr;
	int bytes_cumm;
	int bytes_total;
	int loc_err;

	if( path.argument().find("/app") == 0 ){

		fd = this->open(
					"/app/.install",
					fs::OpenFlags::write_only()
					);

		if( fd < 0 ){
			m_error_message.format(
						"Failed to open destination /app/.install (%d, %d)",
						link_errno,
						fd);
			return -1;
		}

		attr.loc = 0;

		bytes_total = application_image.size();
		bytes_cumm = 0;
		//make sure to instal from the beginning -- file is already open
		application_image.seek( File::Location(0), File::SET );

		do {
			memset(attr.buffer, 0xFF, APPFS_PAGE_SIZE);
			//bytes_read = fread((char*)attr.buffer, 1, APPFS_PAGE_SIZE, source_file);
			bytes_read = bytes_total - bytes_cumm;
			if( bytes_read > APPFS_PAGE_SIZE ){
				bytes_read = APPFS_PAGE_SIZE;
			}

			application_image.read(
						attr.buffer,
						File::Size(bytes_read)
						);

			if( bytes_cumm == 0 ){
				//app_file = (link_appfs_file_t *)attr.buffer;
			}

			if( bytes_read > 0 ){
				attr.nbyte = bytes_read;
				bytes_cumm += attr.nbyte;
				if( (loc_err = ioctl(
							 FileDescriptor(fd),
							 fs::File::IoRequest(I_APPFS_INSTALL),
							 fs::File::IoArgument(&attr)
							 )) < 0 ){

					if( link_errno == 5 ){ //EIO
						if( loc_err < -1 ){
							m_error_message.format(
										"Failed to install because of missing symbol on device at " F32D,
										-1*(loc_err+1)
										);
						} else {
							m_error_message
									= "Failed to install because of unknown symbol error";
						}
					} else if( link_errno == 8 ){ //ENOEXEC
						m_error_message = "Failed to install because of symbol table signature mismatch";
					} else {
						m_error_message.format(
									"Failed to install with result %d and errno %d",
									loc_err,
									link_errno
									);
					}
					close(fd);
					return -1;
				}
				if( progress_callback ){
					progress_callback->update(bytes_cumm,bytes_total);
				}
				attr.loc += APPFS_PAGE_SIZE;
			}
		} while( bytes_read == APPFS_PAGE_SIZE );

		if( close(fd) < 0 ){
			m_error_message.format(
						"Failed to close file on device (%d)",
						link_errno
						);
			return -1;
		}

		if( progress_callback ){ progress_callback->update(0,0); }
	} else {

		File f = File(fs::File::LinkDriver(driver()));

		//copy the file to the destination directory
		var::String dest_str = path.argument() + "/" + name.argument();

		if( f.create(
					dest_str,
					File::IsOverwrite(true),
					Permissions(0555)
					) < 0 ){
			m_error_message.format(
						"failed to create %s on target",
						dest_str.cstring()
						);
			return -1;
		}

		if( f.write(
					application_image,
					File::PageSize(APPFS_PAGE_SIZE),
					File::Size(application_image.size()),
					progress_callback
					) < 0 ){
			return -1;
		}

		if( f.close() < 0 ){
			return -1;
		}
		if( progress_callback ){ progress_callback->update(0,0); }
	}

	return 0;
}




