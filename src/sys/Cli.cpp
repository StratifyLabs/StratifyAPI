/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "fs/File.hpp"
#include "sys/Appfs.hpp"
#include "sys/Cli.hpp"

using namespace var;

namespace sys {

Cli::Cli(int argc, char * argv[], const var::ConstString & app_git_hash) : m_app_git_hash(app_git_hash){
	u16 version;
	if( argc < 0 ){
		argc = 0;
	}
	m_argc = argc;
	m_argv = argv;
	m_is_case_sensitive = true;

	if( argc > 0 ){
		m_path = argv[0];
		m_name = fs::File::name(
					arg::FilePath(argv[0])
				);
#if defined __link
		version = Appfs::get_version(
					arg::SourceFilePath(m_path.cstring()),
					0);
#else
		version = Appfs::get_version(
					arg::SourceFilePath(m_path.cstring())
					);
#endif

		m_version.format("%d.%d", version >> 8, version & 0xff);
	}
}

void Cli::handle_version() const {
#if !defined __link
	if( is_option("--version") || is_option("-v") ){
		printf("%s version: %s by %s\n", m_name.cstring(), m_version.cstring(), m_publisher.cstring());
		exit(0);
	}

	if( is_option("--version-details") ){
		String details = get_version_details();
		printf("%s\n", details.cstring());
		exit(0);
	}
#endif
}

var::String Cli::get_version_details() const {
	String result;

	if( m_app_git_hash == 0 ){
		result.sprintf("%s (api:%s)", m_version.cstring(), api::ApiInfo::git_hash());
	} else {
		result.sprintf("%s (app:%s, api:%s)", m_version.cstring(), m_app_git_hash.cstring(), api::ApiInfo::git_hash());
	}

	return result;
}

var::String Cli::to_string() const {
	String result;
	for(u32 i=1; i < count(); i++){
		result << at(i);
		if( i < count()-1){
			result << " ";
		}
	}
	return result;
}


String Cli::at(u16 value) const {
	String arg;
	if( value < m_argc ){
		arg.assign(m_argv[value]);
	}
	return arg;
}

bool Cli::is_option_equivalent_to_argument(
		const ConstString & option,
		const ConstString & argument) const {
	String option_string = option;
	String argument_string = argument;
	if( is_case_senstive() == false ){
		option_string.to_upper();
		argument_string.to_upper();
	}

	return compare_with_prefix(option_string, argument_string);
}

bool Cli::compare_with_prefix(
		const var::ConstString & option,
		const var::ConstString & argument
		) const {
	String with_prefix;
	if( argument.at(0) != '-' ){ return false; }
	if( option == argument ){ return true; }
	with_prefix << "--" << option;
	if( with_prefix == argument ){ return true; }
	if( with_prefix.substr(arg::Position(1)) == argument ){ return true; }
	return false;
}


bool Cli::is_option_equivalent_to_argument_with_equality(
		const var::ConstString & option,
		const var::ConstString & argument,
		var::String & value) const {

	if( argument.at(0) != '-' ){
		return false;
	}

	Tokenizer tokens(
				arg::TokenEncodedString(argument),
				arg::TokenDelimeters("="),
				arg::IgnoreTokensBetween(""),
				arg::IsCountEmptyTokens(false),
				arg::MaximumTokenCount(1));
	if( tokens.count() == 2 ){
		String a = option;
		String b = tokens.at(0);
		if( is_case_senstive() == false ){
			a.to_upper();
			b.to_upper();
		}

		if( compare_with_prefix(a, b) ){
			value = tokens.at(1);
			return true;
		}
	}
	return false;
}


var::String Cli::get_option(
		const arg::OptionName name,
		const arg::OptionDescription help
		) const {
	var::String result;
	u32 args;


	if( help.argument().is_empty() == false ){
		m_help_list.push_back(String() << name.argument() << ": " << help.argument());
	}

	for(args = 1; args < count(); args++){
		if( is_option_equivalent_to_argument(name.argument(), at(args)) ){
			if( count() > args+1 ){
				ConstString value = at(args+1);
				if( value.at(0) == '-' ){
					result = "true";
				} else {
					result = at(args+1);
				}
				return result;
			} else {
				return ConstString("true");
			}
		} else if( is_option_equivalent_to_argument_with_equality(name.argument(), at(args), result)){
			return result;
		}
	}
	return String();
}

String Cli::get_option_argument(const char * option) const {
	u16 args;
	for(args = 0; args < m_argc; args++){
		if( is_option_equivalent_to_argument(option, at(args).cstring()) ){
			return at(args+1);
		}
	}
	return String();
}

bool Cli::is_option(const var::ConstString & value) const {
	u16 i;
	for(i=0; i < m_argc; i++){
		if( is_option_equivalent_to_argument(value, at(i).cstring()) ){
			return true;
		}
	}
	return false;
}

int Cli::get_option_value(const char * option) const {
	String arg = get_option_argument(option);
	if( arg.is_empty() ){
		return 0;
	}
	return arg.to_integer();
}

int Cli::get_option_hex_value(const char * option) const {
	int value;
	String arg = get_option_argument(option);
	if( arg.is_empty() ){
		return 0;
	}
	value = 0;
	sscanf(arg.cstring(), "0x%X", &value);
	return value;
}


mcu_pin_t Cli::get_option_pin(const char * option) const {
	mcu_pin_t pio;
	Tokenizer arg;
	arg.assign(get_option_argument(option));
	arg.parse(
				arg::TokenDelimeters(".")
				);

	if( arg.size() == 2 ){
		pio.port = arg.at(0).to_integer();
		pio.pin = arg.at(1).to_integer();
	} else {
		pio.port = 255;
		pio.pin = 255;
	}

	return pio;
}

mcu_pin_t Cli::pin_at(u16 value) const {
	mcu_pin_t pio;
	Tokenizer arg;

	arg.assign( at(value) );
	arg.parse(
				arg::TokenDelimeters(".")
				);

	if( arg.size() == 2 ){
		pio.port = arg.at(0).to_integer();
		pio.pin = arg.at(1).to_integer();
	} else {
		pio.port = 255;
		pio.pin = 255;
	}

	return pio;
}

int Cli::value_at(u16 value) const {
	return at(value).to_integer();
}

bool Cli::handle_uart(hal::UartAttributes & attr) const {
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
		if( is_option("-rx") ){ attr.set_rx(get_option_pin("-rx")); }
		if( is_option("-rts") ){ attr.set_rts(get_option_pin("-rts")); }
		if( is_option("-cts") ){ attr.set_cts(get_option_pin("-cts")); }

		if( is_option("-width") ){
			attr.set_width(get_option_value("-width"));
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

		if( is_option("-slave") ){ attr.set_slave_addr(get_option_hex_value("-slave")); }
		if( is_option("-pu") ){ o_flags |= hal::I2C::FLAG_IS_PULLUP; }


		if( is_option("-sda") ){ attr.set_sda(get_option_pin("-sda")); }
		if( is_option("-scl") ){ attr.set_scl(get_option_pin("-scl")); }

		attr.set_flags(o_flags);

		return true;
	}
	return false;
}


void Cli::show_options() const {
	printf("%s options:\n", name().cstring());
	for(u32 i=0; i < m_help_list.count(); i++){
		printf("- %s\n", m_help_list.at(i).cstring());
	}
}





} /* namespace sys */
