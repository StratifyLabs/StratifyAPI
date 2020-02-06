/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#include "inet/Url.hpp"
#include "var/Token.hpp"

using namespace inet;

Url::Url(const var::String & url){
	set(url);
}

int Url::set(const var::String & url){

	var::String url_string(url);

	if( url_string.is_empty() == false ){

		var::Tokenizer url_tokens(
					url,
					var::Tokenizer::Delimeters("/")
					);

		// https://domain.name:port/path

		if( url_tokens.count() < 2 ){
			return -1;
		}

		if( var::String(url_tokens.at(0)) == "https:"){
			m_port = 443;
			m_protocol = PROTOCOL_HTTPS;
		} else if( var::String(url_tokens.at(0)) == "http:"){
			m_port = 80;
			m_protocol = PROTOCOL_HTTP;
		}

		var::Tokenizer domain_name(
					url_tokens.at(1),
					var::Tokenizer::Delimeters(":")
					);

		if( domain_name.count() > 1 ){
			m_port = var::String(domain_name.at(1)).to_integer();
			m_domain_name = domain_name.at(0);
		} else {
			m_domain_name = url_tokens.at(1);
		}

		m_path.clear();
		for(u32 i = 2; i < url_tokens.count(); i++){
			m_path << "/" << url_tokens.at(i);
		}

	}

	return 0;
}

var::String Url::to_string() const {
	var::String result;
	var::String p;
	if( m_protocol == PROTOCOL_HTTPS ){
		p = "https";
	} else {
		p = "http";
	}
	result << p << "://" << m_domain_name << m_path;
	return result;
}

var::String Url::encode(const var::String & input){
	var::String result;
	u32 length = input.length();
	for(u32 i=0; i < length; i++){
		char c = input.at(i);
		if( (c < '0') ||
			 ((c > '9') && (c < 'A')) ||
			 ((c > 'Z') && (c < 'a')) ||
			 (c > 'z')
			 ){
			result << var::String().format("%%%X", c);
		} else {
			result << c;
		}
	}
	return result;
}

var::String Url::decode(const var::String & input){
	return var::String();
}
