/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

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
	SecureSocket(u32 ticket_lifetime);
	~SecureSocket();

	//already documented in inet::Socket
	virtual int create(const arg::SourceSocketAddress address);

	//already documented in inet::Socket
	virtual int connect(const arg::SourceSocketAddress address);

	//already documented in inet::Socket
	virtual int bind_and_listen(const arg::SourceSocketAddress address,
										 const arg::ListenBacklogCount backlog = arg::ListenBacklogCount(4)
			) const;


	/*! \details
	  *
	  */
	Socket accept(arg::DestinationSocketAddress address) const;

	//already documented in inet::Socket
	virtual int shutdown(int how = 0) const;

	using File::write;
	virtual int write(
			const arg::SourceBuffer buf,
			const arg::Size nbyte
			) const;

	using File::read;
	virtual int read(
			arg::DestinationBuffer buf,
			const arg::Size nbyte
			) const;

	virtual int close();

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
