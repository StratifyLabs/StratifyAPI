/* Copyright 2016-2018 Tyler Gilbert ALl Rights Reserved */


#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <sstream>
#include <string>
#include <sos/dev/sys.h>

#define MAX_TRIES 3

#include "sys/File.hpp"
#include "sys/Link.hpp"
#include "sys/Appfs.hpp"

using namespace sys;

static var::String gen_error(const var::String & msg, int err_number){
	var::String s;
	s.sprintf("%s (%d)", msg.str(), err_number);
	return s;
}

Link::Link(){
	//check to see if the device is connected -- if it is not connected, connect to it
	m_stdout_fd = -1;
	m_stdin_fd = -1;
	m_lock = 0;
	m_is_bootloader = false;
	m_status_message = "";
	m_error_message = "";
	m_driver = &m_default_driver;
	link_load_default_driver(m_driver);
}

Link::~Link(){}


int Link::lock_device(){
	return driver()->lock( driver()->dev.handle );
}

int Link::unlock_device(){
	return driver()->unlock( driver()->dev.handle );
}

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
	var::String device_name;
	var::String last_device;

	last_device.set_capacity(256);
	device_name.set_capacity(256);

	last_device.clear();
	device_name.clear();

	while( driver()->getname(device_name.cdata(), last_device.str(), device_name.capacity()) == 0 ){
		result.push_back( device_name ); //this will make a copy of device name and put it on the list
		last_device = device_name;
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
		if( connect(port_list.at(i)) < 0 ){
			//couldn't connect
		} else {
			result.push_back(LinkInfo(port_list.at(i), sys_info()));
			disconnect();
		}
	}

	return result;
}

int Link::connect(const var::ConstString & path, bool is_legacy){
	int err;

	reset_progress();

	if ( m_driver->dev.handle == LINK_PHY_OPEN_ERROR ){

		m_driver->dev.handle = m_driver->dev.open(path.str(), 0);
		if( m_driver->dev.handle == LINK_PHY_OPEN_ERROR ){
			m_error_message = "Failed to Connect to Device";
			return -1;
		}

	} else {
		m_error_message.format("Already Connected (%d)", 1);
		return -1;
	}

	m_is_legacy = is_legacy;

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
		m_driver->dev.close(&m_driver->dev.handle);
		return -1;
	}

	sys_info_t sys_info;
	if( m_is_bootloader == false ){
		link_get_sys_info(driver(), &sys_info);
	} else {
		bootloader_attr_t boot_attr;
		get_bootloader_attr(boot_attr);
		memset(&sys_info, 0, sizeof(sys_info));
		strcpy(sys_info.name, "bootloader");
		sys_info.hardware_id = boot_attr.hardware_id;
		memcpy(&sys_info.serial, boot_attr.serialno, sizeof(mcu_sn_t));
	}

	m_link_info.set(path, sys_info);

	if( File::default_driver() == 0 ){
		File::set_default_driver( driver() );
	}

	return 0;
}


int Link::reconnect(u32 retries, u32 delay_ms){
	int result;
	LinkInfo last_info(info());
	for(u32 i = 0; i < retries; i++){
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

		driver()->dev.wait(delay_ms);
	}

	//restore the last known information on failure
	m_link_info = last_info;

	return -1;
}

int Link::open(const var::ConstString & file, int flags, link_mode_t mode){
	int err;
	if ( m_is_bootloader ){
		return -1;
	}
	lock_device();
	for(int tries = 0; tries < MAX_TRIES; tries++){

		err = link_open(m_driver, file.c_str(), flags, mode);
		if(err != LINK_PROT_ERROR) break;
	}
	unlock_device();
	if ( err < 0 ){
		m_error_message.sprintf("Failed to open file: %s  (%d)", file.str(), link_errno);
	}
	return check_error(err);
}

int Link::close(int fd){
	int err;
	if ( m_is_bootloader ){
		return -1;
	}
	lock_device();
	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_close(m_driver, fd);
		if(err != LINK_PROT_ERROR) break;
	}
	unlock_device();
	if ( err < 0 ){
		m_error_message.sprintf("Failed to close file", link_errno);
	}
	return check_error(err);
}


int Link::read(int fd, void * buf, int nbyte){
	int err;
	if ( m_is_bootloader ){
		return -1;
	}
	lock_device();
	for(int tries = 0; tries < MAX_TRIES; tries++){
		err =  link_read(m_driver, fd, buf, nbyte);
		if(err != LINK_PROT_ERROR) break;
	}
	unlock_device();
	if ( err < 0 ){
		m_error_message.sprintf("Failed to read", link_errno);
	}
	return check_error(err);
}

int Link::write(int fd, const void * buf, int nbyte){
	int err;
	if ( m_is_bootloader ){
		return -1;
	}
	lock_device();
	for(int tries = 0; tries < MAX_TRIES; tries++){
		err =  link_write(m_driver, fd, buf, nbyte);
		if(err != LINK_PROT_ERROR) break;
	}
	if ( err < 0 ){
		m_error_message.sprintf("Failed to write", link_errno);
	}
	unlock_device();
	return check_error(err);

}

int Link::read_flash(int addr, void * buf, int nbyte){
	int err;
	lock_device();
	for(int tries = 0; tries < MAX_TRIES; tries++){
		err =  link_readflash(m_driver, addr, buf, nbyte);
		if(err != LINK_PROT_ERROR) break;
	}
	if ( err < 0 ){
		m_error_message.sprintf("Failed to read flash", link_errno);
	}
	unlock_device();
	return check_error(err);

}

int Link::get_is_executing(const var::ConstString & name){
	sys_taskattr_t task;
	int id;
	int err;
	int fd;

	if ( m_is_bootloader ){
		return -1;
	}

	if ( this->get_is_connected() == false ){
		return -1;
	}

	fd = open("/dev/sys", LINK_O_RDWR);
	if( fd < 0 ){
		this->m_error_message = "Failed to Open /dev/sys";
		return -1;
	}

	id = 0;

	do {
		task.is_enabled = 0;
		task.tid = id;
		memset(task.name, 0, 24);
		err = this->ioctl(fd, I_SYS_GETTASK, &task);

		if( err > 0 ){
			if( task.is_enabled != 0 ){
				if( var::String(task.name) == name ){
					close(fd);
					return task.pid;
				}
			}
		}
		id++;
	} while( err != -1 );

	close(fd);

	return -1;

}

int Link::write_flash(int addr, const void * buf, int nbyte){
	int err;
	lock_device();
	for(int tries = 0; tries < MAX_TRIES; tries++){
		err =  link_writeflash(m_driver, addr, buf, nbyte);
		if(err != LINK_PROT_ERROR) break;
	}
	if ( err < 0 ){
		m_error_message.sprintf("Failed to write flash", link_errno);
	}
	unlock_device();
	return check_error(err);
}


int Link::lseek(int fd, int offset, int whence){
	int err;
	if ( m_is_bootloader ){
		return -1;
	}

	lock_device();
	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_lseek(m_driver, fd, offset, whence);
		if(err != LINK_PROT_ERROR) break;
	}
	if ( err < 0 ){
		m_error_message.sprintf("Failed to lseek", link_errno);
	}
	unlock_device();
	return check_error(err);

}


int Link::ioctl(int fd, int request, void * ctl){
	int err;
	if ( m_is_bootloader ){
		return -1;
	}
	lock_device();
	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_ioctl(m_driver, fd, request, ctl);
		if(err != LINK_PROT_ERROR) break;
	}

	if ( err < 0 ){
		m_error_message.sprintf("Failed to ioctl", link_errno);
	}
	unlock_device();
	return check_error(err);

}

int Link::disconnect(){
	lock_device();
	if ( m_driver->dev.handle != LINK_PHY_OPEN_ERROR ){
		link_disconnect(m_driver);

		if ( m_driver->dev.handle != LINK_PHY_OPEN_ERROR ){
			unlock_device(); //can't unlock the device if it has been destroyed
		}
	}

	if( File::default_driver() == driver() ){
		File::set_default_driver(0);
	}

	m_is_bootloader = false;
	m_status_message = "";
	m_error_message = "";
	m_stdout_fd = -1;
	m_stdin_fd = -1;
	return 0;

}

void Link::set_disconnected(){
	m_driver->dev.handle = LINK_PHY_OPEN_ERROR;
}

bool Link::is_connected() const {
	if( m_driver->dev.handle == LINK_PHY_OPEN_ERROR ){
		return false;
	}

	return true;
}


int Link::stat(const var::ConstString & path, struct link_stat * st){
	int err;
	if ( m_is_bootloader ){
		return -1;
	}
	lock_device();
	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_stat(m_driver, path.c_str(), st);
		if(err != LINK_PROT_ERROR) break;
	}
	unlock_device();
	if( err < 0 ){
		m_error_message.sprintf("Failed to Get Stat", link_errno);
	}
	return check_error(err);
}

int Link::get_time(struct tm * gt){
	int err;
	struct link_tm ltm;
	if ( m_is_bootloader ){
		return -1;
	}
	lock_device();
	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_gettime(m_driver, &ltm);
		if(err != LINK_PROT_ERROR) break;
	}
	unlock_device();
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
	int err;
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
		return -1;
	}
	lock_device();
	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_settime(m_driver, &ltm);
		if(err != LINK_PROT_ERROR) break;
	}
	unlock_device();
	if ( err < 0 ){
		m_error_message.sprintf("Failed to Set Time", link_errno);
	}
	return check_error(err);
}

//Operations on the device
int Link::mkdir(const var::ConstString & directory, link_mode_t mode){
	int err;
	if ( m_is_bootloader ){
		return -1;
	}
	lock_device();
	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_mkdir(m_driver, directory.c_str(), mode);
		if(err != LINK_PROT_ERROR) break;
	}
	unlock_device();
	if ( err < 0 ){
		m_error_message.sprintf("Failed to create %s (%d)", directory.c_str(), link_errno);
	}
	return check_error(err);
}

int Link::rmdir(const var::ConstString & directory){
	int err;
	if ( m_is_bootloader ){
		return -1;
	}
	lock_device();
	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_rmdir(m_driver, directory.c_str());
		if(err != LINK_PROT_ERROR) break;
	}
	unlock_device();
	if ( err < 0 ){
		m_error_message.sprintf("Failed to remove %s (%d)", directory.c_str(), link_errno);
	}
	return check_error(err);
}

var::Vector<var::String> Link::get_dir_list(const var::ConstString & directory){

	var::Vector<var::String> list;
	int dirp;
	struct link_dirent entry;
	struct link_dirent * result;

	if ( m_is_bootloader ){
		return list;
	}

	dirp = opendir(directory);

	if( dirp == 0 ){
		return list;
	}

	while( readdir_r(dirp, &entry, &result) == 0 ){
		list.push_back(var::String(entry.d_name));
	}

	if( closedir(dirp) < 0 ){
		m_error_message.sprintf("Failed to close dir (%d)", link_errno);
	}

	return list;
}

var::String Link::convert_permissions(link_mode_t mode){
	var::String ret;

	link_mode_t type;
	type = mode & LINK_S_IFMT;
	switch(type){
		case LINK_S_IFDIR:
			ret = "d";
			break;
		case LINK_S_IFCHR:
			ret = "c";
			break;
		case LINK_S_IFBLK:
			ret = "b";
			break;
		case LINK_S_IFLNK:
			ret = "l";
			break;
		case LINK_S_IFREG:
			ret = "-";
			break;
		default:
			ret = "x";
	}

	if( mode & LINK_S_IROTH ){
		ret += "r";
	} else {
		ret += "-";
	}

	if( mode & LINK_S_IWOTH ){
		ret += "w";
	} else {
		ret += "-";
	}

	if( mode & LINK_S_IXOTH ){
		ret += "x";
	} else {
		ret += "-";
	}

	if( mode & LINK_S_IRGRP ){
		ret += "r";
	} else {
		ret += "-";
	}

	if( mode & LINK_S_IWGRP ){
		ret += "w";
	} else {
		ret += "-";
	}

	if( mode & LINK_S_IXGRP ){
		ret += "x";
	} else {
		ret += "-";
	}

	if( mode & LINK_S_IRUSR ){
		ret += "r";
	} else {
		ret += "-";
	}

	if( mode & LINK_S_IWUSR ){
		ret += "w";
	} else {
		ret += "-";
	}

	if( mode & LINK_S_IXUSR ){
		ret += "x";
	} else {
		ret += "-";
	}

	return ret;

}


int Link::opendir(const var::String & directory){
	int err;
	if ( m_is_bootloader ){
		return 0;
	}
	lock_device();
	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_opendir(m_driver, directory.c_str());
		if(err != LINK_PROT_ERROR) break;
	}
	unlock_device();
	if ( err < 0 ){
		m_error_message.sprintf("Failed to open %s (%d)", directory.c_str(), link_errno);
	}
	return check_error(err);
}

int Link::readdir_r(int dirp, struct link_dirent * entry, struct link_dirent ** result){
	int err;
	if ( m_is_bootloader ){
		return -1;
	}
	lock_device();
	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_readdir_r(m_driver, dirp, entry, result);
		if(err != LINK_PROT_ERROR) break;
	}
	unlock_device();
	if ( err < 0 ){
		m_error_message.sprintf("Failed to read directory", link_errno);
		return -1;
	}
	return check_error(err);
}

int Link::closedir(int dirp){
	int err;
	if ( m_is_bootloader ){
		return -1;
	}
	lock_device();
	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_closedir(m_driver, dirp);
		if(err != LINK_PROT_ERROR) break;
	}
	unlock_device();
	if ( err < 0 ){
		m_error_message.sprintf("Failed to close directory (%d)", link_errno);
	}
	return check_error(err);
}

int Link::symlink(const var::ConstString & oldPath, const var::ConstString & newPath){
	int err;
	if ( m_is_bootloader ){
		return -1;
	}
	lock_device();
	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_symlink(m_driver, oldPath.c_str(), newPath.c_str());
		if(err != LINK_PROT_ERROR) break;
	}
	unlock_device();
	if ( err < 0 ){
		m_error_message.sprintf("Failed to create symlink %s (%d)", newPath.str(), link_errno);
	}
	return check_error(err);
}

int Link::unlink(const var::ConstString & filename){
	int err;
	if ( m_is_bootloader ){
		return -1;
	}
	lock_device();
	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_unlink(m_driver, filename.c_str());
		if(err != LINK_PROT_ERROR) break;
	}
	unlock_device();
	if ( err < 0 ){
		m_error_message.sprintf("Failed to remove %s (%d)", filename.str(), link_errno);
	}
	return check_error(err);
}

int Link::copy(const var::ConstString & src, const var::ConstString & dest, link_mode_t mode, bool is_to_device, const ProgressCallback * progress_callback){
	int err;
	int flags;
	int bytesRead;
	const int bufferSize = 512;
	char buffer[bufferSize];
	struct link_stat st;
	File host_file;
	File device_file(driver());

	if ( m_is_bootloader ){
		return -1;
	}

	err = 0;

	if ( is_to_device == true ){

		m_progress_max = 0;
		m_progress = 0;

		//Open the host file
		if( host_file.open(src, File::RDONLY) < 0 ){
			m_error_message.sprintf("Could not find file %s on host", src.str());
			return -1;
		}

		m_progress_max = host_file.size();

		var::String dest_file = dest;
		if( dest_file.substr(0, 4) == "/app" ){
			int result;
			var::String dest_name = File::name(dest);

			result = Appfs::create(dest_name, host_file, "/app", progress_callback, driver());
			if( result < 0 ){
				m_error_message.format("Failed to write file %s on device", dest.str());
			}

		} else {

			//Create the device file
			flags = LINK_O_TRUNC | LINK_O_CREAT | LINK_O_WRONLY; //The create new flag settings
			lock_device();
			if( device_file.create(dest, true, mode) < 0 ){
				m_error_message.sprintf("Failed to create file %s on device", dest.cstring(), link_errno);
				unlock_device();
				return -1;
			} else {

				m_error_message = "";
				int result = device_file.write(host_file, APPFS_PAGE_SIZE, host_file.size(), progress_callback);
				if( result < 0 ){

				}
				if( device_file.close() < 0 ){
					m_error_message.sprintf("Failed to close Link device file (%d)", link_errno);
					unlock_device();
					return -1;
				}
			}
		}
		unlock_device();

		return err;

	} else {


		if ( link_stat(m_driver, src.c_str(), &st) < 0 ){
			m_error_message = "Failed to get target file size";
			return -1;
		}

		m_progress = 0;
		m_progress_max = 0;
		//Copy the source file from the device to the host
		if( host_file.create(dest, true, mode) < 0 ){
			m_error_message.sprintf("Failed to open file %s on host", dest.c_str());
			return -1;
		}


		//Open the device file
		flags = LINK_O_RDONLY; //Read the file only
		lock_device();

		if( device_file.open(src, File::RDONLY) < 0 ){
			m_error_message.sprintf("Failed to open file %s on device (%d)", src.cstring(), link_errno);
			unlock_device();
			return -1;
		} else {
			m_progress_max = st.st_size;

			if( host_file.write(device_file, APPFS_PAGE_SIZE, device_file.size(), progress_callback) < 0 ){

			}
			host_file.close();

			if( device_file.close() < 0 ){
				m_error_message.sprintf("Failed to close file %s on device (%d)", src.cstring(), link_errno);
				unlock_device();
				return -1;
			}
		}

		unlock_device();
	}
	return 0;
}

int Link::run_app(const var::ConstString & path){
	int err;
	if ( m_is_bootloader ){
		return -1;
	}

	if( path.length() >= LINK_PATH_ARG_MAX-1 ){
		m_error_message = "Path argument exceeds max";
		return -1;
	}

	lock_device();
	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_exec(m_driver, path.c_str());
		if(err != LINK_PROT_ERROR) break;
	}
	unlock_device();
	if ( err < 0 ){
		if ( err == LINK_TRANSFER_ERR ){
			m_error_message = "Connection Failed";
			this->disconnect();
			return -2;
		} else {
			m_error_message.format("Failed to run program: %s (%d, %d)", path.str(), err, link_errno);
			return -1;
		}
	}
	return err;
}

int Link::format(const var::ConstString & path){
	int err;
	if ( m_is_bootloader ){
		return -1;
	}
	m_error_message = "";
	//Format the filesystem
	lock_device();
	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_mkfs(m_driver, path.c_str());
		if(err != LINK_PROT_ERROR) break;
	}
	unlock_device();
	if ( err < 0 ){
		m_error_message.sprintf("Failed to Format Filesystem", link_errno);
	}
	return check_error(err);
}

int Link::kill_pid(int pid, int signo){
	int err;
	if ( m_is_bootloader ){
		return -1;
	}
	lock_device();
	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_kill_pid(m_driver, pid, signo);
		if(err != LINK_PROT_ERROR) break;
	}
	unlock_device();
	if ( err < 0 ){
		m_error_message.sprintf("Failed to kill process %d (%d)", pid, link_errno);
	}
	return check_error(err);
}

int Link::reset(){
	lock_device();
	link_reset(m_driver);

	unlock_device();
	m_driver->dev.handle = LINK_PHY_OPEN_ERROR;
	return 0;
}

int Link::reset_bootloader(){
	lock_device();
	link_resetbootloader(m_driver);
	unlock_device();
	m_driver->dev.handle = LINK_PHY_OPEN_ERROR;
	return 0;
}

int Link::rename(const var::ConstString & old_path, const var::ConstString & new_path){
	int err;
	if ( m_is_bootloader ){
		return -1;
	}
	lock_device();
	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_rename(m_driver, old_path.c_str(), new_path.c_str());
		if(err != LINK_PROT_ERROR) break;
	}
	unlock_device();
	if( err < 0 ){
		m_error_message.sprintf("Failed to rename file", link_errno);
	}
	return check_error(err);
}

int Link::chown(const var::ConstString & path, int owner, int group){
	int err;
	if ( m_is_bootloader ){
		return -1;
	}
	lock_device();
	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_chown(m_driver, path.c_str(), owner, group);
		if(err != LINK_PROT_ERROR) break;
	}
	unlock_device();
	if ( err < 0 ){
		m_error_message.sprintf("Failed to chown file", link_errno);
	}
	return check_error(err);
}

int Link::chmod(const var::ConstString & path, int mode){
	int err;
	if ( m_is_bootloader ){
		m_error_message = "Target is a bootloader";
		return -1;
	}
	lock_device();
	for(int tries = 0; tries < MAX_TRIES; tries++){
		err = link_chmod(m_driver, path.c_str(), mode);
		if(err != LINK_PROT_ERROR) break;
	}
	unlock_device();
	if ( err < 0 ){
		m_error_message.sprintf("Failed to chmod file", link_errno);
	}
	return check_error(err);

}

int Link::get_bootloader_attr(bootloader_attr_t & attr){
	int err;
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

int Link::update_os(const sys::File & image, bool verify, const ProgressCallback * progress_callback){
	int err;
	uint32_t loc;
	int bytesRead;
	char stackaddr[256];
	const int bufferSize = 1024;
	unsigned char buffer[bufferSize];
	unsigned char cmpBuffer[bufferSize];
	int i;
	bootloader_attr_t attr;
	uint32_t startAddr;
	uint32_t image_id;
	char tmp[256];

	if ( m_is_bootloader == false ){
		m_error_message = "Target is not a bootloader";
		return -1;
	}

	//now write the OS to the device using link_writeflash()
	m_progress_max = 0;
	m_progress = 0;

	if( image.seek(BOOTLOADER_HARDWARE_ID_OFFSET, LINK_SEEK_SET) < 0 ){
		return -1;
	}
	if( image.read(&image_id, sizeof(u32)) != sizeof(u32) ){
		return -1;
	}

	image.seek(0, LINK_SEEK_SET);

	m_progress_max = image.size();

	err = get_bootloader_attr(attr);
	if( err < 0 ){
		m_error_message = "Failed to read attributes";
		return check_error(err);
	}

	startAddr = attr.startaddr;
	loc = startAddr;

	if( (image_id & ~0x01) != (attr.hardware_id & ~0x01) ){
		err = -1;
		m_error_message.format(tmp,
									  "Kernel Image ID (0x%X) does not match Bootloader ID (0x%X)",
									  image_id,
									  attr.hardware_id);
		return check_error(err);
	}

	lock_device();
	if( progress_callback ){ progress_callback->update(0, 100); }
	//first erase the flash
	err = link_eraseflash(m_driver);

	if ( err < 0 ){
		unlock_device();
		if( progress_callback ){ progress_callback->update(0, 0); }
		m_error_message = "Failed to erase flash";
		return check_error(err);
	}

	m_error_message = "";
	m_status_message = "Writing OS to Target...";

	while( (bytesRead = image.read(buffer, bufferSize)) > 0 ){

		if( loc == startAddr ){
			//we want to write the first 256 bytes last because the bootloader checks this for a valid image
			memcpy(stackaddr, buffer, 256);

			//for now write 0xff to the first 256 bytes telling the bootloader the image isn't valid yet
			memset(buffer, 0xFF, 256);
		}

		if ( (err = link_writeflash(m_driver, loc, buffer, bytesRead)) != bytesRead ){
			m_error_message.sprintf("Failed to write to link flash", link_errno);
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

		if ( verify == true ){

			image.seek(0, LINK_SEEK_SET);
			loc = startAddr;
			m_progress = 0;

			m_status_message = "Verifying...";


			while( (bytesRead = image.read(buffer, bufferSize)) > 0 ){

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
						unlock_device();
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

		if( image_id != attr.hardware_id ){
			//if the LSb of the image_id doesn't match the bootloader HW ID, this will correct it
			memcpy(stackaddr + BOOTLOADER_HARDWARE_ID_OFFSET, &attr.hardware_id, sizeof(u32));
		}

		//write the stack address
		if( (err = link_writeflash(m_driver, startAddr, stackaddr, 256)) != 256 ){
			if( progress_callback ){ progress_callback->update(0,0); }
			m_error_message.sprintf("Failed to write stack addr", err);
			return -1;
		}


		if( verify == true ){
			//verify the stack address
			if( (err = link_readflash(m_driver, startAddr, buffer, 256)) != 256 ){
				m_error_message.sprintf("Failed to write stack addr", err);
				if( progress_callback ){ progress_callback->update(0,0); }
				unlock_device();
				return -1;
			}

			if( memcmp(buffer, stackaddr, 256) != 0 ){
				link_eraseflash(m_driver);
				m_error_message = "Failed to verify stack address";
				if( progress_callback ){ progress_callback->update(0,0); }
				unlock_device();
				return -1;
			}
		}

		m_status_message = "Finalizing...";

	}

	m_status_message = "Done";

	unlock_device();

	if( err < 0 ){
		link_eraseflash(m_driver);
	}

	if( progress_callback ){ progress_callback->update(0,0); }

	return check_error(err);
}

int Link::update_binary_install_options(const sys::File & file, const AppfsFileAttributes & attributes){
	var::Data image(sizeof(appfs_file_t));
	int result;

	if( (result = file.read(0, image)) != image.size() ){
		m_error_message.format("Failed to read from binary file (%d, %d)", result, file.error_number());
		return -1;
	}

	attributes.apply(image.to<appfs_file_t>());

	if( (result = file.write(0, image)) != image.size() ){
		m_error_message.format("Failed to write new attributes to binary file (%d, %d)", result, file.error_number());
		return -1;
	}

	return 0;
}

int Link::install_app(const sys::File & image_source, const var::ConstString & dest, const var::ConstString & name, const ProgressCallback * progress_callback){
	int bytes_read;
	int fd;
	appfs_installattr_t attr;
	int bytes_cumm;
	int bytes_total;
	var::String tmp_error;
	int loc_err;

	if( dest.find("/app") == 0 ){
		fd = this->open("/app/.install", LINK_O_WRONLY);
		if( fd < 0 ){
			m_error_message.format("Failed to open destination: %s (%d, %d)", dest.c_str(), link_errno, fd);
			return -1;
		}

		attr.loc = 0;

		bytes_total = image_source.size();
		bytes_cumm = 0;

		do {
			memset(attr.buffer, 0xFF, APPFS_PAGE_SIZE);
			//bytes_read = fread((char*)attr.buffer, 1, APPFS_PAGE_SIZE, source_file);
			bytes_read = bytes_total - bytes_cumm;
			if( bytes_read > APPFS_PAGE_SIZE ){
				bytes_read = APPFS_PAGE_SIZE;
			}

			image_source.read(attr.buffer, bytes_read);

			if( bytes_cumm == 0 ){
				//app_file = (link_appfs_file_t *)attr.buffer;
			}

			if( bytes_read > 0 ){
				attr.nbyte = bytes_read;
				bytes_cumm += attr.nbyte;
				if( (loc_err = ioctl(fd, I_APPFS_INSTALL, &attr)) < 0 ){
					if( link_errno == 5 ){ //EIO
						if( loc_err < -1 ){
							m_error_message.format("Failed to install: missing symbol on device near " F32D, loc_err+1);
						} else {
							m_error_message = "Failed to install: unknown symbol error";
						}
					} if( link_errno == 8 ){ //ENOEXEC
						m_error_message = "Failed to install: symbol table signature mismatch";
					} else {
						tmp_error = m_error_message;
						m_error_message = "Failed to install file on device (";
						m_error_message.append(tmp_error);
						m_error_message.append(")");
					}
					close(fd);
					return -1;
				}
				if( progress_callback ){ progress_callback->update(bytes_cumm,bytes_total); }
				attr.loc += APPFS_PAGE_SIZE;
			}
		} while( bytes_read == APPFS_PAGE_SIZE );

		if( close(fd) < 0 ){
			m_error_message.sprintf("Failed to close file on device", link_errno);
			return -1;
		}

		if( progress_callback ){ progress_callback->update(0,0); }
	} else {

		File f(driver());

		//copy the file to the destination directory
		var::String dest_str;
		dest_str << dest << "/" << name;

		if( f.create(dest_str) < 0 ){
			m_error_message.format("failed to create %s on target", dest_str.str());
			return -1;
		}

		if( f.write(image_source,
						APPFS_PAGE_SIZE,
						image_source.size(),
						progress_callback) < 0 ){
			return -1;
		}

		if( f.close() < 0 ){
			return -1;
		}

	}

	return 0;
}




