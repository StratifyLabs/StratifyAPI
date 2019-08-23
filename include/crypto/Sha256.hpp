/*! \file */ //Copyright 2011-2019 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_CRYPTO_SHA256_HPP_HPP_
#define SAPI_CRYPTO_SHA256_HPP_HPP_

#include "../api/CryptoObject.hpp"
#include "../var/Array.hpp"
#include "../var/String.hpp"
#include "../var/Data.hpp"
#include "../fs/File.hpp"

#if defined __link
#define CRYPTO_SHA256_DEFAULT_PAGE_SIZE 4096
#else
#define CRYPTO_SHA256_DEFAULT_PAGE_SIZE 256
#endif


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
 * some_data.fill((u8)0xaa);
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

	int update(
			const arg::SourceBuffer input,
			const arg::Size size
			);

	int update(
			const arg::SourceData data
			){
		return update(arg::SourceBuffer(data.argument().to_const_void()),
						  arg::Size(data.argument().size()));
	}

	int finish();

	static var::String calculate(
			const arg::SourceFile file,
			arg::PageSize page_size = arg::PageSize(CRYPTO_SHA256_DEFAULT_PAGE_SIZE)
			);

	static var::String calculate(
			const arg::SourceFilePath file_path,
			arg::PageSize page_size = arg::PageSize(CRYPTO_SHA256_DEFAULT_PAGE_SIZE)
			);

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
