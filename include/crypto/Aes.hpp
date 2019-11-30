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


	Aes();
	~Aes();

	int initialize();
	int finalize();

	Aes & set_key(
			const var::Data & key
			);

	Aes & set_initialization_vector(
			const var::Data & value
			){
		m_initialization_vector	= value;
		return *this;
	}

	const var::Data & initialization_vector() const {
		return m_initialization_vector;
	}

	int encrypt_ecb(
			SourceReference source_data,
			DestinationReference destination_data
			);

	int decrypt_ecb(
			SourceReference source_data,
			DestinationReference destination_data
			);

	int encrypt_cbc(
			SourceReference source_data,
			DestinationReference destination_data
			);

	int decrypt_cbc(
			SourceReference source_data,
			DestinationReference destination_data
			);


	int encrypt_ctr(
			SourceReference source_data,
			DestinationReference destination_data
			);

	int decrypt_ctr(
			SourceReference source_data,
			DestinationReference destination_data
			);

private:
	void * m_context = 0;
	var::Data m_initialization_vector;
};

}

#endif // SAPI_CRYPTO_AES_HPP_
