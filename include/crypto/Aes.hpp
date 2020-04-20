/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef SAPI_CRYPTO_AES_HPP_
#define SAPI_CRYPTO_AES_HPP_

#include "../api/CryptoObject.hpp"
#include "../arg/Argument.hpp"
#include "../var/Reference.hpp"
#include "../var/Data.hpp"

namespace crypto {

using InitializationVector = var::Array<u8,16>;
using Iv = InitializationVector;

class Aes : public api::CryptoWorkObject {
public:

	using SourceCipherData = arg::Argument<const var::Reference&, struct AesSourceCipherDataTag >;
	using DestinationCipherData = arg::Argument<var::Reference&, struct AesDestinationCipherDataTag >;

	using SourcePlainData = arg::Argument<const var::Reference&, struct AesSourcePlainDataTag >;
	using DestinationPlainData = arg::Argument<var::Reference&, struct AesDestinationPlainDataTag >;

	~Aes();


	int initialize();
	int finalize();

	bool is_valid() const {
		return m_context != nullptr;
	}

	Aes & set_key(
			const var::Reference & key
			);

	Aes & set_initialization_vector(
			const var::Reference & value
			);

	const InitializationVector & initialization_vector() const {
		return m_initialization_vector;
	}

	int encrypt_ecb(
			SourcePlainData source_data,
			DestinationCipherData destination_data
			);

	int decrypt_ecb(
			SourceCipherData source_data,
			DestinationPlainData destination_data
			);

	int encrypt_cbc(
			SourcePlainData source_data,
			DestinationCipherData destination_data
			);

	int decrypt_cbc(
			SourceCipherData source_data,
			DestinationPlainData destination_data
			);


	int encrypt_ctr(
			SourcePlainData source_data,
			DestinationCipherData destination_data
			);

	int decrypt_ctr(
			SourceCipherData source_data,
			DestinationPlainData destination_data
			);

private:
	void * m_context = nullptr;
	InitializationVector m_initialization_vector;
};

}

#endif // SAPI_CRYPTO_AES_HPP_
