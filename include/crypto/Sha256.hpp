/*! \file */ //Copyright 2011-2019 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_CRYPTO_SHA256_HPP_HPP_
#define SAPI_CRYPTO_SHA256_HPP_HPP_

#include "../api/CryptoObject.hpp"
#include "../var/Array.hpp"
#include "../var/String.hpp"

namespace crypto {

/*! \brief SHA256 Class
 * \details This class provides
 * access to system hardware or software
 * routines that calculate SHA256
 * hash digest values.
 *
 * The system must implmenent the
 * CRYPT_SHA256_API_REQUEST in kernel_request_api().
 *
 * ```
 * #include <sys/crypt.hpp>
 * #include <sys/var.hpp>
 *
 * Sha256 hash;
 * hash.initialize(); //call once per object
 * hash.start(); //start a new digest
 * Data some_data(128);
 * some_data.fill(0xaa);
 * hash << some_data; //update the digest with some data
 * hash.finalize();
 * printf("Hash is %s\n", hash.stringify());
 * ```
 *
 *
 */
class Sha256 : public api::CryptoWorkObject {
public:
	Sha256();
	~Sha256();

	int initialize();
	int finalize();

	int start();
	int update(const char * input, u32 len);
	int finish();

	Sha256 & operator << (const var::Data & a);
	Sha256 & operator << (const var::ConstString & a);
	Sha256 & operator << (const var::String & a);

	const var::Array<u8, 32> & output();
	var::String to_string();

private:
	var::Array<u8,32> m_output;
	void * m_context;
	bool m_is_finished;

	bool is_initialized() const { return m_context != 0; }


};

}

#endif // SAPI_CRYPT_SHA256_HPP_HPP_
