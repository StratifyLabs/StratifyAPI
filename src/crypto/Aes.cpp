#include "crypto/Aes.hpp"

using namespace crypto;

Aes::Aes(){

}

Aes::~Aes(){
	m_initialization_vector.fill(0);

	finalize();
}

int Aes::initialize(){
	return aes_api()->init(&m_context);
}

int Aes::finalize(){
	if( m_context != 0 ){
		aes_api()->deinit(&m_context);
	}
	return 0;
}

Aes & Aes::set_key(
		const var::Data & key
		){
	set_error_number_if_error(
				aes_api()->set_key(
					m_context,
					key.to_const_u8(),
					key.size() * 8
					)
				);
	return *this;
}


int Aes::encrypt_ecb(
		SourceReference source_data,
		DestinationReference destination_data
		){
	if( source_data.argument().size() !=
		 destination_data.argument().size() ){
		return -1;
	}

	if( source_data.argument().size() % 16 != 0 ){
		return -1;
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
		SourceReference source_data,
		DestinationReference destination_data
		){

	if( source_data.argument().size() !=
		 destination_data.argument().size() ){
		return -1;
	}

	if( source_data.argument().size() % 16 != 0 ){
		return -1;
	}

	for(u32 i=0; i < source_data.argument().size(); i+=16){
		if( aes_api()->decrypt_ecb(
				 m_context,
				 source_data.argument().to_const_u8() + i,
				 destination_data.argument().to_u8() + i
				 ) < 0 ){
			return -1;
		}
	}
	return source_data.argument().size();
}

int Aes::encrypt_cbc(
		SourceReference source_data,
		DestinationReference destination_data
		){

	if( source_data.argument().size() !=
		 destination_data.argument().size() ){
		return -1;
	}

	if( source_data.argument().size() % 16 != 0 ){
		return -1;
	}

	if( aes_api()->encrypt_cbc(
			 m_context,
			 source_data.argument().size(),
			 m_initialization_vector.to<unsigned char>(), //init vector
			 source_data.argument().to_const_u8(),
			 destination_data.argument().to_u8()
			 ) < 0 ){
		return -1;
	}


	return 0;
}

int Aes::decrypt_cbc(
		SourceReference source_data,
		DestinationReference destination_data
		){

	if( source_data.argument().size() !=
		 destination_data.argument().size() ){
		return -1;
	}

	if( source_data.argument().size() % 16 != 0 ){
		return -1;
	}

	if( aes_api()->decrypt_cbc(
			 m_context,
			 source_data.argument().size(),
			 m_initialization_vector.to<unsigned char>(), //init vector
			 source_data.argument().to_const_u8(),
			 destination_data.argument().to_u8()
			 ) < 0 ){
		return -1;
	}


	return 0;
}

int Aes::encrypt_ctr(
		SourceReference source_data,
		DestinationReference destination_data
		){
	return -1;
}

int Aes::decrypt_ctr(
		SourceReference source_data,
		DestinationReference destination_data
		){
	return -1;
}

