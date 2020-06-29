/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "fmt/Svic.hpp"

using namespace fmt;

Svic::Svic(const var::String & path){
	m_current_icon_at = static_cast<u32>(-1);
	if( path.is_empty() == false ){
		if( open(path, fs::OpenFlags::read_only()) < 0 ){
			return;
		}
		parse_icons();
	}
}


int Svic::parse_icons(){

	if( fileno() >= 0 ){

		int cursor = seek(Location(0), fs::File::whence_current);

		sg_vector_icon_header_t header;
		while( read(
					 &header,
					 Size(sizeof(header))
					 ) == sizeof(header) ){

			m_icons.push_back(header);

			seek(Location(
					  header.count * sizeof(sg_vector_path_description_t)
					  ),
					fs::File::whence_current);
		}

		seek(Location(cursor),
				fs::File::whence_set);
	}

	return 0;
}

var::String Svic::name_at(u32 i) const {
	if(i < count() ){
		return m_icons.at(i).name;
	}
	return var::String();
}

sgfx::VectorPath Svic::get(const var::String & name) const {
	for(u32 i = 0; i < count(); i++){
		if( name == m_icons.at(i).name ){
			return at(i);
		}
	}
	return sgfx::VectorPath();
}

sgfx::VectorPath Svic::at(u32 i) const {
	sgfx::VectorPath vector_path;
	if( m_current_icon_at != i ){
		m_current_icon.resize(m_icons.at(i).count);
		int result;
		if( (result = read(
				  Location(m_icons.at(i).list_offset),
				  m_current_icon
				  )
			  ) < 0 ){
			return sgfx::VectorPath();
		}
		m_current_icon_at = i;
	}
	vector_path << m_current_icon;
	return vector_path;

}

int Svic::append(
		const var::String & name,
		const var::Vector<sg_vector_path_description_t> & list
		){
	sg_vector_icon_header_t header;
	memset(&header, 0, sizeof(header));
	strncpy(header.name, name.cstring(), 23);
	header.count = list.count();
	header.list_offset = seek(
				Location(0),
				fs::File::whence_current
				) + sizeof(header);
	if( write(
			 &header,
			 Size(sizeof(header))
			 ) != sizeof(header) ){
		return set_error_number_if_error(
					api::error_code_fs_failed_to_write
					);
	}

	if( write(list) != (int)list.size()
		 ){
		return set_error_number_if_error(
					api::error_code_fs_failed_to_write
					);
	}

	return 0;
}
