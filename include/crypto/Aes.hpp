#ifndef SAPI_CRYPTO_AES_HPP_
#define SAPI_CRYPTO_AES_HPP_

#include "../api/CryptoObject.hpp"
#include "../arg/Argument.hpp"
#include "../var/Reference.hpp"
#include "../var/Data.hpp"

namespace crypto {

class Aes : public api::CryptoWorkObject {
public:

	using SourceReference = var::Reference::Source;
	using DestinationReference = var::Reference::Destination;

	using SourceCipherData = arg::Argument<const var::Reference&, struct AesSourceCipherDataTag >;
	using DestinationCipherData = arg::Argument<var::Reference&, struct AesDestinationCipherDataTag >;

	using SourcePlainData = arg::Argument<const var::Reference&, struct AesSourcePlainDataTag >;
	using DestinationPlainData = arg::Argument<var::Reference&, struct AesDestinationPlainDataTag >;

	Aes();
	~Aes();

	int initialize();
	int finalize();

	Aes & set_key(
			const var::Reference & key
			);

	Aes & set_initialization_vector(
			const var::Reference & value
			);

	const var::Data & initialization_vector() const {
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
	void * m_context = 0;
	var::Data m_initialization_vector;
};

}

#endif // SAPI_CRYPTO_AES_HPP_
