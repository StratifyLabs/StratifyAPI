/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include <cstring>
#include <mcu/types.h>
#include "api/ApiObject.hpp"

#include "var/String.hpp"

#if defined __link
#define API_MINIMUM_CHUNK_SIZE 1024
#define API_MALLOC_CHUNK_SIZE 1024
#else
#define API_MINIMUM_CHUNK_SIZE 52
#define API_MALLOC_CHUNK_SIZE 64
#endif

using namespace api;

const char * ApiInfo::git_hash(){
	return SOS_GIT_HASH;
}

const char * ApiInfo::operating_system_name(){
#if defined __link

#if defined __macosx
	return "macosx";
#endif
#if defined __win32
	return "windows";
#endif
#if defined __linux
	return "linux";
#endif

#else
	return "stratifyos";
#endif

	return "unknown";
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

	return result.cstring();
#else
	return "/home";
#endif
}


const char * api::get_error_code_description(s32 ec){
	switch(ec){
		case error_code_flag_calc: return "error_code_flag_calc";
		case error_code_flag_chrono: return "error_code_flag_chrono";
		case error_code_flag_crypto: return "error_code_flag_crypto";
		case error_code_flag_fmt: return "error_code_flag_fmt";
		case error_code_flag_fs: return "error_code_flag_fs";
		case error_code_flag_hal: return "error_code_flag_hal";
		case error_code_flag_inet: return "error_code_flag_inet";
		case error_code_flag_sgfx: return "error_code_flag_sgfx";
		case error_code_flag_sys: return "error_code_flag_sys";
		case error_code_flag_test: return "error_code_flag_test";
		case error_code_flag_ux: return "error_code_flag_ux";
		case error_code_flag_var: return "error_code_flag_var";

		case error_code_crypto_size_mismatch: return "error_code_crypto_size_mismatch";
		case error_code_crypto_bad_block_size: return "error_code_crypto_bad_block_size";
		case error_code_crypto_operation_failed: return "error_code_crypto_operation_failed";
		case error_code_crypto_missing_api: return "error_code_crypto_missing_api";
		case error_code_crypto_unsupported_operation: return "error_code_crypto_unsupported_operation";
		case error_code_crypto_bad_iv_size: return "error_code_crypto_bad_iv_size";

		case error_code_fs_failed_to_open: return "error_code_fs_failed_to_open";
		case error_code_fs_failed_to_read: return "error_code_fs_failed_to_read";
		case error_code_fs_failed_to_write: return "error_code_fs_failed_to_write";
		case error_code_fs_failed_to_seek: return "error_code_fs_failed_to_seek";
		case error_code_fs_failed_to_create: return "error_code_fs_failed_to_create";
		case error_code_fs_failed_to_stat: return "error_code_fs_failed_to_stat";
		case error_code_fs_unsupported_operation: return "error_code_fs_unsupported_operation";
		case error_code_fs_bad_descriptor: return "error_code_fs_bad_descriptor";
		case error_code_fs_cant_read: return "error_code_fs_cant_read";
		case error_code_fs_cant_write: return "error_code_fs_cant_write";
		case error_code_fs_cant_write_read_only: return "error_code_fs_cant_write_read_only";
		case error_code_fs_cant_write_append_only: return "error_code_fs_cant_write_append_only";
		case error_code_fs_not_open: return "error_code_fs_not_open";
		case error_code_fs_failed_to_close: return "error_code_fs_failed_to_close";

		case error_code_inet_failed_to_create_socket: return "error_code_inet_failed_to_create_socket";
		case error_code_inet_failed_to_connect_to_socket: return "error_code_inet_failed_to_connect_to_socket";
		case error_code_inet_failed_to_write_header: return "error_code_inet_failed_to_write_header";
		case error_code_inet_failed_to_write_data: return "error_code_inet_failed_to_write_data";
		case error_code_inet_failed_to_write_incoming_data_to_file: return "error_code_inet_failed_to_write_incoming_data_to_file";
		case error_code_inet_failed_to_find_address: return "error_code_inet_failed_to_find_address";
		case error_code_inet_failed_to_get_status_code: return "error_code_inet_failed_to_get_status_code";
		case error_code_inet_failed_to_get_header: return "error_code_inet_failed_to_get_header";
		case error_code_inet_failed_wrong_domain: return "error_code_inet_failed_wrong_domain";
	}
	return "unknown error code";
}
