//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include <stdlib.h>

#include "calc/Rle.hpp"
#include "fs/File.hpp"
#include "sgfx/Bitmap.hpp"
#include "sgfx/Cursor.hpp"

using namespace sgfx;
using namespace sys;
using namespace calc;

int AntiAliasFilter::initialize(
      var::Array<u8, 8> contrast_map
      ){
   return Bitmap::api()->antialias_filter_init(
            &m_filter,
            contrast_map.data()
            );
}

Palette & Palette::set_bits_per_pixel(u8 bits_per_pixel){
	u32 count = 1 << bits_per_pixel;
	if( count > 32 ){
		m_colors.clear();
		return *this;
	}
	m_colors.resize(count);
	m_palette.mask = (count-1);
	m_palette.colors = static_cast<sg_bmap_data_t*>(m_colors.data());
	return *this;
}

Palette & Palette::fill_gradient_argb8888(sg_color_t color){
	sg_color_t gradient_color;
	u16 alpha;
	set_color(Position(0), 0);
	for(u32 i=1; i < m_colors.count() - 1; i++){
		alpha = i * 255 / m_colors.count();
		gradient_color = (alpha << 24) | color;
		set_color(Position(i), gradient_color);
	}
	set_color(Position(m_colors.count() - 1), 0xff000000 | color);
	return *this;
}

Palette & Palette::fill_gradient_gray(
		IsAscending is_ascending
		){
	sg_color_t gradient_color;
	u8 component;
	if( is_ascending.argument() ){
		set_color(Position(0), 0xff000000);
		for(u32 i=1; i < m_colors.count() - 1; i++){
			component = i * 255 / m_colors.count();
			gradient_color = (0xff << 24) | (component << 16) | (component << 8) | (component);
			set_color(Position(i), gradient_color);
		}
		set_color(Position(m_colors.count() - 1), 0xffffffff);
	} else {
		set_color(Position(0), 0xffffffff);
		for(u32 i=1; i < m_colors.count() - 1; i++){
			component = 255 - (i * 255 / m_colors.count());
			gradient_color = (0xff << 24) | (component << 16) | (component << 8) | (component);
			set_color(Position(i), gradient_color);
		}
		set_color(Position(m_colors.count() - 1), 0xff000000);
	}
	return *this;
}


Region Bitmap::get_viewable_region() const {
	Point point = Point(
				margin_left(), margin_top()
				);
	Area dim = Area(
				Area::Width(width() - margin_left() - margin_right()),
				Area::Height(height() - margin_top() - margin_bottom())
				);
	Region region(point, dim);
	return region;
}

void Bitmap::calculate_members(const Area & dim){
	//we need to grab the read only in case the Data object is read only
	api()->bmap_set_data(
				&m_bmap,
				(sg_bmap_data_t*)((const sg_bmap_data_t*)to<sg_bmap_data_t>()),
				dim,
				m_bmap.bits_per_pixel
				);
}

int Bitmap::set_internal_bits_per_pixel(u8 bpp){
	//api bpp of zero means the api supports variable bpp values
	if( api()->bits_per_pixel == 0 ){
		switch(bpp){
			case 1:
			case 2:
			case 4:
			case 8:
			case 16:
			case 32:
				m_bmap.bits_per_pixel = bpp;
				return 0;
		}
	} else {
		//bpp is fixed by the sgfx library build
		m_bmap.bits_per_pixel = api()->bits_per_pixel;
	}
	return -1;
}


int Bitmap::set_bits_per_pixel(u8 bits_per_pixel){
	return allocate(area(), BitsPerPixel(bits_per_pixel));
}

void Bitmap::initialize_members(){

	//ensure api() is valid
	if( api().is_valid() == false ){
		exit_fatal("sgfx api not available");
	}

	if( api()->bits_per_pixel == 0 ){
		m_bmap.bits_per_pixel = 1; //use 1 as the default others are supported
	} else {
		m_bmap.bits_per_pixel = api()->bits_per_pixel;
	}
	m_bmap.margin_bottom_right.area = 0;
	m_bmap.margin_top_left.area = 0;
	m_bmap.pen.thickness = 1;
	m_bmap.pen.o_flags = 0;
	m_bmap.pen.color = 65535;
}

void Bitmap::refer_to(
		ReadOnlyBuffer buffer,
		const Area & area,
		BitsPerPixel bpp
		){
	set_internal_bits_per_pixel(bpp.argument());

	Data::refer_to(
				buffer,
				Size(calculate_size(area))
				);

	calculate_members(area);
}

void Bitmap::refer_to(
		ReadWriteBuffer buffer,
		const Area & area,
		BitsPerPixel bpp){
	set_internal_bits_per_pixel(bpp.argument());
	Data::refer_to(
				buffer,
				Size(calculate_size(area))
				);
	calculate_members(area);

}

void Bitmap::refer_to(
		const sg_bmap_header_t * hdr,
		IsReadOnly is_read_only
		){
	char * ptr;
	ptr = (char*)hdr;
	ptr += sizeof(sg_bmap_header_t);

	if( is_read_only.argument() ){
		refer_to(
					ReadOnlyBuffer(ptr),
					Area(hdr->width, hdr->height),
					BitsPerPixel(hdr->bits_per_pixel)
					);
	} else {
		refer_to(
					ReadWriteBuffer(ptr),
					Area(hdr->width ,hdr->height),
					BitsPerPixel(hdr->bits_per_pixel)
					);
	}
}

Bitmap Bitmap::create_reference(
		const Region & region
		){
	Bitmap result;
	result.m_bmap.bits_per_pixel = bits_per_pixel();

	if( is_read_only() ){
		result.Data::refer_to(
					Reference::ReadOnlyBuffer(bmap_data(region.point())),
					Size(result.calculate_size(region.area()))
					);
	} else {
		result.Data::refer_to(
					Reference::ReadWriteBuffer(bmap_data(region.point())),
					Size(result.calculate_size(region.area()))
					);
	}
	result.calculate_members(region.area());
	return result;
}

int Bitmap::allocate(
		const Area & dim,
		BitsPerPixel bpp
		){
	set_internal_bits_per_pixel(bpp.argument());
	if( Data::allocate(calculate_size(dim) ) < 0 ){
		calculate_members(Area());
		return -1;
	}
	calculate_members(dim);
	return 0;
}

int Bitmap::free(){
	int result = Data::free();
	if( result == 0 ){
		calculate_members(Area());
	}
	return result;
}

Bitmap::Bitmap(){
	initialize_members();
	set_internal_bits_per_pixel(1);
	calculate_members(Area());
}

Bitmap::Bitmap(
		ReadOnlyBuffer buffer,
		const Area & area,
		BitsPerPixel bpp
		){
	initialize_members();
	refer_to(buffer,area,bpp);
}

Bitmap::Bitmap(
		ReadWriteBuffer buffer,
		const Area & area,
		BitsPerPixel bpp
		){
	initialize_members();
	refer_to(buffer,area,bpp);
}


Bitmap::Bitmap(
		const sg_bmap_header_t * hdr,
		IsReadOnly is_read_only
		){
	initialize_members();
	refer_to(hdr, is_read_only);
}

Bitmap::Bitmap(
		const Area & area,
		BitsPerPixel bits_per_pixel
		){
	initialize_members();
	allocate(area, bits_per_pixel);
}

Bitmap::~Bitmap(){
	free();
}

Point Bitmap::center() const{
	return Point(width()/2,height()/2);
}

bool Bitmap::resize(const Area & area){
	u32 size = calculate_size(area);
	if( size <= capacity() ){
		Data::resize(size);
		api()->bmap_set_data(&m_bmap, to<sg_bmap_data_t>(),
									area,
									m_bmap.bits_per_pixel
									);
		return true;
	}
	return false;
}

const sg_bmap_data_t * Bitmap::bmap_data(const Point & p) const {
	const sg_bmap_data_t * data = to<sg_bmap_data_t>();
	if( data == 0 ){ return 0; }
	return api()->bmap_data(bmap(), p);
}

sg_bmap_data_t * Bitmap::bmap_data(const Point & p){
	sg_bmap_data_t * data = to<sg_bmap_data_t>();
	if( data == 0 ){ return 0; }
	return api()->bmap_data(bmap(), p);
}



int Bitmap::load(const var::String & path){
	sg_bmap_header_t hdr;
	fs::File f;

	if( f.open(
			 path,
			 fs::OpenFlags::read_only()
			 ) < 0 ){
		return -1;
	}

	if( f.read(
			 &hdr,
			 fs::File::Size(sizeof(hdr))
			 ) != sizeof(hdr) ){
		return -1;
	}

	if( (hdr.version != api()->version) || (hdr.bits_per_pixel != api()->bits_per_pixel) ){
		return -1;
	}

	if( resize(
			 Area(hdr.width, hdr.height)
			 ) == false ){
		return -1;
	}

	if( f.read(
			 to_void(),
			 Size(hdr.size)
			 ) != (s32)hdr.size ){
		return -1;
	}

	return 0;
}


Area Bitmap::load_area(
		const var::String & path
		){
	sg_bmap_header_t hdr;
	fs::File f;
	if( f.open(
			 path,
			 fs::OpenFlags::read_only()) < 0 ){
		return Area();
	}

	if( f.read(
			 &hdr,
			 fs::File::Size(sizeof(hdr))
			 ) != sizeof(hdr) ){
		return Area();
	}

	if( (hdr.version != api()->version) || (hdr.bits_per_pixel != api()->bits_per_pixel) ){
		return Area();
	}

	return Area(hdr.width, hdr.height);
}

int Bitmap::save(const var::String & path) const{
	sg_bmap_header_t hdr;

	hdr.width = width();
	hdr.height = height();
	hdr.size = calculate_size();
	hdr.bits_per_pixel = api()->bits_per_pixel;
	hdr.version = api()->version;

	fs::File f;
	if( f.create(
			 path,
			 fs::File::IsOverwrite(true)
			 ) < 0 ){
		return -1;
	}

	if( f.write(
			 &hdr,
			 fs::File::Size(sizeof(hdr))
			 ) < 0 ){
		f.close();
		fs::File::remove(path);
		return -1;
	}

	if( f.write(
			 to_const_void(),
			 fs::File::Size(hdr.size)
			 ) != (s32)hdr.size ){
		f.close();
		fs::File::remove(path);
		return -1;
	}

	if( f.close() < 0 ){
		return -1;
	}

	return 0;
}

Region Bitmap::calculate_active_region() const {
	Region result;
	sg_point_t point;
	sg_point_t top_left;
	sg_point_t bottom_right;

	top_left.x = width();
	top_left.y = height();
	bottom_right.x = 0;
	bottom_right.y = 0;
	bool is_blank = true;

	for(point.y = 0; point.y < height(); point.y++){
		bool is_row_blank = true;
		for(point.x = 0; point.x < width(); point.x++){

			if( get_pixel(point) ){
				is_blank = false;
				is_row_blank = false;

				if( point.x < top_left.x ){
					top_left.x = point.x;
				}

				if( point.x > bottom_right.x ){
					bottom_right.x = point.x;
				}
			}
		}

		if( !is_row_blank ){
			if( point.y < top_left.y ){
				top_left.y = point.y;
			}

			if( point.y > bottom_right.y ){
				bottom_right.y = point.y;
			}
		}
	}

	if( is_blank ){
		top_left.x = width()/2;
		top_left.y = width()/2;
		bottom_right.x = width()/2;
		bottom_right.y = width()/2;
	}

	result.set_region(top_left, bottom_right);

	return result;
}

bool Bitmap::is_empty(const Region & region) const {
	Cursor x_cursor;
	Cursor y_cursor(*this, region.point());
	for(u32 h = 0; h < region.area().height(); h++){
		x_cursor = y_cursor;
		for(u32 w = 0; w < region.area().width(); w++){
			//get pixel increments the cursor
			if( x_cursor.get_pixel() != 0 ){
				return false;
			}
		}
		y_cursor.increment_y();
	}
	return true;
}

void Bitmap::downsample_bitmap(
		const Bitmap & source,
		const Area & factor
		){

	Cursor cursor_x, cursor_y;

	if( factor.width() == 0 ){ return; }
	if( factor.height() == 0 ){ return; }

	if( factor.width() > source.width() ){ return; }
	if( factor.height() > source.height() ){ return; }


	Bitmap sample(
				factor,
				BitsPerPixel(bits_per_pixel())
				);

	cursor_y.set_bitmap(*this);

	for(sg_int_t y = 0;
		 y <= source.height() - factor.height()/2;
		 y += factor.height()){

		cursor_x = cursor_y;

		for(sg_int_t x = 0;
			 x <= source.width() - factor.width()/2;
			 x+=factor.width()){

			Region region(
						Point(x, y),
						factor
						);

			sample.clear();
			sample.draw_sub_bitmap(Point(), source, region);

			u32 color = sample.calculate_color_sum();
			if( color >= factor.calculate_area()/2 ){
				bmap()->pen.color = (u32)-1;
			} else {
				bmap()->pen.color = 0;
			}

			cursor_x.draw_pixel();
		}

		cursor_y.increment_y();
	}

}

sg_color_t Bitmap::calculate_color_sum(){
	sg_color_t color = 0;
	Cursor cursor_y, cursor_x;
	cursor_y.set_bitmap(*this);
	for(sg_size_t y = 0; y < height(); y++){
		cursor_x = cursor_y;
		for(sg_size_t x = 0; x < width(); x++){
			color += cursor_x.get_pixel();
		}
		cursor_y.increment_y();
	}
	return color;
}






