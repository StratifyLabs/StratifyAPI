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

AuthToken::AuthToken(const var::ConstString & token){
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
				var::ConstString(hex).to_unsigned_long(arg::NumberBase(16));
	}

}

#if defined __link
Auth::Auth(link_transport_mdriver_t * driver){
	m_driver = driver;
	m_fd = set_error_number_if_error(::link_open(m_driver, "/dev/auth", O_RDWR));
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
