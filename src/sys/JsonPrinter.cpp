#include "sys/JsonPrinter.hpp"

using namespace sys;

JsonPrinter::JsonPrinter(){
	container_list().push_back(
				Container(level_fatal, container_array)
				);
	set_flags(print_no_progress_newline | print_key_quotes | print_value_quotes);
}

void JsonPrinter::print_open_object(
		enum verbose_level level,
		const char * key
		){
	open_object(key, level);
}

void JsonPrinter::print_close_object(){
	close_object();
}

void JsonPrinter::print(
		enum verbose_level level,
		const char * key,
		const char * value,
		bool is_newline
		){

	if(level > verbose_level()){
		return;
	}

	for(auto const & c: container_list() ){
		if(c.verbose_level() > verbose_level()){
			return;
		}
	}

	insert_comma();

	if( container().type() == container_array ){
		key = nullptr;
	}

	Printer::print(level, key, value, false);
}

JsonPrinter & JsonPrinter::open_object(
		const var::String & key,
		enum verbose_level level
		){

	if( verbose_level() >= level ){
		insert_comma();
		if( container().type() == container_object ){
			print_final("\"%s\":{", key.cstring());
		} else {
			print_final("{", key.cstring());
		}
	}

	container_list().push_back(
				Container(level, container_object)
				);
	return *this;
}

JsonPrinter & JsonPrinter::open_array(
		const var::String & key,
		enum verbose_level level
		){
	if( verbose_level() >= level ){
		insert_comma();
		if( container().type() == container_object ){
			print_final("\"%s\":[", key.cstring());
		} else {
			print_final("[", key.cstring());
		}
	}
	container_list().push_back(
				Container(level, container_array)
				);
	return *this;
}

JsonPrinter & JsonPrinter::close_object(){
	if( container_list().count() > 1 ){
		if( verbose_level() >= container().verbose_level() ){
			if( container().type() == container_array ){
				print_final("]");
			} else if( container().type() == container_object ){
				print_final("}");
			}
		}
		container_list().pop_back();
	}
	return *this;
}

void JsonPrinter::insert_comma(){
	if( container().count() > 1 ){
		print_final(",");
	}
	container().count() = container().count() + 1;
}


