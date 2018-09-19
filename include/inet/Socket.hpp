#ifndef SOCKET_HPP
#define SOCKET_HPP

//#if defined __win32

//#define _BSD_SOURCE
//#include <winsock2.h>
//#include <ws2tcpip.h>

//#define DEFAULT_PORT "27015"
//#endif
#include "SocketAttributes.hpp"
#include "../sys/File.hpp"

#define DEFAULT_PORT "27015"

/* \tg
 *
 * We don't want to expose any Winsocket API's in any hpp files.
 *
 *
 */

#define WS_OK                                           0x0000  /**< Operation successful. */
#define WS_ERROR                                        -0x0001 /**< Operation not successful. */


namespace inet {

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
 * \ck : INVALID_SOCKET is a macro which defines as SOCKET ~(0) in winsock2.h. So
 * I have to change the enum in File to FILE_SOCKET.
 *
 * Also please look closely at sys::File and how it reads and writes data.
 * The Socket class should be able to read and write data in a very similar
 * manner (like using int File::write(const var::Data & data))
 *
 *
 */
class Socket : public sys::File {
public:
    Socket();
    ~Socket();

    /*!
     * \details        Initializes WS2_32.dll.Stores socket address.
     *
     * @params         address - instance of SocketAddress containing the host address and port
     *                 blocking - 1 to turn on blocking mode
     *                            0 to turn off blocking mode
     *                 proto    - Indicates the protocol TCP/UDP to be used for connection
     *
     * @return         WS_OK if successful, or
     *                 WS_ERROR if not successful
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

    /*!
     * \details       Connects to the server using the SocketAddress object passed to
     *                  create() function.
     *
     * @return         WS_OK if successful, or
     *                 WS_ERROR if not successful
     */
    int connect();

    /*!
     * \details       Binds to the port for which the socket is created
     *
     *
     * @return         WS_OK if successful, or
     *                 WS_ERROR if not successful
     */
    int bind();

    /*!
     * \details       Listens to the port for which the socket is created
     *
     *
     * @return         WS_OK if successful, or
     *                 WS_ERROR if not successful
     */
    int listen();

    /*!
     * \details       Connects to the server using the SocketAttributes object passed to
     *                  create() function.
     *
     * @return         WS_OK if successful, or
     *                 WS_ERROR if not successful
     */
    int accept();

    /*!
     * \details       Sends data on the connected socket
     *
     * @param           socket - descriptor identifying the socket to send the data to.
     * @param           send_buffer - character buffer containing the data to be sent.
     *
     * @return        - -1 if failed.
     *                  number of bytes sent if successful.
     */

	 int send(var::ConstString send_buffer);

    /*!
     * \details       Receives data on the connected socket.
     *
     * @param           socket - descriptor identifying the connecting socket.
     * @param           receive_buffer - character buffer containing the data received.
     * @param           buffer-length - length of the buffer
     *
     * @return        - -1 if failed.
     *                  number of bytes sent if successful.
     */
	 int receive(var::ConstString receive_buffer, int buffer_length);

    /*!
     * \details       Closes all the sockets and cleans ws2_32 lib
     *
     * @return         WS_OK if successful, or
     *                 WS_ERROR if not successful
     */
    int close();

private:
    SocketAttributes m_socketattributes;


    /*!
     * \details        Initializes WS2_32.dll.
     *
     * @return         WS_OK if successful, or
     *                 WS_ERROR if not successful
     */
    int init();

#if defined __win32
    ::SOCKET m_socket;
#endif
};


/*
 * \tg The socket API above will eventually
 * be the same as the one below.  The only
 * place we should really need to say
 * #if defined __win32 is for declaring
 * the ::SOCKET member variable m_socket.
 *
 *
 */
//class Socket : public sys::File {
//public:
//    Socket();
//    ~Socket();

//    int init();

//    /*! \details Opens a new socket. */
//    int create(const SocketAttributes & attributes); //calls ::socket() - calls File::set_fileno();

//    int connect(); //calls ::connect()
//    int listen(); //calls ::listen()
//    Socket accept(); //calls ::accept(m_socket); and returns a new Socket
//    int bind(); //calls ::bind()

//    //read and write are inherited from sys::File and will work without any additional methods

//private:

//#if defined __win32
//    ::SOCKET m_socket;
//#endif

//};
//#endif

}

#endif // SOCKET_HPP
