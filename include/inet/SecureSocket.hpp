#ifndef SECURESOCKET_HP
#define SECURESOCKET_HP

#include "Socket.hpp"

namespace inet {

class SecureSocket : public Socket {
public:
	SecureSocket();

	//already documented in inet::Socket
	virtual int create(const SocketAddress & address);

	//already documented in inet::Socket
	virtual int connect(const SocketAddress & address);


	//already documented in inet::Socket
	virtual int bind_and_listen(const SocketAddress & address, int backlog = 4) const;


	/*! \details
	  *
	  */
	Socket accept(SocketAddress & address) const;

	//already documented in inet::Socket
	virtual int shutdown(int how = 0) const;

	using File::write;
	virtual int write(const void * buf, int nbyte) const;

	using File::read;
	virtual int read(void * buf, int nbyte) const;

	virtual int close();

private:

};

}

#endif // SECURESOCKET_HP
