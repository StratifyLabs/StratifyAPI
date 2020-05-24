/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef RANDOM_HPP
#define RANDOM_HPP

#include "../api/CryptoObject.hpp"
#include "../arg/Argument.hpp"
#include "../var/Reference.hpp"

namespace crypto {

class Random : public api::CryptoWorkObject {
public:

	~Random();

	int initialize();
	void finalize();

	int seed(const var::Reference & source_data);
	int randomize(var::Reference& destination_data);


private:
	void * m_context = nullptr;

};

}

#endif // RANDOM_HPP
