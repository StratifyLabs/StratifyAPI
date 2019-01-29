/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SECURESOCKET_HP
#define SECURESOCKET_HP

#include <mbedtls_api.h>
#include "../sys/requests.h"
#include "Socket.hpp"

namespace inet {

typedef api::Api<mbedtls_api_t, MBEDTLS_API_REQUEST> SecureSocketApi;

class SecureSocket : public Socket {
public:
	SecureSocket();
	SecureSocket(u32 ticket_lifetime);

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

	const var::Data & ticket() const { return m_ticket; }
	var::Data & ticket(){ return m_ticket; }
	static SecureSocketApi api(){ return m_api; }

private:

	static SecureSocketApi m_api;
	void * m_context;
	u32 m_ticket_lifetime;
	var::Data m_ticket;

};

}

#endif // SECURESOCKET_HP
