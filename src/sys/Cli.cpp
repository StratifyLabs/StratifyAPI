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
		m_path = argv[0];
		m_name = File::name(argv[0]);
	}
}

void Cli::handle_version() const {
	if( is_option("--version") || is_option("-v") ){
		u16 version;
		String tmp;
		version = Appfs::get_version(m_path);
		tmp.sprintf("%d.%d", version >> 8, version & 0xff);
		printf("%s version: %s by %s\n", m_name.c_str(), tmp.c_str(), m_publisher.c_str());
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

bool Cli::handle_uart(hal::UartAttr & attr) const {
	if( is_option("-uart") ){
		u32 o_flags = hal::Uart::FLAG_SET_LINE_CODING;
		attr.set_port(get_option_value("-uart"));

		if( is_option("-freq") ){
			attr.set_freq(get_option_value("-freq"));
		} else {
			attr.set_freq(115200);
		}

		if( is_option("-even") ){
			o_flags |= hal::Uart::FLAG_IS_PARITY_EVEN;
		} else if( is_option("-odd") ){
			o_flags |= hal::Uart::FLAG_IS_PARITY_ODD;
		}

		if( is_option("-stop1") ){
			o_flags |= hal::Uart::FLAG_IS_STOP1;
		} else if( is_option("-stop2") ){
			o_flags |= hal::Uart::FLAG_IS_STOP2;
		} else {
			o_flags |= hal::Uart::FLAG_IS_STOP1;
		}

		if( is_option("-tx") ){ attr.set_tx(get_option_pin("-tx")); }
		if( is_option("-rx") ){ attr.set_tx(get_option_pin("-rx")); }
		if( is_option("-rts") ){ attr.set_tx(get_option_pin("-rts")); }
		if( is_option("-cts") ){ attr.set_tx(get_option_pin("-cts")); }

		if( is_option("-width") ){
			attr.set_width(get_option_value("-w"));
		} else {
			attr.set_width(8);
		}

		attr.set_flags(o_flags);

		return true;
	}
	return false;
}

bool Cli::handle_i2c(hal::I2CAttr & attr) const {
	if( is_option("-i2c") ){
		u32 o_flags = hal::I2C::FLAG_SET_MASTER;
		attr.set_port(get_option_value("-i2c"));

		if( is_option("-freq") ){
			attr.set_freq(get_option_value("-freq"));
		} else {
			attr.set_freq(100000);
		}

		if( is_option("-slave") ){ attr.set_slave_addr(get_option_value("-slave")); }
		if( is_option("-pu") ){ o_flags |= hal::I2C::FLAG_IS_PULLUP; }


		if( is_option("-sda") ){ attr.set_sda(get_option_pin("-sda")); }
		if( is_option("-scl") ){ attr.set_scl(get_option_pin("-scl")); }

		attr.set_flags(o_flags);

		return true;
	}
	return false;
}







} /* namespace sys */
