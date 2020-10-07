/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef RANDOM_HPP
#define RANDOM_HPP

#include "../api/CryptoObject.hpp"
#include "../arg/Argument.hpp"
#include "../var/Reference.hpp"
#include "../var/String.hpp"

namespace crypto {

class Random : public api::CryptoWorkObject {
public:

	~Random();

	int initialize();
	void finalize();

	int seed();
	int seed(var::View source_data);
	int randomize(var::View& destination_data);

	static var::String get_string(var::String::Length length);
	static var::Data get_data(u32 size);



private:
	void * m_context = nullptr;

};

}

#endif // RANDOM_HPP
