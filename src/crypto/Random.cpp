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
		const var::Reference & source_data
		){
	return random_api()->seed(
				m_context,
				source_data.to_const_u8(),
				source_data.size()
				);
}

int Random::random(
		const var::Reference & destination_data
		){
	return random_api()->random(
				m_context,
				destination_data.to_u8(),
				destination_data.size()
				);
}
