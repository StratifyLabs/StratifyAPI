#ifndef RANDOM_HPP
#define RANDOM_HPP

#include "../api/CryptoObject.hpp"
#include "../arg/Argument.hpp"
#include "../var/Data.hpp"

namespace crypto {

class Random : public api::CryptoWorkObject {
public:
	Random();
	~Random();

	int initialize();
	void finalize();

	int seed(
			arg::SourceData source_data
			);

	int random(
			arg::DestinationData destination_data
			);


private:
	void * m_context = 0;

};

}

#endif // RANDOM_HPP
