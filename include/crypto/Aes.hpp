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

class AesOptions {
	API_ACCESS_COMPOUND(AesOptions,var::View,plain_data);
	API_ACCESS_COMPOUND(AesOptions,var::View,cipher_data);
};

class Aes : public api::CryptoWorkObject {
public:

	using SourceCipherData = arg::Argument<const var::View&, struct AesSourceCipherDataTag >;
	using DestinationCipherData = arg::Argument<var::View&, struct AesDestinationCipherDataTag >;

	using SourcePlainData = arg::Argument<const var::View&, struct AesSourcePlainDataTag >;
	using DestinationPlainData = arg::Argument<var::View&, struct AesDestinationPlainDataTag >;

	~Aes();


	int initialize();
	int finalize();

	bool is_valid() const {
		return m_context != nullptr;
	}

	Aes & set_key(
			var::View key
			);

	Aes & set_initialization_vector(
			var::View value
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

	class CbcCipherData {
		API_AC(CbcCipherData,var::Data,data);
		API_AC(CbcCipherData,InitializationVector,initialization_vector);
	};

	static CbcCipherData get_cbc_cipher_data(const var::Blob & key, const var::Blob & source);
	static var::Data get_plain_data(const var::Blob & key, const CbcCipherData & source);

private:
	void * m_context = nullptr;
	InitializationVector m_initialization_vector;
};

}

#endif // SAPI_CRYPTO_AES_HPP_
