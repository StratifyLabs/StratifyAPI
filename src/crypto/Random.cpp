/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "var/Data.hpp"
#include "crypto/Random.hpp"
#include "chrono/Clock.hpp"

using namespace crypto;


Random::~Random(){
	finalize();
}

int Random::initialize(){
	if( random_api().is_valid() == false ){
		return set_error_number_if_error(api::error_code_crypto_missing_api);
	}
	finalize();
	int result = 0;
	if( random_api()->init(&m_context) < 0 ){
		result = api::error_code_crypto_operation_failed;
	}
	seed(var::Data());
	return set_error_number_if_error(result);
}

void Random::finalize(){
	if( m_context != nullptr ){
		random_api()->deinit(&m_context);
		m_context = nullptr;
	}
}

int Random::seed(){
	var::Vector<u32> list(8);
	for(u32 & item: list){
		item = chrono::Clock::get_time().nanoseconds();
		chrono::wait(chrono::Microseconds(item % 1000));
	}

	return seed(list);
}

int Random::seed(
		const var::Reference & source_data
		){
	int result;
	if( (result = random_api()->seed(
				m_context,
				source_data.to_const_u8(),
				source_data.size()
				)) < 0 ){
		result = api::error_code_crypto_operation_failed;
	}
	return set_error_number_if_error(result);
}

int Random::randomize(
		var::Reference & destination_data
		){
	int result;
	if( (result = random_api()->random(
				m_context,
				destination_data.to_u8(),
				destination_data.size())
				) < 0 ){
		result = api::error_code_crypto_operation_failed;
	}
	return set_error_number_if_error(result);
}

var::String Random::get_string(var::String::Length length){
	return get_data(length.argument()/2).to_string();
}

var::Data Random::get_data(u32 size){
	Random r;
	r.initialize();
	r.seed();
	var::Data result(size);
	r.randomize(result);
	return result;
}
