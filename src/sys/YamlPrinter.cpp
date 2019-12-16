#include "sys/YamlPrinter.hpp"


using namespace sys;


YamlPrinter::YamlPrinter(){
	container_list().push_back(
				Container(level_fatal, container_array)
				);
}

void YamlPrinter::print_open_object(
		enum verbose_level level,
		const char * key
		){
	open_object(key, level);
}

void YamlPrinter::print_close_object(){
	close_object();
}

void YamlPrinter::print(
		enum verbose_level level,
		const char * key,
		const char * value){

	if(level > verbose_level()){
		return;
	}

	for(auto const & c: container_list() ){
		if(c.verbose_level() > verbose_level()){
			return;
		}
	}

	for(size_t i=0; i < container_list().count() - 1; i++){
		print_final("   ");
	}

	if( container().type() == container_array ){
		print_final("- ");
	}

	if( key != nullptr ){
		if( o_flags() & PRINT_BOLD_KEYS ){ set_format_code(FORMAT_BOLD); }
		if( o_flags() & PRINT_CYAN_KEYS ){ set_color_code(COLOR_CODE_CYAN); }
		if( o_flags() & PRINT_YELLOW_KEYS ){ set_color_code(COLOR_CODE_YELLOW); }
		if( o_flags() & PRINT_MAGENTA_KEYS ){ set_color_code(COLOR_CODE_MAGENTA); }
		print_final("%s: ", key);
		if( o_flags() & PRINT_BOLD_KEYS ){ clear_format_code(FORMAT_BOLD); }
		if( o_flags() & (PRINT_CYAN_KEYS | PRINT_YELLOW_KEYS | PRINT_MAGENTA_KEYS) ){ clear_color_code(); }
	}

	if( value != nullptr ){
		if( o_flags() & PRINT_BOLD_VALUES ){ set_format_code(FORMAT_BOLD); }
		if( o_flags() & PRINT_GREEN_VALUES){ set_color_code(COLOR_CODE_GREEN); }
		if( o_flags() & PRINT_YELLOW_VALUES){ set_color_code(COLOR_CODE_YELLOW); }
		if( o_flags() & PRINT_RED_VALUES){ set_color_code(COLOR_CODE_RED); }

		print_final(value);

		if( o_flags() & (PRINT_GREEN_VALUES | PRINT_YELLOW_VALUES | PRINT_RED_VALUES) ){ clear_color_code(); }
		if( o_flags() & PRINT_BOLD_VALUES ){ clear_format_code(FORMAT_BOLD); }
		print_final("\n");
	}

}

YamlPrinter & YamlPrinter::open_object(
		const var::String & key,
		enum verbose_level level
		){

	if( verbose_level() >= level ){
		if( o_flags() & PRINT_BOLD_OBJECTS ){ set_format_code(FORMAT_BOLD); }
		print(level, key.cstring(), " ");
		if( o_flags() & PRINT_BOLD_OBJECTS ){ clear_format_code(FORMAT_BOLD); }
	}

	container_list().push_back(
				Container(level, container_object)
				);
	return *this;
}

YamlPrinter & YamlPrinter::open_array(
		const var::String & key,
		enum verbose_level level
		){
	if( verbose_level() >= level ){
		if( o_flags() & PRINT_BOLD_OBJECTS ){ set_format_code(FORMAT_BOLD); }
		print(level, key.cstring(), " ");
		if( o_flags() & PRINT_BOLD_OBJECTS ){ clear_format_code(FORMAT_BOLD); }
	}
	container_list().push_back(
				Container(level, container_array)
				);
	return *this;
}

YamlPrinter & YamlPrinter::close_object(){
	if( container_list().count() > 1 ){
		container_list().pop_back();
	}
	return *this;
}


