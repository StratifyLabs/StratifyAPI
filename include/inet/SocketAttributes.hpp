#ifndef SOCKETATTRIBUTES_HPP
#define SOCKETATTRIBUTES_HPP

#if defined __win32

#define _BSD_SOURCE
#include <winsock2.h>
#include <ws2tcpip.h>

//#define DEFAULT_PORT "27015"
#else

#include <sys/socket.h>
#endif

//#include "../sys/File.hpp"
#include "../api/InetObject.hpp"
#include "../var/ConstString.hpp"

/* \tg
 *
 * We don't want to expose any Winsocket API's in any hpp files.
 *
 *
 */

namespace inet {

class Socket;

class SocketAddress : public api::InetInfoObject {
public:
    /*!
     * \details Constructor to build the sockaddr structure using the host and port details.
     *
     * @param ipaddress     - ip address of the endpoint.
     * @param port          - port number
     *
     */
    SocketAddress(u32& ipaddress, int port);
    /*!
     * \details Copy Constructor
     *
     * @param socketaddress     - SocketAddress structure to be copied from.
     *
     */
    SocketAddress(const SocketAddress& socketaddress);
    /*!
     * \details Constructor to set the sockaddr structure to 0.
     */
    SocketAddress();
    /*!
     * \details Sets the ipaddress of the sockaddr structure
     *
     * @param ipaddress     - ip address of the endpoint.
     *
     */
    void set_socketipaddress(u32 ipaddress);
    /*!
     * \details Sets the port number of the sockaddr structure
     * @param port          - port number
     *
     */
    void set_port(u32 port);


protected:
    friend class Socket;
    struct sockaddr m_sockaddr;

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

    Ipv4Address(){}

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
    void set_address(u8 a, u8 b, u8 c, u8 d){
        m_address = a | (b<<8) | (c<<16) | (d<<24);
        set_socketipaddress(m_address);
    }

    /*! \details Sets the address based on the string provided.
     *
     * @param address Address a string, e.g., "192.186.1.1"
     *
     */
    void set_address(const var::ConstString & address);

private:
    u32 m_address;
};

/*
 * \tg
 *
 * This object should contain everything needed
 * to create a socket so Socket::create becomes
 *
 * int Socket::create(const SocketAttributes & attributes);
 *
 *
 */
class SocketAttributes : public api::InetInfoObject {
public:
    enum family {
        FAMILY_NONE,
        FAMILY_INET
    };

    enum type {
        TYPE_NONE,
        TYPE_STREAM,
    };

    enum protocol {
        PROTOCOL_TCP,
        PROTOCOL_UDP
    };

    //these may need to be Or'd so an anonymous enum is OK
    enum flag{
        FLAG_PASSIVE=0x01,
        FLAG_BLOCKING=0x02
    };

    SocketAttributes();

    bool is_valid() const;

    //need functions to set these
    void set_family(enum family value){ m_family = value; }
    void set_type(enum type value){ m_type = value; }
    void set_protocol(enum protocol value){ m_protocol = value; }
    void set_flag(int value){ m_flag = value; }
    void set_ipaddress(u8 a, u8 b,u8 c,u8 d) {m_ipv4_address.set_address(a,b,c,d);}
    void set_ipaddress(var::ConstString& address) {m_ipv4_address.set_address(address);}
    void set_ipaddressandport(var::ConstString& address,u32 port) {
        set_ipaddress(address);
        set_port(port);
    }
    void set_ipaddressandport(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint32_t port) {
        set_ipaddress(a,b,c,d);
        set_port(port);
    }
    void set_port(u32 port) { m_ipv4_address.set_port(port);}

    int flags() const { return m_flag; }
    enum family family() const { return m_family; }
    enum protocol protocol() const { return m_protocol; }
    enum type type() const { return m_type; }

    //allows read access
    const Ipv4Address & ipv4_address() const { return m_ipv4_address; }
    //allows write access
    Ipv4Address & ipv4_address(){ return m_ipv4_address; }

private:
    Ipv4Address m_ipv4_address;
    u32 m_port;
    //family
    enum family m_family;
    //type
    enum type m_type;
    //protocol
    enum protocol m_protocol;
    //flags
    u32 m_flag;

};
}

#endif // SOCKETATRRIBUTES_HPP
