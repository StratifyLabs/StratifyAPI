#include "sgfx/IconFont.hpp"

using namespace sgfx;

IconFont::IconFont(const fs::File & file) : m_file(file){
	refresh();
}

size_t IconFont::find(const var::String name) const {
	for(size_t i=0; i < m_list.count(); i++){
		if( this->get_info(i).name() == name ){
			return i;
		}
	}
	return m_list.count();
}

int IconFont::refresh(){
	m_list.clear();
	m_list.shrink_to_fit();
	m_file.seek(0);
	if( m_file.read(m_header) != sizeof(m_header) ){
		return -1;
	}

	for(u32 i=0; i < m_header.icon_count; i++){
		sg_font_icon_t icon;
		if( m_file.read(icon) != sizeof(icon) ){
			m_list.clear();
			m_list.shrink_to_fit();
			return -1;
		}
		m_list.push_back(icon);
	}

	m_master_canvas_idx = -1;
	m_master_canvas.allocate(
				Area(
					m_header.canvas_width, m_header.canvas_height
					),
				Bitmap::BitsPerPixel(m_header.bits_per_pixel)
				);

	return 0;
}

int IconFont::draw(
		size_t offset,
		Bitmap & dest,
		const Point & point
		) const {

	if( m_master_canvas.to_void() == nullptr ){
		return -1;
	}

	if( offset >= m_list.count() ){
		return -1;
	}

	const sg_font_icon_t & icon = m_list.at(offset);

	if( icon.canvas_idx != m_master_canvas_idx ){
		u32 offset = m_header.size + icon.canvas_idx*m_master_canvas.size();

		if( m_file.read(
				 fs::File::Location(offset),
				 m_master_canvas.to_void(),
				 fs::File::Size(m_master_canvas.size())
				 ) != (int)m_master_canvas.size() ){
			return -1;
		}
		m_master_canvas_idx = icon.canvas_idx;
	}

	dest.draw_sub_bitmap(
				point,
				m_master_canvas,
				Region(
					Point(icon.canvas_x ,icon.canvas_y),
					Area(icon.width, icon.height)
					)
				);

	return 0;
}

int IconFont::draw(
		const var::String & name,
		Bitmap & dest,
		const Point & point
		) const {
	size_t offset = find(name);
	return draw(offset, dest, point);
}
