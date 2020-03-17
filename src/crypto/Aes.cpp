/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#include <errno.h>
#include "crypto/Aes.hpp"

using namespace crypto;

Aes::~Aes(){
	m_initialization_vector.fill(0);
	finalize();
}

int Aes::initialize(){
	if( aes_api().is_valid() == false ){
		return set_error_number_if_error(api::error_code_crypto_missing_api);
	}
	finalize();
	int result = 0;
	if( aes_api()->init(&m_context) < 0 ){
		result = api::error_code_crypto_operation_failed;
	}
	return set_error_number_if_error(result);
}

int Aes::finalize(){
	if( m_context != nullptr ){
		aes_api()->deinit(&m_context);
	}
	return 0;
}

Aes & Aes::set_initialization_vector(
		const var::Reference & value
		){
	m_initialization_vector.resize(value.size());
	var::Reference::memory_copy(
				var::Reference::SourceBuffer(value.to_const_void()),
				var::Reference::DestinationBuffer(m_initialization_vector.to_void()),
				var::Reference::Size(m_initialization_vector.size())
				);
	return *this;
}

Aes & Aes::set_key(
		const var::Reference & key
		){
	set_error_number_if_error(
				aes_api()->set_key(
					m_context,
					key.to_const_u8(),
					key.size() * 8,
					8
					)
				);
	return *this;
}


int Aes::encrypt_ecb(
		SourcePlainData source_data,
		DestinationCipherData destination_data
		){
	if( source_data.argument().size() >
			destination_data.argument().size() ){
		return set_error_number_if_error(api::error_code_crypto_size_mismatch);
	}

	if( source_data.argument().size() % 16 != 0 ){
		return set_error_number_if_error(api::error_code_crypto_bad_block_size);
	}

	for(
			u32 i=0;
			i < source_data.argument().size();
			i+=16
			){
		if( aes_api()->encrypt_ecb(
					m_context,
					source_data.argument().to_const_u8() + i,
					destination_data.argument().to_u8() + i
					) < 0 ){
			return -1;
		}
	}
	return source_data.argument().size();

}

int Aes::decrypt_ecb(
		SourceCipherData source_data,
		DestinationPlainData destination_data
		){

	if( source_data.argument().size() >
			destination_data.argument().size() ){
		return set_error_number_if_error(api::error_code_crypto_size_mismatch);
	}

	if( source_data.argument().size() % 16 != 0 ){
		return set_error_number_if_error(
					api::error_code_crypto_bad_block_size
					);
	}

	for(u32 i=0; i < source_data.argument().size(); i+=16){
		if( aes_api()->decrypt_ecb(
					m_context,
					source_data.argument().to_const_u8() + i,
					destination_data.argument().to_u8() + i
					) < 0 ){
			return set_error_number_if_error(
						api::error_code_crypto_operation_failed
						);
		}
	}

	return set_error_number_if_error(
				source_data.argument().size()
				);
}

int Aes::encrypt_cbc(
		SourcePlainData source_data,
		DestinationCipherData destination_data
		){

	if( source_data.argument().size() >
			destination_data.argument().size() ){
		return set_error_number_if_error(api::error_code_crypto_size_mismatch);

	}

	if( source_data.argument().size() % 16 != 0 ){
		return set_error_number_if_error(api::error_code_crypto_bad_block_size);
	}

	int result;
	if( (result = aes_api()->encrypt_cbc(
				 m_context,
				 source_data.argument().size(),
				 m_initialization_vector.to<unsigned char>(), //init vector
				 source_data.argument().to_const_u8(),
				 destination_data.argument().to_u8()
				 ) < 0)
			){
		result = api::error_code_crypto_operation_failed;
	}

	return set_error_number_if_error(result);
}

int Aes::decrypt_cbc(
		SourceCipherData source_data,
		DestinationPlainData destination_data
		){

	if( source_data.argument().size() >
			destination_data.argument().size() ){
		return set_error_number_if_error(api::error_code_crypto_size_mismatch);
	}

	if( source_data.argument().size() % 16 != 0 ){
		return set_error_number_if_error(api::error_code_crypto_bad_block_size);
	}

	int result;
	if( (result = aes_api()->decrypt_cbc(
				 m_context,
				 source_data.argument().size(),
				 m_initialization_vector.to<unsigned char>(), //init vector
				 source_data.argument().to_const_u8(),
				 destination_data.argument().to_u8()
				 )) < 0 ){
		result = api::error_code_crypto_operation_failed;
	}

	return set_error_number_if_error(result);
}

int Aes::encrypt_ctr(
		SourcePlainData source_data,
		DestinationCipherData destination_data
		){
	return set_error_number_if_error(
				api::error_code_crypto_unsupported_operation
				);
}

int Aes::decrypt_ctr(
		SourceCipherData source_data,
		DestinationPlainData destination_data
		){
	return set_error_number_if_error(
				api::error_code_crypto_unsupported_operation
				);
}

