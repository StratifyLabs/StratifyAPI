#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/socket.h>

#include "../api/InetObject.hpp"
#include "../sys/File.hpp"
#include "../var/ConstString.hpp"

namespace inet {

class Socket;

class SocketAddress : public api::InetInfoObject {
public:

    SocketAddress();


protected:
    friend class Socket;
    struct sockaddr m_address;

};

/*!
 * \brief IP Version 4 Address
 * \details The IP Version 4 Address class holds
 * an IPv4 address. The address is stored in
 * network order (big-endian).
 *
 */
class Ipv4Address : public SocketAddress {
public:

    Ipv4Address(){ m_address = 0; }

    Ipv4Address(u8 a, u8 b, u8 c, u8 d){
        set_address(a,b,c,d);
    }


    /*! \details Sets the address based on the four values given.
     *
     *
     * \code
     * set_address(192,168,1,1);
     * \endcode
     *
     */
    void set_address(u8 a, u8 b, u8 c, u8 d){ m_address = a | (b<<8) | (c<<16) | (d<<24); }

    /*! \details Sets the address based on the string provided.
     *
     * @param address Address a string, e.g., "192.186.1.1"
     *
     */
    void set_address(const var::ConstString & address);

private:
    u32 m_address;
};






class Socket : public sys::File {
public:
    Socket();

    /*! \details Opens a new socket. */
    int create(const SocketAddress & address);


    int connect();
    int listen();
    int accept();


private:

};

}

#endif // SOCKET_HPP
