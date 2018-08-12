#include <sys/socket.h>
#include <unistd.h>

#include "inet/Socket.hpp"

using namespace inet;

Socket::Socket(){}

int Socket::create(const SocketAddress & address){

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


