#include "fmt/Svic.hpp"

using namespace fmt;

Svic::Svic(const arg::SourceFilePath & path){
	if( path.argument().is_empty() == false ){
		open(
					arg::FilePath(path.argument()),
					fs::OpenFlags::read_only()
					);
		parse_icons();
	}
	m_current_icon_at = (u32)-1;
}


int Svic::parse_icons(){

	if( fileno() >= 0 ){

		int cursor = seek(arg::Location(0), fs::File::CURRENT);

		sg_vector_icon_header_t header;
		while( read(
					 arg::DestinationBuffer(&header),
					 arg::Size(sizeof(header))
					 ) == sizeof(header) ){
			if( m_icons.push_back(header) < 0 ){
				set_error_number(m_icons.error_number());
				return -1;
			}
			seek(
						arg::Location(header.count * sizeof(sg_vector_path_description_t)),
						fs::File::CURRENT
						);
		}

		seek(
					arg::Location(cursor),
					fs::File::SET
					);
	}

	return 0;

}

var::String Svic::name_at(u32 i) const {
	if(i < count() ){
		return var::String(
					m_icons.at(i).name
					);
	}
	return var::String();
}

sgfx::VectorPath Svic::get(const var::ConstString & name) const {
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
		m_current_icon.resize(arg::Count(m_icons.at(i).count));
		int result;
		if( (result = read(
				  arg::Location(m_icons.at(i).list_offset),
				  arg::DestinationData(m_current_icon))
			  ) < 0 ){
			return sgfx::VectorPath();
		}
		m_current_icon_at = i;
	}
	vector_path << m_current_icon;
	return vector_path;

}

int Svic::append(
		const var::ConstString & name,
		const var::Vector<sg_vector_path_description_t> & list
		){
	sg_vector_icon_header_t header;
	var::DataReference list_data;
	memset(&header, 0, sizeof(header));
	strncpy(header.name, name.cstring(), 23);
	header.count = list.count();
	header.list_offset = seek(
				arg::Location(0),
				fs::File::CURRENT
				) + sizeof(header);
	if( write(
			 arg::SourceBuffer(&header),
			 arg::Size(sizeof(header))
			 ) != sizeof(header) ){
		set_error_number( error_number() );
		return -1;
	}

	list_data = list;

	if( write(
			 arg::SourceData(list_data)
			 ) != (int)list.size()
		 ){
		set_error_number( error_number() );
		return -1;
	}

	return 0;
}
