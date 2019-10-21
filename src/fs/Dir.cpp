//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include <sys/stat.h>
#include <unistd.h>

#include "var/String.hpp"
#include "var/Tokenizer.hpp"
#include "fs/File.hpp"
#include "fs/Dir.hpp"
using namespace fs;
using namespace arg;



#if defined __link
Dir::Dir(arg::LinkDriver driver){
	m_dirp = 0;
	m_driver = driver.argument();
	m_dirp_local = 0;
}

#else
Dir::Dir(){
	m_dirp = 0;
}

#endif

Dir::~Dir(){
	close();
}

#if !defined __link
int Dir::remove(
		const arg::SourceDirectoryPath path,
		const arg::IsRecursive recursive
		){
#else
int Dir::remove(
		const arg::SourceDirectoryPath path,
		const arg::IsRecursive recursive,
		arg::LinkDriver driver
		){
#endif
	int ret = 0;
	if( recursive.argument() ){
#if defined __link
		Dir d(driver);
#else
		Dir d;
#endif
		if( d.open(
				 path
				 ) == 0 ){
			var::String entry;
			while( (entry = d.read()).is_empty() == false ){
				Stat info;
				var::String entry_path;
				entry_path << path.argument() << "/" << entry;
				info = File::get_info(
							SourceFilePath(entry_path)
			#if defined __link
							, driver
			#endif
							);
				if( info.is_directory() ){
					if( entry != "." && entry != ".."){
						ret = Dir::remove(
									arg::SourceDirectoryPath(entry_path),
									arg::IsRecursive(true)
			#if defined __link
									, driver
			#endif
									);
					}
				} else {
					ret = File::remove(
								arg::SourceFilePath(entry_path)
			#if defined __link
								, driver
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
		if( driver.argument() ){
			ret = File::remove(
						arg::SourceFilePath(path.argument()),
						driver
						);
		} else {
			ret = ::rmdir(
						path.argument().cstring()
						);
		}
#else
		ret = File::remove(
					arg::SourceFilePath(path.argument())
					);
#endif
	}

	return ret;
}

int Dir::copy(
		const arg::SourceDirectoryPath source_path,
		const arg::DestinationDirectoryPath destination_path
		#if defined __link
		, arg::SourceLinkDriver source_driver,
		arg::DestinationLinkDriver destination_driver
		#endif
		){

	var::Vector<var::String> source_contents =
			Dir::read_list(
				source_path,
				arg::IsRecursive(false)
			#if defined __link
				, arg::LinkDriver(source_driver.argument())
			#endif
				);

	for(auto entry: source_contents){
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
					arg::SourceFilePath(entry_path)
			#if defined __link
					, arg::LinkDriver(source_driver.argument())
			#endif
					);

		if( info.is_directory() ){
			//copy recursively

			if( Dir::create(
					 arg::DestinationDirectoryPath(destination_entry_path),
					 Permissions::all_access(),
					 arg::IsRecursive(true)
		 #if defined __link
					 , arg::LinkDriver(destination_driver.argument())
		 #endif
					 ) < 0 ){
				return -1;
			}

			copy(arg::SourceDirectoryPath(entry_path),
				  arg::DestinationDirectoryPath(destination_entry_path)
	  #if defined __link
				  , source_driver,
				  destination_driver
	  #endif
				  );

		} else if( info.is_file() ){
			if( File::copy(
					 arg::SourceFilePath(entry_path),
					 arg::DestinationFilePath(destination_entry_path),
					 arg::IsOverwrite(true)
					 ) < 0 ){
				return -1;
			}
		}
	}
	return 0;
}

#if defined __link
int Dir::create(
		const DestinationDirectoryPath & path,
		const Permissions & permissions,
		arg::LinkDriver driver
		){
	int result;
	if( driver.argument() ){
		result = link_mkdir(
					driver.argument(),
					path.argument().cstring(),
					permissions.permissions()
					);
	} else {
		//open a directory on the local system (not over link)
#if defined __win32
		result = mkdir(path.argument().cstring());
#else
		result = mkdir(path.argument().cstring(), permissions.permissions());
#endif
	}
	return result;
}
#else
int Dir::create(
		const DestinationDirectoryPath & path,
		const Permissions & permissions
		){
	return mkdir(
				path.argument().cstring(),
				permissions.permissions()
				);
}
#endif

#if defined __link
int Dir::create(
		const DestinationDirectoryPath & path,
		const Permissions & permissions,
		const arg::IsRecursive is_recursive,
		arg::LinkDriver driver
		){
	if( is_recursive.argument() == false ){
		return create(path, permissions, driver);
	}
#else
int Dir::create(
		const DestinationDirectoryPath & path,
		const Permissions & permissions,
		const arg::IsRecursive is_recursive){
	if( is_recursive.argument() == false ){
		return create(path, permissions);
	}
#endif
	var::Tokenizer path_tokens(
				arg::TokenEncodedString(path.argument()),
				arg::TokenDelimeters("/")
				);
	var::String base_path;

	if( path.argument().find(
			 arg::StringToFind("/")
			 ) == 0 ){
		base_path << "/";
	}

	for(u32 i=0; i < path_tokens.count(); i++){
		base_path << path_tokens.at(i);
		create(DestinationDirectoryPath(base_path),
					permissions
			#if defined __link
					, driver
			#endif
					);
		base_path << "/";
	}


	return 0;
}



#if defined __link
bool Dir::exists(
		const SourceDirectoryPath & path,
		arg::LinkDriver driver
		){
	Dir d(driver);
#else
bool Dir::exists(
		const SourceDirectoryPath & path
		){
	Dir d;
#endif
	if( d.open(path) < 0 ){ return false; }
	d.close();
	return true;
}


int Dir::open(const SourceDirectoryPath & name){
#if defined __link
	if( driver() ){
		m_dirp = link_opendir(driver(), name.argument().cstring());
	} else {
		//open a directory on the local system (not over link)

		m_dirp_local = opendir(name.argument().cstring());
		if( m_dirp_local == 0 ){
			return -1;
		}
		m_path.assign(name.argument());
		return 0;
	}
#else
	m_dirp = opendir(name.argument().cstring());
#endif

	if( m_dirp == 0 ){
		set_error_number_to_errno();
		return -1;
	}

	m_path.assign(name.argument());
	return 0;
}


#if !defined __link
int Dir::count(){
	long loc;
	int count;

	if( !is_open() ){
		return -1;
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

	seek(arg::Location(loc));

	return count;

}

#endif


#if defined __link
var::Vector<var::String> Dir::read_list(
		const SourceDirectoryPath & path,
		arg::IsRecursive is_recursive,
		arg::LinkDriver driver
		){
	Dir directory(driver);
#else
var::Vector<var::String> Dir::read_list(
		const SourceDirectoryPath & path,
		arg::IsRecursive is_recursive
		){
	Dir directory;
#endif
	var::Vector<var::String> result;
	if( directory.open(path) < 0 ){ return result; }
	result = directory.read_list(is_recursive);
	directory.close();
	return result;
}


var::Vector<var::String> Dir::read_list(
		const IsRecursive is_recursive
		){
	var::Vector<var::String> result;
	var::String entry;

	do {
		entry.clear();
		entry = read();
		if( !entry.is_empty() &&
			 (entry != ".") &&
			 (entry != "..") ){

			if( is_recursive.argument() ){
				var::String entry_path;
				entry_path << m_path << "/" << entry;
				FileInfo info = File::get_info(
							arg::SourceFilePath(entry_path)
			#if defined __link
							, arg::LinkDriver(driver())
			#endif
							);

				if( info.is_directory() ){

					var::Vector<var::String> intermediate_result =
							Dir::read_list(
								arg::SourceDirectoryPath(entry_path),
								is_recursive
			#if defined __link
								, arg::LinkDriver(driver())
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
	} while( entry.is_empty() == false );

	return result;
}

const char * Dir::read(){

#if defined __link
	if( driver() ){
		struct link_dirent * result;
		memset(&m_entry, 0, sizeof(m_entry));
		if( link_readdir_r(driver(), m_dirp, &m_entry, &result) < 0 ){
			return 0;
		}
	} else {
		struct dirent * result_local;
		if( (readdir_r(m_dirp_local, &m_entry_local, &result_local) < 0) || (result_local == 0) ){
			return 0;
		}
		return m_entry_local.d_name;
	}
#else
	struct dirent * result;
	if( readdir_r(m_dirp, &m_entry, &result) < 0 ){
		set_error_number_to_errno();
		return 0;
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
#else //__link
		if( closedir(m_dirp) < 0 ){
			set_error_number_to_errno();
		}
#endif
		m_dirp = 0;
	}

#if defined __link
	if( m_dirp_local ){
		DIR * dirp_copy = m_dirp_local;
		m_dirp_local = 0;
		if( closedir(dirp_copy) < 0 ){
			return -1;
		}
		return 0;
	}
#endif
	return 0;
}

