/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#ifndef SAPI_INET_SECURESOCKET_HPP_
#define SAPI_INET_SECURESOCKET_HPP_

#include <mbedtls_api.h>
#include "../sys/requests.h"
#include "Socket.hpp"

namespace inet {

typedef api::Api<mbedtls_api_t, MBEDTLS_API_REQUEST> SecureSocketApi;

class SecureSocket : public Socket {
public:
	SecureSocket();
	explicit SecureSocket(u32 ticket_lifetime);
	~SecureSocket();

	//already documented in inet::Socket
	virtual int create(const SocketAddress & address) override;

	//already documented in inet::Socket
	virtual int connect(const SocketAddress & address) override;

	//already documented in inet::Socket
	virtual int bind_and_listen(
			const SocketAddress & address,
			ListenBacklogCount backlog = ListenBacklogCount(4)
			) const override;


	/*! \details
	  *
	  */
	Socket accept(SocketAddress & address) const;

	//already documented in inet::Socket
	virtual int shutdown(int how = 0) const;

	using File::write;
	virtual int write(
			const void * buf,
			Size nbyte
			) const override;

	using File::read;
	virtual int read(
			void * buf,
			Size nbyte
			) const override;

	virtual int close() override;

	const var::Data & ticket() const { return m_ticket; }
	var::Data & ticket(){ return m_ticket; }
	static SecureSocketApi api(){ return m_api; }

private:

	/*! \cond */
	static SecureSocketApi m_api;
	void * m_context;
	u32 m_ticket_lifetime;
	var::Data m_ticket;
	/*! \endcond */

};

}

#endif // SAPI_INET_SECURESOCKET_HPP_
