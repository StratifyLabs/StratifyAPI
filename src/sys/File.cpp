//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include <cstdio>
#include <cstring>
#include "sys/File.hpp"
#include "chrono/Timer.hpp"
using namespace sys;

#if defined __link

link_transport_mdriver_t * File::m_default_driver = 0;

File::File(link_transport_mdriver_t * driver){
    m_fd = -1; //The file is not open
	 m_driver = driver;
}

#else
File::File() {
    m_fd = -1; //The file is not open
}
#endif

int File::open(const var::ConstString & name, int flags){
	 return open(name, flags, 0);
}

#if !defined __link
int File::remove(const var::ConstString & path){
    return ::remove(path.str());
}
#else
int File::remove(const var::ConstString & name, link_transport_mdriver_t * driver){
    return link_unlink(driver, name.str());
}
#endif


#if !defined __link
int File::copy(var::String & source_path, var::String & dest_path){
    File source;
    File dest;
    return copy(source, dest, source_path, dest_path);
}
#else
int File::copy(const var::String & source_path, const var::String & dest_path, link_transport_mdriver_t * driver){
    File source;
    File dest;
    source.set_driver(driver);
    dest.set_driver(driver);
    return copy(source, dest, source_path, dest_path);
}
#endif

#if !defined __link
int File::rename(const var::ConstString & old_path, const var::ConstString & new_path){
    return ::rename(old_path.str(), new_path.str());
}
#else
int File::rename(const var::ConstString & old_path, const var::ConstString & new_path, link_transport_mdriver_t * driver){
    return link_rename(driver, old_path.str(), new_path.str());
}
#endif


int File::copy(File & source, File & dest, const var::ConstString & source_path, const var::ConstString & dest_path){
    if( source.open(source_path.str(), File::RDONLY) < 0 ){
        return -1;
    }

    if( dest.create(dest_path.str()) < 0 ){
        source.close();
        return -1;
    }

    char buffer[128];
    int bytes;
    while( (bytes = source.read(buffer, 128)) > 0 ){
        dest.write(buffer, bytes);
    }

    source.close();
    dest.close();
    return -1;
}


int File::open(const var::ConstString & name, int access, int perms){
    if( m_fd != -1 ){
        close(); //close and re-open
    }

#if defined __link
    m_fd = link_open(driver(), name.str(), access, perms);
#else
    m_fd = ::open(name.str(), access, perms);
#endif

    return set_error_number_if_error(m_fd);

}

int File::create(const var::ConstString & name, bool overwrite, int perms){
    int access = LINK_O_RDWR | LINK_O_CREAT;
    if( overwrite ){
        access |= LINK_O_TRUNC;
    }
    return open(name, access, perms);
}

u32 File::size() const {
	 u32 loc = seek(0, SEEK_CUR); //get current cursor
	 u32 size = seek(0, SEEK_END); //seek to the end
	 seek(loc, SEEK_SET); //restore the cursor
	 return size;
}

#if defined __link
int File::stat(const var::ConstString & name, struct link_stat * st, link_transport_mdriver_t * driver){
    return link_stat(driver, name.str(), st);
}
#else
int File::stat(const var::ConstString & name, struct stat * st){
    return ::stat(name.str(), st);
}
#endif

#if !defined __link
u32 File::size(const var::ConstString & name){
    struct stat st;
    if( stat(name.str(), &st) < 0 ){
        return (s32)-1;
    }
    return st.st_size;
}
#else
u32 File::size(const var::ConstString & name, link_transport_mdriver_t * driver){
    struct link_stat st;
    if( stat(name.str(), &st, driver) < 0 ){
        return (s32)-1;
    }
    return st.st_size;
}

#endif

int File::read(int loc, void * buf, int nbyte) const {
    int result = seek(loc);
    if( result < 0 ){ return result; }
    return read(buf, nbyte);
}

int File::write(int loc, const void * buf, int nbyte) const {
    int result = seek(loc);
    if( result < 0 ){ return result; }
    return write(buf, nbyte);
}

int File::readline(char * buf, int nbyte, int timeout, char term) const {
    int t;
    int bytes_recv;
    char c;
    t = 0;
    bytes_recv = 0;
    do {
        if( read(&c, 1) == 1 ){
            *buf = c;
            buf++;
            bytes_recv++;
            if( c == term ){
                return bytes_recv;
            }
        } else {
            t++;
#if !defined __link
            chrono::Timer::wait_milliseconds(1);
#endif
        }
    } while( (bytes_recv < nbyte) && (t < timeout) );

    return bytes_recv;
}

int File::close(){
    int ret = 0;
    if( m_fd >= 0 ){
#if defined __link
        ret = link_close(driver(), m_fd);
#else
        ret = ::close(m_fd);
#endif
        m_fd = -1;
        if( ret < 0 ){
            set_error_number_to_errno();
        }
    }
    return ret;
}

int File::read(void * buf, int nbyte) const {
#if defined __link
    return set_error_number_if_error( link_read(driver(), m_fd, buf, nbyte) );
#else
    return set_error_number_if_error( ::read(m_fd, buf, nbyte) );
#endif
}

int File::write(const void * buf, int nbyte) const {
#if defined __link
    return set_error_number_if_error( link_write(driver(), m_fd, buf, nbyte) );
#else
    return set_error_number_if_error( ::write(m_fd, buf, nbyte) );
#endif
}

int File::seek(int loc, int whence) const {
#if defined __link
    return set_error_number_if_error( link_lseek(driver(), m_fd, loc, whence) );
#else
    return set_error_number_if_error( ::lseek(m_fd, loc, whence) );
#endif
}

int File::fileno() const {
    return m_fd;
}

int File::loc() const {
    return seek(0, CURRENT);
}

int File::flags() const{
#if defined __link
    return -1;
#else
    if( fileno() < 0 ){
        return -1;
    }
    return _global_impure_ptr->procmem_base->open_file[m_fd].flags;
#endif
}

char * File::gets(char * s, int n, char term) const {
    int t;
    int ret;
    int i;
    char buffer[GETS_BUFFER_SIZE];

    if( n < 1 ){
        return 0;
    }

    s[0] = 0;
    t = 0;
    do {
        ret = read(buffer, GETS_BUFFER_SIZE);
        for(i=0; i < ret; i++){
            s[t] = buffer[i];
            s[t+1] = 0;
            if( s[t] == '\n'){
                break;
            }
            if( t == (n-1) ){
                break;
            }
            t++;
        }
    } while( (t < (n-1)) && (s[t] != term) && (ret > 0) );

    if( t == 0 ){
        return 0;
    }

    if( ret > 0 ){
        seek(i - ret + 1, CURRENT);
    }

    return s;

}

var::String File::gets(char term) const {
    var::String ret;
    gets(ret, term);
    return ret;
}

char * File::gets(var::String & s, char term) const {
    int ret;
    char c;
    s.clear();
    do {
        ret = read(&c, 1);
        if( ret > 0 ){
            s.append(c);
        } else {
            return 0;
        }
    } while(c != term);
    return s.cdata();
}


var::ConstString File::name(const var::ConstString & path){
    int len;
    int i;
    len = path.length();
    for(i = len; i >= 0; i--){
        if( path.at(i) == '/' ){
            return path.str() + i + 1;
        }
    }
	 return path;
}

#if !defined __link
int File::access(const var::ConstString & path, int o_access){
    return ::access(path.str(), o_access);
}
#endif


var::ConstString File::suffix(const var::ConstString & path){
    int len;
    int i;
    len = path.length();
    for(i = len; i >= 0; i--){
        if( path.at(i) == '.' ){
            return path.str() + i + 1;
        }
    }
    return 0;
}


int File::ioctl(int req, void * arg) const {
#if defined __link
    return set_error_number_if_error( link_ioctl(driver(), m_fd, req, arg) );
#else
    return set_error_number_if_error( ::ioctl(m_fd, req, arg) );
#endif
}

