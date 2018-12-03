#include "fmt/Svic.hpp"

using namespace fmt;

Svic::Svic(sys::File & file) : m_file(file){
	parse_icons();
}


int Svic::parse_icons(){

	int cursor = file().seek(0, sys::File::CURRENT);

	sg_vector_path_icon_header_t header;
	while( file().read(&header, sizeof(header)) == sizeof(header) ){
		if( m_icons.push_back(header) < 0 ){
			set_error_number(m_icons.error_number());
			return -1;
		}
		file().seek(header.count * sizeof(sg_vector_path_description_t), sys::File::CURRENT);
	}

	file().seek(cursor, sys::File::SET);

	return 0;

}

var::String Svic::name_at(u32 i){
	if(i < count() ){
		return var::String(m_icons.at(i).name);
	}
	return var::String();
}

sgfx::VectorPath Svic::get(const var::ConstString & name){
	for(u32 i = 0; i < count(); i++){
		if( name == m_icons.at(i).name ){
			return at(i);
		}
	}
	return sgfx::VectorPath();
}

sgfx::VectorPath Svic::at(u32 i){
	sgfx::VectorPath vector_path;
	m_current_icon.resize(m_icons.at(i).count);
	if( file().read(m_icons.at(i).list_offset, m_current_icon) < 0 ){
		return sgfx::VectorPath();
	}
	vector_path << m_current_icon;
	return vector_path;

}

int Svic::append(const var::ConstString & name, const var::Vector<sg_vector_path_description_t> & list){
	sg_vector_path_icon_header_t header;
	var::Data list_data;
	memset(&header, 0, sizeof(header));
	strncpy(header.name, name.cstring(), 23);
	header.count = list.count();
	header.list_offset = file().seek(0, sys::File::CURRENT) + sizeof(header);
	if( file().write(&header, sizeof(header)) != sizeof(header) ){
		set_error_number( file().error_number() );
		return -1;
	}
	list_data.refer_to((void*)list.to_void(), list.size(), true);
	if( file().write(list_data) != list.size() ){
		set_error_number( file().error_number() );
		return -1;
	}

	return 0;
}
