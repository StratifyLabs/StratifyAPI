/*! \file */ //Copyright 2011-2019 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_CRYPT_SHA256_HPP_HPP_
#define SAPI_CRYPT_SHA256_HPP_HPP_

#include "../api/CryptObject.hpp"
#include "../var/Array.hpp"
#include "../var/String.hpp"

namespace crypt {

class Sha256 : public api::CryptWorkObject {
public:
	Sha256();
	~Sha256();
	int initialize();
	int finalize();
	int start();
	int update(const char * input, u32 len);
	int finish();

	Sha256 & operator << ( const var::Data & a);
	Sha256 & operator << ( const var::ConstString & a);
	Sha256 & operator << ( const var::String & a);

	const var::Array<u8, 32> & output();
	var::String stringify();

private:
	var::Array<u8,32> m_output;
	void * m_context;
	bool m_is_finished;

	bool is_initialized() const { return m_context != 0; }


};

}

#endif // SAPI_CRYPT_SHA256_HPP_HPP_
