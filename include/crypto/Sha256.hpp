/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#ifndef SAPI_CRYPTO_SHA256_HPP_
#define SAPI_CRYPTO_SHA256_HPP_

#include "../api/CryptoObject.hpp"
#include "../var/Array.hpp"
#include "../var/Reference.hpp"
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

	using SourceBuffer = var::View::SourceBuffer;
	using Size = var::View::Size;
	using SourceFile = fs::File::Source;
	using PageSize = fs::File::PageSize;

	Sha256();
	~Sha256();

	int initialize();
	int finalize();

	int start();

	int update(
			SourceBuffer input,
			Size size
			);

	int update(
			var::View data
			){

		return update(
					SourceBuffer(data.to_const_void()),
					Size(data.size())
					);
	}

	int finish();

	static var::String calculate(
			const fs::File & file,
			PageSize page_size = PageSize(CRYPTO_SHA256_DEFAULT_PAGE_SIZE)
			);

	static var::String calculate(
			const var::String & file_path,
			PageSize page_size = PageSize(CRYPTO_SHA256_DEFAULT_PAGE_SIZE)
			);

	Sha256 & operator << (var::View a);

	const var::Array<u8, 32> & output();
	var::String to_string();
	static u32 length(){
		return 32;
	}

private:
	var::Array<u8,32> m_output;
	void * m_context;
	bool m_is_finished;

	bool is_initialized() const { return m_context != 0; }


};

}

#endif // SAPI_CRYPTO_SHA256_HPP_
