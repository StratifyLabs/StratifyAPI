#ifndef SOCKET_HPP
#define SOCKET_HPP

// \tg use __win32 rather than __WIN32
#ifdef __WIN32
#define _BSD_SOURCE
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#else
#include <sys/socket.h>
#include "../sys/File.hpp"
#endif

#include "../api/InetObject.hpp"
#include "../var/ConstString.hpp"


/* \tg
 *
 * We don't want to expose any Winsocket API's in any hpp files.
 *
 *
 */
#define WS_ERR_NET_SOCKET_FAILED                     -0x0042  /**< Failed to open a socket. */
#define WS_ERR_NET_CONNECT_FAILED                    -0x0044  /**< The connection to the given server / port failed. */
#define WS_ERR_NET_BIND_FAILED                       -0x0046  /**< Binding of the socket failed. */
#define WS_ERR_NET_LISTEN_FAILED                     -0x0048  /**< Could not listen on the socket. */
#define WS_ERR_NET_ACCEPT_FAILED                     -0x004A  /**< Could not accept the incoming connection. */
#define WS_ERR_NET_RECV_FAILED                       -0x004C  /**< Reading information from the socket failed. */
#define WS_ERR_NET_SEND_FAILED                       -0x004E  /**< Sending information through the socket failed. */
#define WS_OK                                         0x0000  /**< Operation successful. */
#define WS_NET_PROTO_TCP 0 /**< The TCP transport protocol */
#define WS_NET_PROTO_UDP 1 /**< The UDP transport protocol */



namespace inet {

class Socket;

class SocketAddress : public api::InetInfoObject {
public:

    /*
     * \tg
     *
     * This is where constsocketaddress() should go.
     *
     *
     */
    SocketAddress(){}


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




/*
 * \tg
 *
 * The interface for __win32 should be the same for everything
 * else. We shouldn't have to
 *
 */
#ifdef __win32
class Socket  {
public:
    Socket();


    /*
     * \tg Please look at the API documentation in all the other
     * hpp files. They are like this
     *
     * \details Describes what the method does.
     *
     * @param a Describes what a does
     *
     * These are complete sentences with additional details
     * about how to use the method. Notice we use \details
     * rather than \brief
     *
     * \code
     *
     * \endcode
     *
     *
     */

    /**
     * @brief constructsockaddr - constructs the sockaddr structure using the
     *                            host and port details
     * @param address - sockaddr structure that needs to be constructed.
     * @param host    - ip address of the endpoint.
     * @param port    - port number
     *
     * @return        - none
     */

    /*
     * \tg Constructing a SocketAddress should happen in the
     * SocketAddress class.  See comments above.
     *
     * For naming conventions, please see https://github.com/StratifyLabs/StratifyAPI
     *
     * Also, don't use `const char*` anywhere. Use `const var::ConstString &` instead.
     *
     */
    void constructsockaddr(SocketAddress& address, const char* ipaddr, int port);


    /**
     * \brief          Initializes WS2_32.dll.Stores socket address.
     *
     * \return         WS_OK if successful, or
     *                 WS_ERR_NET_SOCKET_FAILED
     */
    /*
     * \tg
     *
     * We shouldn't return WS_ERR_...
     *
     * Just return -1 if something fails. We need to
     * define posix error number values for windows. That
     * will go in the file include/api/ApiObject.hpp
     *
     * Then if the fucntion fails, we use set_error_number(EIO);
     * where EIO is replaced with something appropriate for the failure.
     *
     * The list should match: https://stratifylabs.co/StratifyOS/html/group___e_r_r_n_o.html
     *
     */

    int create(const SocketAddress & address);


    /**
     * @brief connect - connects to the server using the SocketAddress object passed to
     *                  create() function.
     *
     * @return        - WS_ERR_NET_CONNECT_FAILED if the connection failed.
     *                  int socket descriptor
     */
    int connect();

    int listen();
    int accept();


private:
    SocketAddress m_sockaddress;
};
#else
class Socket : public sys::File {
public:
    Socket();

    int init();

    /*! \details Opens a new socket. */
    int create(const SocketAddress & address);


    int connect();
    int listen();
    int accept();


private:

};
#endif

}

#endif // SOCKET_HPP
