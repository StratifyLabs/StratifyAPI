#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "../api/InetObject.hpp"

namespace inet {


class SocketAddress : public api::InetInfoObject {
public:

};

class Socket : public api::InetWorkObject {
public:
    Socket();

    int create();

    int listen();
    int accept();
    int connect();

    int fileno() const { return m_fileno; }
    virtual int read(void * buffer, int nbyte);
    virtual int write(const void * buffer, int nbyte);
    virtual int close();

private:
    int m_fileno;

};

}

#endif // SOCKET_HPP
