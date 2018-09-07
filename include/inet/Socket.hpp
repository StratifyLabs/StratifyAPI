#ifndef SOCKET_HPP
#define SOCKET_HPP

#ifdef __win32
#define _BSD_SOURCE
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")

#define DEFAULT_PORT "27015"
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
    /**
     * \details constructor to build the sockaddr structure using the host and port details.
     *
     * @param address - sockaddr structure that needs to be constructed.
     * @param host    - ip address of the endpoint.
     * @param port    - port number
     *
     * @return        - none
     */
    SocketAddress(const var::ConstString & ipaddr, int port);
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
 * This object should contain everything needed
 * to create a socket so Socket::create becomes
 *
 * int Socket::create(const SocketAttributes & attributes);
 *
 *
 */
class SocketAttributes : public api::InetInfoObject {
public:

    SocketAttributes(){
        m_family = FAMILY_NONE;
        //init the rest
    }

    bool is_valid() const {
        //check other members for invalid settings
        return m_family != FAMILY_NONE;
    }

    enum family {
        FAMILY_NONE,
        FAMILY_INET
    };

    enum type {
        TYPE_NONE,
        TYPE_STREAM
    };

    enum protocol {
        PROTOCOL_TCP,
        PROTOCOL_UDP
    };

    //these may need to be Or'd so an anonymous enum is OK
    enum {
        FLAG_PASSIVE
    };

    //need functions to set these
    void set_family(enum family value){ m_family = value; }

    int flags() const { return m_flags; }
    int family() const { return m_family; }
    int protocol() const { return m_protocol; }
    int type() const { return m_type; }

    //allows read access
    const Ipv4Address & ipv4_address() const { return m_ipv4_address; }
    //allows write access
    Ipv4Address & ipv4_address(){ return m_ipv4_address; }

private:
    Ipv4Address m_ipv4_address;
    //family
    enum family m_family;
    //type
    int m_type;
    //protocol
    int m_protocol;
    //flags
    int m_flags;

};



/*
 * \tg
 *
 * The interface for __win32 should be the same for everything
 * else. We shouldn't have to
 *
 * \ck The Socket class is derfived from Sys::File which has an enum SOCKET.
 * So the SOCKET variable used in connect function was wrongly resolving to this enum
 * SOCKET and throwing an error. I had to move the class definition under the ifdef for
 * that reason. We should address that.
 *
 * \tg You should be able to do ::SOCKET to push the definition to
 * the global scope. If that doesn't work change the enum in File to FILE_SOCKET
 *
 * Also please look closely at sys::File and how it reads and writes data.
 * The Socket class should be able to read and write data in a very similar
 * manner (like using int File::write(const var::Data & data))
 *
 *
 */
#ifdef __win32
class Socket  {
public:
    Socket();
    ~Socket();


    /**
     * \details        Initializes WS2_32.dll.
     *
     * @return         WS_OK if successful, or
     *                 -1
     */
    int create();

    /**
     * \details        Initializes WS2_32.dll.Stores socket address.
     *
     * @params         address - instance of SocketAddress containing the host address and port
     *                 blocking - 1 to turn on blocking mode
     *                            0 to turn off blocking mode
     *                 proto    - Indicates the protocol TCP/UDP to be used for connection
     *
     * @return         WS_OK if successful, or
     *                 -1
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
     * \ck should I do this now? Yes :)
     *
     *
     * \tg Try implementing int create(const SocketAttributes & attributes) which
     * should call ::socket() and store the result in the m_socket member
     * variable. It should turn -1 if ::socket() fails.
     *
     */
    int create(const SocketAttributes & attributes);
    int create(const SocketAddress & address, bool blocking, int proto);


    /**
     * \details       connects to the server using the SocketAddress object passed to
     *                  create() function.
     *
     * @return        - -1 if the connection failed.
     *                  int socket descriptor if the connection was successful.
     */
    int connect();

    /**
     * \details       binds and listens to the port for which the socket is created
     *
     *
     * @return        - -1 if failed.
     *                  int socket descriptor if the connection was successful.
     */

    /*
     * \tg
     * This method should only listen on the socket. It shouldn't
     * bind() or create the socket. Just listen.  There should be
     * another method for bind().
     *
     * the create() method should be equivalent to calling socket()
     *
     *
     */
    int listen();

    /**
     * \details       connects to the server using the SocketAddress object passed to
     *                  create() function.
     *
     * @return        - -1 if failed.
     *                  int socket descriptor if the connection was successful.
     */
    int accept();

    /*
     * \tg
     * Please look at how the comments are done on all the other
     * modules and follow the same format. Use *! instead of ** when
     * starting comment blocks
     *
     * \details Sends data on the connected socket. Details should
     * start with a capital letter and end with a period.
     *
     *
     *
     *
     */

    /**
     * \details       sends data on the connected socket
     *
     * @param           socket - descriptor identifying the connecting socket.
     * @param           send_buffer - character buffer containing the data to be sent.
     * @param           buffer-length - length of the buffer
     *
     * @return        - -1 if failed.
     *                  number of bytes sent if successful.
     */

    /*
     *
     * \tg
     * `SOCKET socket` should be a member variable called m_socket.
     *
     * So this API would then be int send(const var::ConstString & send_buffer);
     *
     * You don't need to pass socket because you will just use m_socket.
     *
     * You don't need to pass the lenght because you can use send_buffer.length()
     *
     * So the code would look like this:
     *
     * \code
     * Socket s;
     * s.connect();
     * s.send("Hello World");
     *
     * \endcode
     *
     *
     *
     */
    int send(SOCKET socket, var::ConstString send_buffer, int buffer_length);

    /**
     * \details       receives data on the connected socket.
     *
     * @param           socket - descriptor identifying the connecting socket.
     * @param           receive_buffer - character buffer containing the data received.
     * @param           buffer-length - length of the buffer
     *
     * @return        - -1 if failed.
     *                  number of bytes sent if successful.
     */
    int receive(SOCKET socket, var::ConstString receive_buffer, int buffer_length);

    /**
     * \details       closes all the sockets and cleans ws2_32 lib
     *
     */

    /*
     * \tgil
     *
     * This should just be close();
     *
     * Example:
     *
     * Socket s;
     * s.connect();
     * s.close();
     *
     *
     */
    void close_socket();

private:
    SocketAddress m_sockaddress;


    /*
     * \tgil
     *
     * You should only have one member socket (m_socket) per
     * Socket object.
     *
     * If you need a socket for listening and one for connecting,
     * you would instantiate two sockets.
     *
     * Socket listen_socket;
     * Socket connect_socket;
     *
     * listen_socket.listen();
     * connect_socket.connect();
     *
     * connect_socket.send("Hello");
     *
     *
     *
     *
     */
    SOCKET m_listen_socket;
    SOCKET m_connect_socket;
};
#else

/*
 * \tg The socket API above will eventually
 * be the same as the one below.  The only
 * place we should really need to say
 * #if defined __win32 is for declaring
 * the ::SOCKET member variable m_socket.
 *
 *
 */
class Socket : public sys::File {
public:
    Socket();
    ~Socket();

    int init();

    /*! \details Opens a new socket. */
    int create(const SocketAttributes & attributes); //calls ::socket() - calls File::set_fileno();

    int connect(); //calls ::connect()
    int listen(); //calls ::listen()
    Socket accept(); //calls ::accept(m_socket); and returns a new Socket
    int bind(); //calls ::bind()

    //read and write are inherited from sys::File and will work without any additional methods

private:

#if defined __win32
    ::SOCKET m_socket;
#endif

};
#endif

}

#endif // SOCKET_HPP
