/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include <cstring>
#include <mcu/types.h>
#include "api/ApiObject.hpp"
#include "var/String.hpp"

#if defined __link
#define API_MINIMUM_CHUNK_SIZE 1024
#define API_MALLOC_CHUNK_SIZE 1024
#if !defined __win32
#include <execinfo.h>
#endif
#else
#include <mcu/arch.h>
#define API_MALLOC_CHUNK_SIZE MALLOC_CHUNK_SIZE
#define API_MINIMUM_CHUNK_SIZE (MALLOC_CHUNK_SIZE-12)
#endif

#define ERROR_CODE_CASE(c) \
	case c: return MCU_STRINGIFY(c)

using namespace api;

const char * ApiInfo::git_hash(){
	return SOS_GIT_HASH;
}

const char * ApiInfo::operating_system_name(){
#if defined __link

#if defined __macosx
	return "macosx";
#elif defined __win32
	return "windows";
#elif defined __linux
	return "linux";
#else
	return "unknown";
#endif

#else
	return "stratifyos";
#endif

}

bool ApiInfo::is_windows(){ return strcmp(operating_system_name(), "windows") == 0; }
bool ApiInfo::is_macosx(){ return strcmp(operating_system_name(), "macosx") == 0; }
bool ApiInfo::is_linux(){ return strcmp(operating_system_name(), "linux") == 0; }
bool ApiInfo::is_stratify_os(){ return strcmp(operating_system_name(), "stratifyos") == 0; }


u32 ApiInfo::malloc_start_chunk_size(){ return API_MINIMUM_CHUNK_SIZE; }
u32 ApiInfo::malloc_chunk_size(){ return API_MALLOC_CHUNK_SIZE; }


const char * ApiInfo::user_data_path(){

#if defined __link
	static var::String result;
	result.clear();
	char * path;
	if( is_windows() ){
		path = getenv("LocalAppData");
		result << path;
	}

	if( is_macosx() ){
		//read env home variable
		path = getenv("HOME");
		result << path << "/Library/Application Support";
	}

	if( is_linux() ){
		path = getenv("HOME");
		result << path << "/.sl";
	}

	return result.cstring();
#else
	return "/home";
#endif
}

void api::api_assert(
		bool value,
		const char * function,
		int line){
	if( !value ){
		printf("assertion %s():%d\n", function, line);
#if defined __link && !defined __win32
		void *array[200];
		size_t size;
		size = backtrace(array,100);
		backtrace_symbols_fd(array, size, STDERR_FILENO);
#endif
		::abort();
	}
}

const char * api::get_error_code_description(s32 ec){
	switch(ec){
		ERROR_CODE_CASE(error_code_flag_calc);
		ERROR_CODE_CASE(error_code_flag_chrono);
		ERROR_CODE_CASE(error_code_flag_crypto);
		ERROR_CODE_CASE(error_code_flag_fmt);
		ERROR_CODE_CASE(error_code_flag_fs);
		ERROR_CODE_CASE(error_code_flag_hal);
		ERROR_CODE_CASE(error_code_flag_inet);
		ERROR_CODE_CASE(error_code_flag_sgfx);
		ERROR_CODE_CASE(error_code_flag_sys);
		ERROR_CODE_CASE(error_code_flag_test);
		ERROR_CODE_CASE(error_code_flag_ux);
		ERROR_CODE_CASE(error_code_flag_var);

		ERROR_CODE_CASE(error_code_crypto_size_mismatch);
		ERROR_CODE_CASE(error_code_crypto_bad_block_size);
		ERROR_CODE_CASE(error_code_crypto_operation_failed);
		ERROR_CODE_CASE(error_code_crypto_missing_api);
		ERROR_CODE_CASE(error_code_crypto_unsupported_operation);
		ERROR_CODE_CASE(error_code_crypto_bad_iv_size);


		ERROR_CODE_CASE(error_code_fs_failed_to_open);
		ERROR_CODE_CASE(error_code_fs_failed_to_read);
		ERROR_CODE_CASE(error_code_fs_failed_to_write);
		ERROR_CODE_CASE(error_code_fs_failed_to_seek);
		ERROR_CODE_CASE(error_code_fs_failed_to_create);
		ERROR_CODE_CASE(error_code_fs_failed_to_stat);
		ERROR_CODE_CASE(error_code_fs_unsupported_operation);
		ERROR_CODE_CASE(error_code_fs_bad_descriptor);
		ERROR_CODE_CASE(error_code_fs_cant_read);
		ERROR_CODE_CASE(error_code_fs_cant_write);
		ERROR_CODE_CASE(error_code_fs_cant_write_read_only);
		ERROR_CODE_CASE(error_code_fs_cant_write_append_only);
		ERROR_CODE_CASE(error_code_fs_not_open);
		ERROR_CODE_CASE(error_code_fs_failed_to_close);
		ERROR_CODE_CASE(error_code_fs_failed_to_unlink);
		ERROR_CODE_CASE(error_code_fs_failed_to_rmdir);

		ERROR_CODE_CASE(error_code_inet_failed_to_create_socket);
		ERROR_CODE_CASE(error_code_inet_failed_to_connect_to_socket);
		ERROR_CODE_CASE(error_code_inet_failed_to_write_header);
		ERROR_CODE_CASE(error_code_inet_failed_to_write_data);
		ERROR_CODE_CASE(error_code_inet_failed_to_write_incoming_data_to_file);
		ERROR_CODE_CASE(error_code_inet_failed_to_find_address);
		ERROR_CODE_CASE(error_code_inet_failed_to_get_status_code);
		ERROR_CODE_CASE(error_code_inet_failed_to_get_header);
		ERROR_CODE_CASE(error_code_inet_failed_wrong_domain);
		ERROR_CODE_CASE(error_code_inet_wifi_api_missing);

		ERROR_CODE_CASE(error_code_var_json_unknown);
		ERROR_CODE_CASE(error_code_var_json_out_of_memory);
		ERROR_CODE_CASE(error_code_var_json_stack_overflow);
		ERROR_CODE_CASE(error_code_var_json_cannot_open_file);
		ERROR_CODE_CASE(error_code_var_json_invalid_argument);
		ERROR_CODE_CASE(error_code_var_json_invalid_utf8);
		ERROR_CODE_CASE(error_code_var_json_premature_end_of_input);
		ERROR_CODE_CASE(error_code_var_json_end_of_input_expected);
		ERROR_CODE_CASE(error_code_var_json_invalid_syntax);
		ERROR_CODE_CASE(error_code_var_json_invalid_format);
		ERROR_CODE_CASE(error_code_var_json_wrong_type);
		ERROR_CODE_CASE(error_code_var_json_null_character);
		ERROR_CODE_CASE(error_code_var_json_null_value);
		ERROR_CODE_CASE(error_code_var_json_null_byte_in_key);
		ERROR_CODE_CASE(error_code_var_json_duplicate_key);
		ERROR_CODE_CASE(error_code_var_json_numeric_overflow);
		ERROR_CODE_CASE(error_code_var_json_item_not_found);
		ERROR_CODE_CASE(error_code_var_json_index_out_of_range);

	}
	return "unknown error code";
}
