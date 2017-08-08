/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "sys/File.hpp"
#include "sys/Appfs.hpp"
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

void Cli::handle_version() const {
	const char * name;
	if( is_option("--version") || is_option("-v") ){
		u16 version;
		String tmp;
		version = Appfs::get_version(m_name);
		tmp.sprintf("%d.%d", version >> 8, version & 0xff);
		name = File::name(m_name.c_str());
		printf("%s version: %s by %s\n", name, tmp.c_str(), m_publisher.c_str());
		exit(0);
	}
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

mcu_pin_t Cli::get_option_pin(const char * option) const {
	mcu_pin_t pio;
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

mcu_pin_t Cli::pin_at(u16 value) const {
	mcu_pin_t pio;
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
