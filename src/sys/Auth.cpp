/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include <fcntl.h>
#include <unistd.h>

#include "sys/Auth.hpp"

#if defined __link
#define OPEN() ::link_open(m_driver, "/dev/auth", LINK_O_RDWR)
#define CLOSE() ::link_close(m_driver, m_fd)
#define IOCTL(REQUEST, CTL) ::link_ioctl(m_driver, m_fd, REQUEST, CTL)
#else
#define OPEN() ::open("/dev/auth", O_RDWR)
#define CLOSE() ::close(m_fd)
#define IOCTL(REQUEST, CTL) ::ioctl(m_fd, REQUEST, CTL)
#endif

using namespace sys;

AuthToken::AuthToken(const var::String & token){
	memset(&m_auth_token, 0, sizeof(m_auth_token));

	if( token.length() != 2*sizeof(m_auth_token) ){
		return;
	}

	char hex[3];
	hex[2] = 0;

	for(u32 i=0; i < sizeof(m_auth_token); i++){
		hex[0] = token.at(i*2);
		hex[1] = token.at(i*2+1);
		m_auth_token.data[i] =
				var::String(hex).to_unsigned_long(var::String::base_16);
	}

}

#if defined __link
Auth::Auth(
		SAPI_LINK_DRIVER
		){
	m_driver = link_driver.argument();
	m_fd = set_error_number_if_error(
				::link_open(
					m_driver,
					"/dev/auth",
					O_RDWR)
				);
}
#else
Auth::Auth(){
	m_fd = -1;
}
#endif

Auth::~Auth(){
	close();
}

int Auth::open(){
	if( m_fd < 0 ){
		m_fd = set_error_number_if_error(OPEN());
	}
	return m_fd;
}

void Auth::close(){
	if( m_fd < 0 ){ return; }
	CLOSE();
}


AuthToken Auth::start(const AuthToken & token){
	auth_token_t result;
	memcpy(&result, &token.auth_token(), sizeof(auth_token_t));
	if( open() < 0 ){ return AuthToken(); }
	if( set_error_number_if_error( IOCTL(I_AUTH_START, &result)) < 0 ){ return AuthToken(); }
	return AuthToken(result);
}

AuthToken Auth::finish(const AuthToken & token){
	auth_token_t result;
	memcpy(&result, &token.auth_token(), sizeof(auth_token_t));
	if( set_error_number_if_error( IOCTL(I_AUTH_FINISH, &result)) < 0 ){ return AuthToken(); }
	close();
	return AuthToken(result);
}
