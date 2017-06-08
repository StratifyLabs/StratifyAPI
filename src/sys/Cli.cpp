/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "sys/Cli.hpp"

using namespace var;

namespace sys {

Cli::Cli(int argc, char * argv[]){
	if( argc < 0 ){
		argc = 0;
	}
	m_argc = argc;
	m_argv = argv;

	if( argc > 0 ){
		m_name = argv[0];
	}
}

void Cli::print_version() const {
	printf("%s version: %s by %s\n", m_name.c_str(), m_version.c_str(), m_publisher.c_str());
}

String Cli::at(u16 value) const {
	String arg;
	if( value < m_argc ){
		arg.assign(m_argv[value]);
	}
	return arg;
}

String Cli::get_option_argument(const char * option) const {
	u16 args;
	for(args = 0; args < m_argc; args++){
		if( at(args) == option ){
			return at(args+1);
		}
	}
	return String();
}

bool Cli::is_option(const char * value) const {
	u16 i;
	for(i=0; i < m_argc; i++){
		if( at(i) == value ){
			return true;
		}
	}
	return false;
}

int Cli::get_option_value(const char * option) const {
	String arg = get_option_argument(option);
	if( arg.empty() ){
		return 0;
	}
	return arg.atoi();
}

pio_t Cli::get_option_pio(const char * option) const {
	pio_t pio;
	Token arg;
	arg.assign(get_option_argument(option).c_str());
	arg.parse(".");

	if( arg.size() == 2 ){
		pio.port = atoi(arg.at(0));
		pio.pin = atoi(arg.at(1));
	} else {
		pio.port = 255;
		pio.pin = 255;
	}

	return pio;
}

pio_t Cli::pio_at(u16 value) const {
	pio_t pio;
	Token arg;

	arg.assign( at(value).c_str() );
	arg.parse(".");

	if( arg.size() == 2 ){
		pio.port = atoi(arg.at(0));
		pio.pin = atoi(arg.at(1));
	} else {
		pio.port = 255;
		pio.pin = 255;
	}

	return pio;
}

int Cli::value_at(u16 value) const {
	return at(value).atoi();
}





} /* namespace sys */
