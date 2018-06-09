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

using namespace sys;

//#include "link_flags.h"

/*
static int atoh(string s){
    stringstream ss;
    int x;
    ss << hex << s;
    ss >> x;
    return x;
}
 */


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
    m_serial_number = "";
    m_driver = &m_default_driver;
    link_load_default_driver(m_driver);
    memset(&m_sys_info, 0, sizeof(m_sys_info));
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


int Link::connect(const var::String & path, const var::String & sn, bool is_legacy){
    int err;

    reset_progress();

    if ( m_driver->dev.handle == LINK_PHY_OPEN_ERROR ){

        m_driver->dev.handle = m_driver->dev.open(path.c_str(), 0);
        if( m_driver->dev.handle == LINK_PHY_OPEN_ERROR ){
            m_error_message = "Failed to Connect to Device";
            return -1;
        }
        m_serial_number = sn;
        m_path = path;

    } else {
        m_error_message.sprintf("Already Connected (%d)", 1);
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
        return -1;
    }


    if( m_is_bootloader == false ){
        link_get_sys_info(driver(), &m_sys_info);
    } else {
        bootloader_attr_t boot_attr;
        get_bootloader_attr(boot_attr);
        memset(&m_sys_info, 0, sizeof(m_sys_info));
        strcpy(m_sys_info.name, "bootloader");
        m_sys_info.hardware_id = boot_attr.hardware_id;
        memcpy(&m_sys_info.serial, boot_attr.serialno, sizeof(mcu_sn_t));
    }

    if( File::default_driver() == 0 ){
        File::set_default_driver( driver() );
    }

    return 0;
}


int Link::open(const var::String & file, int flags, link_mode_t mode){
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

int Link::get_is_executing(const var::String & name){
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
    m_serial_number = "";
    m_stdout_fd = -1;
    m_stdin_fd = -1;
    memset(&m_sys_info, 0, sizeof(m_sys_info));
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


int Link::stat(const var::String & path, struct link_stat * st){
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
int Link::mkdir(const var::String & directory, link_mode_t mode){
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

int Link::rmdir(const var::String & directory){
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

var::Vector<var::String> Link::get_dir_list(const var::String & directory){

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
        list.push_back(entry.d_name);
    }

    if( closedir(dirp) < 0 ){
        m_error_message.sprintf("Failed to close dir (%d)", link_errno);
    }

    list.set_transfer_ownership();
    return list;
}

var::String Link::convert_permissions(link_mode_t mode){
    var::String ret;

    link_mode_t type;
    type = mode & LINK_S_IFMT;
    switch(type){
    case LINK_S_IFDIR:
        ret = 'd';
        break;
    case LINK_S_IFCHR:
        ret = 'c';
        break;
    case LINK_S_IFBLK:
        ret = 'b';
        break;
    case LINK_S_IFLNK:
        ret = 'l';
        break;
    case LINK_S_IFREG:
        ret = '-';
        break;
    default:
        ret = 'x';
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

int Link::symlink(const var::String & oldPath, const var::String & newPath){
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

int Link::unlink(const var::String & filename){
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

int Link::copy(const var::String & src, const var::String & dest, link_mode_t mode, bool toDevice, bool (*update)(void*, int, int), void * context){
    FILE * hostFile;
    int err;
    int deviceFile;
    int flags;
    int bytesRead;
    const int bufferSize = 512;
    char buffer[bufferSize];
    struct link_stat st;

    if ( m_is_bootloader ){
        return -1;
    }

    err = 0;

    if ( toDevice == true ){

        m_progress_max = 0;
        m_progress = 0;

        //Open the host file
        hostFile = fopen(src.c_str(), "rb");
        if ( hostFile == NULL ){
            m_error_message.sprintf("Could not find file %s on host", src.str());
            return -1;
        }

        //Create the device file
        flags = LINK_O_TRUNC | LINK_O_CREAT | LINK_O_WRONLY; //The create new flag settings
        lock_device();
        deviceFile = link_open(m_driver, dest.c_str(), flags, mode);

        fseek(hostFile, 0, SEEK_END);
        m_progress_max = ftell(hostFile);
        rewind(hostFile);

        m_error_message = "";

        if ( deviceFile >= 0 ){
            while( (bytesRead = fread(buffer, 1, bufferSize, hostFile)) > 0 ){
                if ( (err = link_write(m_driver, deviceFile, buffer, bytesRead)) != bytesRead ){
                    m_error_message.sprintf("Failed to write to Link device file", link_errno);
                    if ( err > 0 ){
                        err = -1;
                    }
                    break;
                } else {
                    m_progress += bytesRead;
                    if( update != 0 ){
                        if( update(context, m_progress, m_progress_max) == true ){
                            //update progress and check for abort
                            break;
                        }
                    }
                    err = 0;
                }
            }
        } else {
            unlock_device();
            fclose(hostFile);

            if ( deviceFile == LINK_TRANSFER_ERR ){
                m_error_message = "Connection Failed";
                this->disconnect();
                return -2;
            }

            m_error_message.sprintf("Failed to create file %s on Link device (%d)", dest.str(), link_errno);
            return -1;
        }

        fclose(hostFile);

        if ( err == LINK_TRANSFER_ERR ){
            unlock_device();
            m_error_message = "Connection Failed";
            this->disconnect();
            return -2;
        }

        if ( link_close(m_driver, deviceFile) ){
            m_error_message.sprintf("Failed to close Link device file (%d)", link_errno);
            unlock_device();
            return -1;
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
        hostFile = fopen(dest.c_str(), "wb");
        if ( hostFile == NULL ){
            m_error_message.sprintf("Failed to open file %s on host", dest.c_str());
            return -1;
        }


        //Open the device file
        flags = LINK_O_RDONLY; //Read the file only
        lock_device();
        deviceFile = link_open(m_driver, src.c_str(), flags, 0);

        if ( deviceFile > 0 ){
            m_progress_max = st.st_size;

            while( (bytesRead = link_read(m_driver, deviceFile, buffer, bufferSize)) > 0 ){
                fwrite(buffer, 1, bytesRead, hostFile);
                m_progress += bytesRead;
                if( update != 0 ){
                    if( update(context, m_progress, m_progress_max) == true ){
                        //update progress and check for abort

                        break;
                    }
                }
                if ( bytesRead < bufferSize ){
                    break;
                }
            }
        } else {
            if ( deviceFile == LINK_TRANSFER_ERR ){
                m_error_message = "Connection Failed";
                unlock_device();
                this->disconnect();
                return -2;
            } else {
                m_error_message.sprintf("Failed to open file %s on Link device (%d)", src.str(), link_errno);
                fclose(hostFile);
                unlock_device();
                return -1;
            }
        }

        fclose(hostFile);

        if ( (err = link_close(m_driver, deviceFile)) ){
            if ( err == LINK_TRANSFER_ERR ){
                m_error_message = "Connection Failed";
                unlock_device();
                this->disconnect();
                return -2;
            } else {
                m_error_message.sprintf("Failed to close Link file", link_errno);
                unlock_device();
                return -1;
            }
        }
        unlock_device();
    }
    return 0;
}

int Link::run_app(const var::String & path){
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
            m_error_message.sprintf("Failed to run program: %s (%d)", path.str(), link_errno);
            return -1;
        }
    }
    return err;
}

int Link::format(const var::String & path){
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

int Link::rename(const var::String & old_path, const var::String & new_path){
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

int Link::chown(const var::String & path, int owner, int group){
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

int Link::chmod(const var::String & path, int mode){
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

int Link::update_os(const var::String & path, bool verify, bool (*update)(void*,int,int), void * context){
    int err;
    uint32_t loc;
    int bytesRead;
    FILE * hostFile;
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

    //Open the host file
    hostFile = fopen(path.c_str(), "rb");
    if ( hostFile == NULL ){
        m_error_message.sprintf("Could not find file %s on host", path.c_str());
        return -1;
    }

    fseek(hostFile, BOOTLOADER_HARDWARE_ID_OFFSET, SEEK_SET);
    fread(&image_id, 1, sizeof(u32), hostFile);

    fseek(hostFile, 0, SEEK_END);
    m_progress_max = ftell(hostFile);
    rewind(hostFile);


    err = get_bootloader_attr(attr);
    //err = link_ioctl(d, LINK_BOOTLOADER_FILDES, I_BOOTLOADER_GETATTR, &attr);
    if( err < 0 ){
        m_error_message = "Failed to read attributes";
        fclose(hostFile);
        return check_error(err);
    }

    startAddr = attr.startaddr;
    loc = startAddr;

    if( (image_id & ~0x01) != (attr.hardware_id & ~0x01) ){
        err = -1;
        sprintf(tmp,
                "Kernel Image ID (0x%X) does not match Bootloader ID (0x%X)",
                image_id,
                attr.hardware_id);
        m_error_message.sprintf(tmp, link_errno);
        fclose(hostFile);
        return check_error(err);
    }

    lock_device();
    //first erase the flash
    err = link_eraseflash(m_driver);

    if ( err < 0 ){
        unlock_device();
        fclose(hostFile);
        m_error_message = "Failed to erase flash";
        return check_error(err);
    }

    m_error_message = "";
    m_status_message = "Writing OS to Target...";

    while( (bytesRead = fread(buffer, 1, bufferSize, hostFile)) > 0 ){

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
        if( update(context, m_progress, m_progress_max) == true ){
            //update progress and check for abort
            break;
        }
        err = 0;
    }

    if ( err == 0 ){

        if ( verify == true ){

            rewind(hostFile);
            loc = startAddr;
            m_progress = 0;

            m_status_message = "Verifying...";


            while( (bytesRead = fread(buffer, 1, bufferSize, hostFile)) > 0 ){

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
                        fclose(hostFile);
                        unlock_device();
                        return -1;
                    }

                    loc += bytesRead;
                    m_progress += bytesRead;
                    if( update(context, m_progress, m_progress_max) == true ){
                        //update progress and check for abort
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
            m_error_message.sprintf("Failed to write stack addr", err);
            return -1;
        }


        if( verify == true ){
            //verify the stack address
            if( (err = link_readflash(m_driver, startAddr, buffer, 256)) != 256 ){
                m_error_message.sprintf("Failed to write stack addr", err);
                fclose(hostFile);
                unlock_device();
                return -1;
            }

            if( memcmp(buffer, stackaddr, 256) != 0 ){
                link_eraseflash(m_driver);
                m_error_message = "Failed to verify stack address";
                fclose(hostFile);
                unlock_device();
                return -1;
            }
        }

        m_status_message = "Finalizing...";

    }

    m_status_message = "Done";

    fclose(hostFile);
    unlock_device();

    if( err < 0 ){
        link_eraseflash(m_driver);
    }

    return check_error(err);
}

int Link::update_binary_install_options(const var::String & path, const var::String & name, const var::String & id, int version, bool startup, bool run_in_ram, int ram_size){
    appfs_file_t appfs_file;
    FILE * binary_file;

    binary_file = fopen(path.c_str(), "rb+");
    if( binary_file ==  0 ){
        m_error_message.sprintf("Failed to open %s (%d)", path.c_str(), link_errno);
        return -1;
    }


    if( fread((char*)&appfs_file, sizeof(appfs_file), 1, binary_file) <= 0 ){
        fclose(binary_file);
        m_error_message = "Failed to read app filesystem data";
        return -1;
    }

    memset(appfs_file.hdr.name, 0, LINK_NAME_MAX);
    strncpy(appfs_file.hdr.name, name.c_str(), LINK_NAME_MAX - 2);
    memset(appfs_file.hdr.id, 0, LINK_NAME_MAX);
    strncpy(appfs_file.hdr.id, id.c_str(), LINK_NAME_MAX - 2);
    appfs_file.hdr.mode = 0777;
    appfs_file.hdr.version = version;

    if ( startup == true ){
        appfs_file.exec.o_flags |= APPFS_FLAG_IS_STARTUP;
    } else {
        appfs_file.exec.o_flags &= ~APPFS_FLAG_IS_STARTUP;
    }

    if ( run_in_ram == true ){
        appfs_file.exec.o_flags &= ~(APPFS_FLAG_IS_FLASH);
    } else {
        appfs_file.exec.o_flags |= (APPFS_FLAG_IS_FLASH);
    }

    if( ram_size >= 4096 ){
        appfs_file.exec.ram_size = ram_size;
    }

    if( appfs_file.exec.ram_size < 4096 ){
        appfs_file.exec.ram_size = 4096;
    }

    fseek(binary_file, 0, SEEK_SET);

    if( fwrite((char*)&appfs_file, sizeof(appfs_file), 1, binary_file) <= 0 ){
        fclose(binary_file);
        m_error_message = "Failed to write updated binary info";
        return -1;
    }

    fclose(binary_file);
    return 0;
}

int Link::install_app(const var::String & source, const var::String & dest, const var::String & name, bool (*update)(void*,int,int), void * context){
    FILE * source_file;
    ssize_t source_size;
    int bytes_read;
    int fd;
    appfs_installattr_t attr;
    int bytes_cumm;
    int bytes_total;
    var::String tmp_error;
    int loc_err;

    if ( m_is_bootloader ){
        return -1;
    }

    //link_appfs_file_t * app_file;

    source_file = fopen(source.c_str(), "rb");
    if( source_file == 0 ){
        m_status_message.sprintf("Failed to open file: %s", source.c_str());
        return -1;
    }

    fseek(source_file, 0, SEEK_END);
    source_size = ftell(source_file);
    fseek(source_file, 0, SEEK_SET);

    if( dest.substr(0, strlen("/app")) == var::String("/app")){
        fd = open("/app/.install", LINK_O_WRONLY);
        if( fd < 0 ){
            fclose(source_file);
            m_error_message.sprintf("Failed to open destination: %s", dest.c_str());
            return -1;
        }

        attr.loc = 0;

        bytes_total = source_size;
        bytes_cumm = 0;

        do {
            memset(attr.buffer, 0xFF, APPFS_PAGE_SIZE);
            bytes_read = fread((char*)attr.buffer, 1, APPFS_PAGE_SIZE, source_file);

            if( bytes_cumm == 0 ){
                //app_file = (link_appfs_file_t *)attr.buffer;
            }

            if( bytes_read > 0 ){
                attr.nbyte = bytes_read;
                bytes_cumm += attr.nbyte;
                if( (loc_err = ioctl(fd, I_APPFS_INSTALL, &attr)) < 0 ){
                    if( link_errno == 5 ){ //EIO
                        if( loc_err < -1 ){
                            m_error_message.sprintf("Failed to install: missing symbol on device near", loc_err+1);
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
                    fclose(source_file);
                    return -1;
                }
                if( update ){ update(context, bytes_cumm, bytes_total); }

                attr.loc += APPFS_PAGE_SIZE;
            }
        } while( bytes_read == APPFS_PAGE_SIZE );

        fclose(source_file);

        if( close(fd) < 0 ){
            m_error_message.sprintf("Failed to close file on device", link_errno);
            return -1;
        }


        if( update ){ update(context, 0, 0); }

    } else {
        //copy the file to the destination directory
        var::String dest_str;
        dest_str.sprintf("%s/%s", dest.str(), name.str());
        if( copy_file_to_device(source, dest_str, 0666, update, context) < 0 ){
            m_error_message.sprintf("Failed to copy %s to %s (%d)", source.str(), dest.str(), link_errno);
            return -1;
        }

        if( update ){ update(context, 0, 0); }
    }

    return 0;
}




