#include "fmt/Svic.hpp"

using namespace fmt;

Svic::Svic(const var::ConstString & path){
	open_readonly(path);
	parse_icons();
	m_current_icon_at = (u32)-1;
}


int Svic::parse_icons(){

	if( fileno() >= 0 ){

		int cursor = seek(0, sys::File::CURRENT);

		sg_vector_icon_header_t header;
		while( read(&header, sizeof(header)) == sizeof(header) ){
			if( m_icons.push_back(header) < 0 ){
				set_error_number(m_icons.error_number());
				return -1;
			}
			seek(header.count * sizeof(sg_vector_path_description_t), sys::File::CURRENT);
		}

		seek(cursor, sys::File::SET);
	}

	return 0;

}

var::String Svic::name_at(u32 i) const {
	if(i < count() ){
		return var::String(m_icons.at(i).name);
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
		m_current_icon.resize(m_icons.at(i).count);
		int result;
		if( (result = read(m_icons.at(i).list_offset, m_current_icon)) < 0 ){
			return sgfx::VectorPath();
		}
		m_current_icon_at = i;
	}
	vector_path << m_current_icon;
	return vector_path;

}

int Svic::append(const var::ConstString & name, const var::Vector<sg_vector_path_description_t> & list){
	sg_vector_icon_header_t header;
	var::Data list_data;
	memset(&header, 0, sizeof(header));
	strncpy(header.name, name.cstring(), 23);
	header.count = list.count();
	header.list_offset = seek(0, sys::File::CURRENT) + sizeof(header);
	if( write(&header, sizeof(header)) != sizeof(header) ){
		set_error_number( error_number() );
		return -1;
	}
	list_data.refer_to((void*)list.to_void(), list.size(), true);
	if( write(list_data) != (int)list.size() ){
		set_error_number( error_number() );
		return -1;
	}

	return 0;
}
