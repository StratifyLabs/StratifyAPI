#ifndef AUTH_HPP
#define AUTH_HPP

#include <sos/link.h>
#include <sos/dev/auth.h>
#include "../api/SysObject.hpp"
#include "../var/String.hpp"
#include "../fs/File.hpp"

namespace sys {


class AuthToken : public api::InfoObject {
public:
	AuthToken(){ memset(&m_auth_token, 0, sizeof(auth_token_t)); }
	AuthToken(const auth_token_t & auth_token){
		m_auth_token = auth_token;
	}

	AuthToken(const var::String & token);

	bool is_valid() const {
		for(u8 i=0; i < sizeof(auth_token_t); i++){
			if( m_auth_token.data[i] != 0 ){ return true; }
		}
		return false;
	}

	bool operator == (const AuthToken & a) const {
		return memcmp(&a.m_auth_token, &m_auth_token, sizeof(auth_token_t)) == 0;
	}

	var::String to_string() const {
		var::String result;
		for(u8 i=0; i < sizeof(auth_token_t); i++){
			result << var::String().format("%02x", m_auth_token.data[i]);
		}
		return result;
	}

	const auth_token_t & auth_token() const { return m_auth_token; }

private:
	auth_token_t m_auth_token;

};

/*! \brief Auth Class
 * \details This class is used to authenticate
 * the calling thread to the OS and gain root privileges.
 *
 * This is similar to using `sudo`.
 *
 *
 * ```msc
 * Caller->System: Sends 128-bit Random Number
 * Note right of System: Appends 128-bits
 * System->Caller: 256-bit Random Number
 * Caller->System: SHA256(Secret Key, Random Number)
 * Note right of System: Validates token
 * System->Caller: SHA256(Random Number, Secret Key)
 * ```
 *
 */
class Auth : public api::WorkObject {
public:
	Auth(
			SAPI_LINK_DRIVER_NULLPTR
			);

	~Auth();

	AuthToken start(const AuthToken & token);
	AuthToken finish(const AuthToken & token);

	bool is_valid() const {	return m_fd >= 0; }


private:
	int m_fd;
	int open();
	void close();

#if defined __link
	link_transport_mdriver_t * m_driver;
#endif
};

}



#endif // AUTH_HPP
