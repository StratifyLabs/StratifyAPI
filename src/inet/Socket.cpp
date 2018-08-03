#include <sys/socket.h>
#include <unistd.h>

#include "inet/Socket.hpp"

using namespace inet;

Socket::Socket(){
    m_fileno = -1;
}

int Socket::create(){

    return -1;
}

int Socket::listen(){
    return -1;
}

int Socket::accept(){
    return -1;
}

int Socket::connect(){
    return -1;
}

int Socket::read(void * buffer, int nbyte){
    return ::read(fileno(), buffer, nbyte);
}

int Socket::write(const void * buffer, int nbyte){
    return ::write(fileno(), buffer, nbyte);
}

int Socket::close(){
    return ::close(fileno());
}
