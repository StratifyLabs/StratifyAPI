#ifndef RANDOM_HPP
#define RANDOM_HPP

#include "../api/CryptoObject.hpp"
#include "../arg/Argument.hpp"
#include "../var/Reference.hpp"

namespace crypto {

class Random : public api::CryptoWorkObject {
public:

	using SourceReference = var::Reference::Source;
	using DestinationReference = var::Reference::Destination;


	Random();
	~Random();

	int initialize();
	void finalize();

	int seed(
			const var::Reference & source_data
			);

	int random(
			const var::Reference & destination_data
			);


private:
	void * m_context = 0;

};

}

#endif // RANDOM_HPP
