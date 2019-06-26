#include "crypto/Sha256.hpp"

using namespace crypto;

Sha256::Sha256(){
	if( sha256_api().is_valid() == false ){
		exit_fatal("sha256_api api missing");
	}
	m_context = 0;
	m_is_finished = true;
}

Sha256::~Sha256(){
	finalize();
}

Sha256 & Sha256::operator << ( const var::Data & a){
	update(a.to_char(), a.size());
	return *this;
}

Sha256 & Sha256::operator << (const var::ConstString & a){
	update(a.cstring(), a.length());
	return *this;
}

Sha256 & Sha256::operator << ( const var::String & a){
	update(a.cstring(), a.length());
	return *this;
}

int Sha256::initialize(){
	finalize();
	return set_error_number_if_error(sha256_api()->init(&m_context));
}

var::String Sha256::stringify(){
	var::String result;
	const var::Array<u8, 32> & out = output();
	for(u32 i=0; i < out.size(); i++){
		result << var::String().format("%02x", out.at(i));
	}
	return result;
}

const var::Array<u8, 32> & Sha256::output(){
	finish();
	return m_output;
}

int Sha256::finalize(){
	if( m_context != 0 ){
		sha256_api()->deinit(&m_context);
	}
	return 0;
}

int Sha256::start(){
	m_is_finished = false;
	return set_error_number_if_error(sha256_api()->start(m_context));
}

int Sha256::update(const char * input, u32 len){
	if( is_initialized() == false ){
		initialize();
	}

	if( m_is_finished ){
		start();
	}

	return set_error_number_if_error(sha256_api()->update(m_context, (const unsigned char*)input, len));
}

int Sha256::finish(){
	if( m_is_finished == false){
		m_is_finished = true;
		return set_error_number_if_error(sha256_api()->finish(m_context, (unsigned char*)m_output.data(), m_output.size()));
	}
	return 0;
}
