#include "crypto/Random.hpp"

using namespace crypto;

Random::Random(){
	m_context = 0;
}

Random::~Random(){
	finalize();
}

int Random::initialize(){
	finalize();
	return random_api()->init(&m_context);
}

void Random::finalize(){
	random_api()->deinit(&m_context);
}

int Random::seed(
		arg::SourceData source_data
		){
	return random_api()->seed(
				m_context,
				source_data.argument().to_const_u8(),
				source_data.argument().size()
				);
}

int Random::random(
		arg::DestinationData destination_data
		){
	return random_api()->random(
				m_context,
				destination_data.argument().to_u8(),
				destination_data.argument().size()
				);
}
