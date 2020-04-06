/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
//Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#include <sys/stat.h>
#include <unistd.h>

#include "var/String.hpp"
#include "var/Tokenizer.hpp"
#include "fs/File.hpp"
#include "fs/Dir.hpp"
using namespace fs;
using namespace arg;



Dir::Dir(
		SAPI_LINK_DRIVER
		){
	m_dirp = 0;
#if defined __link
	m_dirp_local = 0;
	m_driver = link_driver.argument();
#endif
}

Dir::~Dir(){
	close();
}

int Dir::remove(
		const var::String & path,
		IsRecursive recursive
		SAPI_LINK_DRIVER_LAST
		){
	int ret = 0;
	if( recursive.argument() ){
		Dir d
		#if defined __link
				(link_driver)
		#endif
				;

		if( d.open(path) == 0 ){
			var::String entry;
			while( (entry = d.read()).is_empty() == false ){
				Stat info;
				var::String entry_path;
				entry_path << path << "/" << entry;
				info = File::get_info(
							entry_path
			#if defined __link
							, link_driver
			#endif
							);
				if( info.is_directory() ){
					if( entry != "." && entry != ".."){
						ret = Dir::remove(
									entry_path,
									IsRecursive(true)
			#if defined __link
									, link_driver
			#endif
									);
					}
				} else {
					ret = File::remove(
								entry_path
			#if defined __link
								, link_driver
			#endif
								);
				}
			}
		}
		d.close();
	}

	if( ret >= 0 ){
		//this will remove an empty directory or a file
#if defined __link
		if( link_driver.argument() ){
			ret = File::remove(
						path,
						link_driver
						);
		} else {
			ret = ::rmdir(
						path.cstring()
						);
		}
#else
		ret = File::remove(path);
#endif
	}

	return ret;
}

int Dir::copy(
		SourcePath source_path,
		DestinationPath destination_path,
		const sys::ProgressCallback * progress_callback
		#if defined __link
		, SourceLinkDriver source_driver,
		DestinationLinkDriver destination_driver
		#endif
		){

	var::Vector<var::String> source_contents =
			Dir::read_list(
				source_path.argument(),
				IsRecursive(false)
			#if defined __link
				, LinkDriver(source_driver.argument())
			#endif
				);

	for(auto entry: source_contents.vector()){
		var::String entry_path =
				var::String()
				<< source_path.argument()
				<< "/"
				<< entry;

		var::String destination_entry_path =
				var::String()
				<< destination_path.argument()
				<< "/"
				<< entry;

		FileInfo info =
				File::get_info(
					entry_path
			#if defined __link
					, LinkDriver(source_driver.argument())
			#endif
					);

		if( info.is_directory() ){
			//copy recursively

			if( Dir::create(
						destination_entry_path,
						Permissions::all_access(),
						IsRecursive(true)
			#if defined __link
						, LinkDriver(destination_driver.argument())
			#endif
						) < 0 ){
				return api::error_code_fs_failed_to_create;
			}

			copy(SourcePath(entry_path),
					 DestinationPath(destination_entry_path),
					 progress_callback
		 #if defined __link
					 , source_driver,
					 destination_driver
		 #endif
					 );

		} else if( info.is_file() ){
			int result;
			if( (result = File::copy(
						 SourcePath(entry_path),
						 DestinationPath(destination_entry_path),
						 IsOverwrite(true),
						 progress_callback
			 #if defined __link
						 , source_driver,
						 destination_driver
			 #endif
						 )) < 0 ){
				return result;
			}
		}
	}
	return 0;
}

int Dir::create(
		const var::String & path,
		const Permissions & permissions
		SAPI_LINK_DRIVER_LAST
		){
	int result;

#if defined __link
	if( link_driver.argument() ){
		result = link_mkdir(
					link_driver.argument(),
					path.cstring(),
					permissions.permissions()
					);
	} else {
		//open a directory on the local system (not over link)
#if defined __win32
		result = mkdir(path.cstring());
#else
		result = mkdir(
					path.cstring(),
					permissions.permissions()
					);
#endif
	}
#else
	result = mkdir(
				path.cstring(),
				permissions.permissions()
				);
#endif
	return result;
}


int Dir::create(
		const var::String & path,
		const Permissions & permissions,
		IsRecursive is_recursive
		SAPI_LINK_DRIVER_LAST
		){
	if( is_recursive.argument() == false ){
		return create(
					path,
					permissions
			#if defined __link
					, link_driver
			#endif
					);
	}
	var::Tokenizer path_tokens(
				path,
				var::Tokenizer::Delimeters("/")
				);
	var::String base_path;

	if( path.find("/") == 0 ){
		base_path << "/";
	}

	int result = 0;
	for(u32 i=0; i < path_tokens.count(); i++){
		if( path_tokens.at(i).is_empty() == false ){
			base_path << path_tokens.at(i);
			result = create(
						base_path,
						permissions
			#if defined __link
						, link_driver
			#endif
						);
			base_path << "/";
		}
	}

	return result;
}

bool Dir::exists(
		const var::String & path
		SAPI_LINK_DRIVER_LAST
		){
	Dir d
		#if defined __link
			(
				link_driver
				)
		#endif
			;
	if( d.open(path) < 0 ){ return false; }
	d.close();
	return true;
}


int Dir::open(const var::String & path){
#if defined __link
	if( driver() ){
		m_dirp = link_opendir(
					driver(),
					path.cstring()
					);
	} else {
		//open a directory on the local system (not over link)

		m_dirp_local = opendir(path.cstring());
		if( m_dirp_local == 0 ){
			return set_error_number_if_error(api::error_code_fs_failed_to_open);
		}
		m_path.assign(path);
		return 0;
	}
#else
	m_dirp = opendir(path.cstring());
#endif

	if( m_dirp == 0 ){
		return set_error_number_if_error(
					api::error_code_fs_failed_to_open
					);
	}

	m_path.assign(path);
	return 0;
}


#if !defined __link
int Dir::count(){
	long loc;
	int count;

	if( !is_open() ){
		return set_error_number_if_error(api::error_code_fs_not_open);
	}

#if defined __link

#else
	loc = tell();
#endif

#if defined __link

#else
	rewind();
#endif

	count = 0;
	while( read() != 0 ){
		count++;
	}

	seek(Location(loc));

	return count;

}

#endif

var::Vector<var::String> Dir::read_list(
		const var::String & path,
		std::function<const var::String(const var::String & entry)> filter,
		IsRecursive is_recursive
		SAPI_LINK_DRIVER_LAST
		){
	Dir directory
		#if defined __link
			(link_driver)
		#endif
			;
	var::Vector<var::String> result;
	if( directory.open(path) < 0 ){ return result; }
	result = directory.read_list(filter, is_recursive);
	directory.close();
	return result;
}


var::Vector<var::String> Dir::read_list(
		const var::String & path,
		IsRecursive is_recursive
		SAPI_LINK_DRIVER_LAST
		){
	return read_list(
				path,
				nullptr,
				is_recursive
			#if defined __link
				, link_driver
			#endif
				);
}


var::Vector<var::String> Dir::read_list(
		std::function<const var::String(const var::String & entry)> filter,
		IsRecursive is_recursive
		){
	var::Vector<var::String> result;
	var::String entry;
	bool is_the_end = false;

	do {
		entry.clear();
		entry = read();
		if( entry.is_empty() ){ is_the_end = true; }
		if( filter != nullptr ){
			entry = filter(entry);
		}
		if( !entry.is_empty() &&
				(entry != ".") &&
				(entry != "..") ){

			if( is_recursive.argument() ){
				var::String entry_path;
				entry_path << m_path << "/" << entry;
				FileInfo info = File::get_info(
							entry_path
			#if defined __link
							, LinkDriver(driver())
			#endif
							);

				if( info.is_directory() ){

					var::Vector<var::String> intermediate_result =
							Dir::read_list(
								entry_path,
								filter,
								is_recursive
			#if defined __link
								, LinkDriver(driver())
			#endif
								);

					for(auto intermediate_entry: intermediate_result){
						var::String push_entry;
						push_entry << entry << "/" << intermediate_entry;
						result.push_back(push_entry);
					}
				} else {
					result.push_back(entry);
				}
			} else {
				result.push_back(entry);
			}
		}

	} while( !is_the_end );

	return result;
}

var::Vector<var::String> Dir::read_list(
		const IsRecursive is_recursive
		){
	return read_list(
				std::function<const var::String(const var::String & entry)>(nullptr),
				is_recursive
				);
}

const char * Dir::read(){

#if defined __link
	if( driver() ){
		struct link_dirent * result;
		memset(&m_entry, 0, sizeof(m_entry));
		if( link_readdir_r(
					driver(),
					m_dirp,
					&m_entry,
					&result
					) < 0 ){
			return nullptr;
		}
	} else {
		struct dirent * result_local;
		if( (readdir_r(
					 m_dirp_local,
					 &m_entry_local,
					 &result_local) < 0) || (result_local == 0) ){
			return nullptr;
		}
		return m_entry_local.d_name;
	}
#else
	struct dirent * result;
	if( readdir_r(m_dirp, &m_entry, &result) < 0 ){
		set_error_number_to_errno();
		return nullptr;
	}
#endif
	return m_entry.d_name;
}

bool Dir::get_entry(var::String & path_dest){
	const char * entry = read();

	if( entry == 0 ){
		return false;
	}

	path_dest.clear();
	path_dest << m_path;
	if( m_path.is_empty() == false ){
		m_path << "/" << entry;
	}
	return true;
}

var::String Dir::get_entry(){
	var::String entry;
	get_entry(entry);
	return entry;
}

int Dir::close(){
	m_path.clear();
	if( m_dirp ){
#if defined __link
		if( driver() ){
			link_closedir(driver(), m_dirp);
		}
		m_dirp = 0;
#else //__link
		if( closedir(m_dirp) < 0 ){
			set_error_number_to_errno();
		}
		m_dirp = nullptr;
#endif
	}

#if defined __link
	if( m_dirp_local ){
		DIR * dirp_copy = m_dirp_local;
		m_dirp_local = 0;
		if( closedir(dirp_copy) < 0 ){
			return set_error_number_if_error(api::error_code_fs_failed_to_close);
		}
	}
#endif
	return 0;
}

