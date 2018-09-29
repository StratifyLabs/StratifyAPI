#ifndef SAPI_SOCKET_HPP
#define SAPI_SOCKET_HPP

#include "SocketAttributes.hpp"
#include "../sys/File.hpp"

namespace inet {

class Socket : public sys::File {
public:
	Socket();
	~Socket();

	/*!
	  * \details Creates a new socket.
	  *
	  * @params attributes The Socket Attributes
	  *
	  * @return Zero on success
	  */
	int create(const SocketAttributes & attributes);

	/*!
	  * \details Connects to the server using the SocketAddress
	  * object passed to create() method.
	  * @return Less than zero on error or zero on success
	  */
	int connect();

	/*!
	  * \details Binds to the port for which the socket was created.
	  * @return Less than zero on error and zero on success
	  */
	int bind();

	/*!
	  * \details Listens on the port specified during create().
	  *
	  * @param backlog Limit the number of outstanding connections in the socket's listen queue
	  * @return Less than zero on error or zero on success
	  */
	int listen(int backlog = 4);

	/*!
	  * \details Accepts a socket connection on a socket that is listening.
	  *
	  * @return A valid Socket if the operation is successful.
	  */
	Socket accept();

	/*! \details Shuts down the socket.
	 *
	 * @param how Use READWRITE, READONLY, or WRITEONLY to disable the specified operations
	 *
	 * \code
	 * Socket s;
	 * s.shutdown(); //disables further send/receive operations
	 * s.shutdown(Socket::READWRITE); //disables further send/receive operations
	 * s.shutdown(Socket::WRITEONLY); //disables further send operations
	 * s.shutdown(Socket::READONLY); //disables further receive operations
	 * \endcode
	 *
	 * @return Zero on success
	 */
	int shutdown(int how = 0);

	//already documented in sys::File
	int write(const void * buf, int nbyte);

	//already documented in sys::File
	int read(void * buf, int nbyte);

	//already documented in sys::File
	int close();

	bool is_valid() const;

private:
	SocketAttributes m_socketattributes;


	/*!
	  * \details Initializes the system socket API.
	  *
	  * @return Zero on success
	  */
	int init();

	static int decode_socket_return(int value);

#if defined __win32
	::SOCKET m_socket;
#else
	//socket on all other platforms is a file handler
	int m_socket;
#endif

	static bool m_is_initialized;
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

#endif // SAPI_SOCKET_HPP
