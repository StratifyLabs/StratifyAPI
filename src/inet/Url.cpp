/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "inet/Url.hpp"
#include "var/Token.hpp"

using namespace inet;

Url::Url(const var::ConstString & url){
	set(url);
}

int Url::set(const var::ConstString & url){

	if( url.is_empty() == false ){

		var::Tokenizer url_tokens(
					arg::TokenEncodedString(url),
					arg::TokenDelimeters("/")
					);

		// https://domain.name:port/path

		if( url_tokens.count() < 2 ){
			return -1;
		}

		if( url_tokens.at(0) == "https:"){
			m_port = 443;
			m_protocol = PROTOCOL_HTTPS;
		} else if( url_tokens.at(0) == "http:"){
			m_port = 80;
			m_protocol = PROTOCOL_HTTP;
		}

		var::Tokenizer domain_name(
					arg::TokenEncodedString(url_tokens.at(1)),
					arg::TokenDelimeters(":")
					);

		if( domain_name.count() > 1 ){
			m_port = domain_name.at(1).atoi();
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

var::String Url::encode(const var::ConstString & input){
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

var::String Url::decode(const var::ConstString & input){
	return var::String();
}
