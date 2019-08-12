//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include <stdlib.h>

#include "calc/Rle.hpp"
#include "fs/File.hpp"
#include "sgfx/Bitmap.hpp"
#include "sgfx/Cursor.hpp"

using namespace sgfx;
using namespace sys;
using namespace calc;

void Palette::fill_gradient_argb8888(sg_color_t color){
	sg_color_t gradient_color;
	u16 alpha;
	set_color(0, 0);
	for(u32 i=1; i < m_colors.count() - 1; i++){
		alpha = i * 255 / m_colors.count();
		gradient_color = (alpha << 24) | color;
		set_color(i, gradient_color);
	}
	set_color(m_colors.count() - 1, 0xff000000 | color);
}

void Palette::fill_gradient_gray(bool is_ascending){
	sg_color_t gradient_color;
	u8 component;
	if( is_ascending ){
		set_color(0, 0xff000000);
		for(u32 i=1; i < m_colors.count() - 1; i++){
			component = i * 255 / m_colors.count();
			gradient_color = (0xff << 24) | (component << 16) | (component << 8) | (component);
			set_color(i, gradient_color);
		}
		set_color(m_colors.count() - 1, 0xffffffff);
	} else {
		set_color(0, 0xffffffff);
		for(u32 i=1; i < m_colors.count() - 1; i++){
			component = 255 - (i * 255 / m_colors.count());
			gradient_color = (0xff << 24) | (component << 16) | (component << 8) | (component);
			set_color(i, gradient_color);
		}
		set_color(m_colors.count() - 1, 0xff000000);
	}
}


Region Bitmap::get_viewable_region() const {
	Point point(margin_left(), margin_top());
	Area dim(width() - margin_left() - margin_right(), height() - margin_top() - margin_bottom());
	Region region(point, dim);
	return region;
}

void Bitmap::calculate_members(const Area & dim){
	//we need to grab the read only in case the Data object is read only
	api()->bmap_set_data(&m_bmap, (sg_bmap_data_t*)read_only_data(), dim, m_bmap.bits_per_pixel);
}

int Bitmap::set_bits_per_pixel(u8 bits_per_pixel){
	if( api()->bits_per_pixel == 0 ){
		switch(bits_per_pixel){
			case 1:
			case 2:
			case 4:
			case 8:
			case 16:
			case 32:
				m_bmap.bits_per_pixel = bits_per_pixel;
				allocate(area());
				return 0;
		}
	}
	return -1;
}

#include "sys/requests.h"
extern "C" const void * kernel_request_api(u32 request);

void Bitmap::initialize_members(){

	//ensure api() is valid
	if( api().is_valid() == false ){
		exit_fatal("sgfx api not available");
	}

	if( api()->bits_per_pixel == 0 ){
		m_bmap.bits_per_pixel = 1;
	} else {
		m_bmap.bits_per_pixel = api()->bits_per_pixel;
	}
	m_bmap.margin_bottom_right.area = 0;
	m_bmap.margin_top_left.area = 0;
	m_bmap.pen.thickness = 1;
	m_bmap.pen.o_flags = 0;
	m_bmap.pen.color = 65535;
}

void Bitmap::set_data(sg_bmap_data_t * mem, sg_size_t w, sg_size_t h, bool readonly){
	Data::refer_to(
				arg::DestinationBuffer(mem),
				arg::Size(calc_size(w,h)),
				arg::IsReadOnly(readonly));
	calculate_members(Area(w,h));
}

void Bitmap::set_data(const sg_bmap_header_t * hdr, bool readonly){
	char * ptr;
	ptr = (char*)hdr;
	ptr += sizeof(sg_bmap_header_t);
	Data::refer_to(
				arg::DestinationBuffer(ptr),
				arg::Size(calc_size(hdr->width, hdr->height)),
				arg::IsReadOnly(readonly)
				);
	calculate_members(Area(hdr->width, hdr->height));
}

int Bitmap::allocate(const Area & dim){
	if( Data::allocate(
			 arg::Size(calculate_size(dim))
			 ) < 0 ){
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
	calculate_members(Area());
}

Bitmap::Bitmap(sg_size_t w, sg_size_t h){
	initialize_members();
	allocate(Area(w,h));
}

Bitmap::Bitmap(sg_area_t d){
	initialize_members();
	allocate(d);
}


Bitmap::Bitmap(sg_bmap_data_t * mem, sg_size_t w, sg_size_t h, bool readonly){
	initialize_members();
	set_data(mem, w, h, readonly);
}

Bitmap::Bitmap(const sg_bmap_header_t * hdr, bool readonly){
	initialize_members();
	set_data(hdr, readonly);
}

Bitmap::Bitmap(const Area & area, u8 bits_per_pixel){
	initialize_members();
	if( api()->bits_per_pixel == 0 ){
		switch(bits_per_pixel){
			case 1:
			case 2:
			case 4:
			case 8:
			case 16:
			case 32:
				m_bmap.bits_per_pixel = bits_per_pixel;
		}
	}
	allocate(area);
}

Bitmap::~Bitmap(){
	free();
}

Point Bitmap::center() const{
	return Point(width()/2, height()/2);
}

bool Bitmap::set_size(sg_size_t w, sg_size_t h, sg_size_t offset){
	u32 size = calc_size(w,h);
	if( size <= capacity() ){
		Data::set_size(size);
		api()->bmap_set_data(&m_bmap, to<sg_bmap_data_t>(), sg_dim(w,h), m_bmap.bits_per_pixel);
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



int Bitmap::load(const arg::SourceFilePath & path){
	sg_bmap_header_t hdr;
	fs::File f;
	void * src;

	if( f.open(
			 arg::FilePath(path.argument()),
			 fs::OpenFlags::read_only()
			 ) < 0 ){
		return -1;
	}

	if( f.read(
			 arg::DestinationBuffer(&hdr),
			 arg::Size(sizeof(hdr))
			 ) != sizeof(hdr) ){
		return -1;
	}

	if( (hdr.version != api()->version) || (hdr.bits_per_pixel != api()->bits_per_pixel) ){
		return -1;
	}

	if( set_size(hdr.width, hdr.height) == false ){
		//couln't resize using existing memory -- try re-allocating
		if( alloc(hdr.width, hdr.height) < 0 ){
			return -1;
		}
	}

	src = data();


	if( f.read(
			 arg::DestinationBuffer(src),
			 arg::Size(hdr.size)
			 ) != (s32)hdr.size ){
		return -1;
	}

	return 0;
}


Area Bitmap::load_dim(const arg::SourceFilePath & path){
	sg_bmap_header_t hdr;
	fs::File f;
	if( f.open(
			 arg::FilePath(path.argument()),
			 fs::OpenFlags::read_only()) < 0 ){
		return Area();
	}

	if( f.read(
			 arg::DestinationBuffer(&hdr),
			 arg::Size(sizeof(hdr))
			 ) != sizeof(hdr) ){
		return Area();
	}

	if( (hdr.version != api()->version) || (hdr.bits_per_pixel != api()->bits_per_pixel) ){
		return Area(0,0);
	}

	return Area(hdr.width, hdr.height);
}

int Bitmap::save(const arg::DestinationFilePath & path) const{
	sg_bmap_header_t hdr;

	hdr.width = width();
	hdr.height = height();
	hdr.size = calculate_size();
	hdr.bits_per_pixel = api()->bits_per_pixel;
	hdr.version = api()->version;

	fs::File f;
	if( f.create(
			 arg::DestinationFilePath(path),
			 arg::IsOverwrite(true)
			 ) < 0 ){
		return -1;
	}

	if( f.write(
			 arg::SourceBuffer(&hdr),
			 arg::Size(sizeof(hdr))
			 ) < 0 ){
		f.close();
		fs::File::remove(
					arg::SourceFilePath(path.argument())
					);
		return -1;
	}

	if( f.write(
			 arg::SourceBuffer(data()),
			 arg::Size(hdr.size)
			 ) != (s32)hdr.size ){
		f.close();
		fs::File::remove(
					arg::SourceFilePath(path.argument())
					);
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

	for(point.y = 0; point.y < height(); point.y++){
		for(point.x = 0; point.x < width(); point.x++){

			if( get_pixel(point) ){

				if( point.x < top_left.x ){
					top_left.x = point.x;
				}

				if( point.x > bottom_right.x ){
					bottom_right.x = point.x;
				}

				if( point.y < top_left.y ){
					top_left.y = point.y;
				}

				if( point.y > bottom_right.y ){
					bottom_right.y = point.y;
				}
			}
		}
	}

	result.set_region(top_left, bottom_right);

	return result;
}

bool Bitmap::is_empty(const Region & region) const {
	Cursor x_cursor;
	Cursor y_cursor;
	y_cursor.set(*this, region.point());
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

void Bitmap::downsample_bitmap(const Bitmap & source, const Area & factor){
	Cursor cursor_x, cursor_y;

	if( factor.width() == 0 ){ return; }
	if( factor.height() == 0 ){ return; }

	if( factor.width() > source.width() ){ return; }
	if( factor.height() > source.height() ){ return; }


	Bitmap sample(factor);

	cursor_y.set(*this, Point(0,0));

	for(sg_int_t y = 0; y <= source.height() - factor.height()/2; y+=factor.height()){

		cursor_x = cursor_y;

		for(sg_int_t x = 0; x <= source.width() - factor.width()/2; x+=factor.width()){
			Region region(Point(x,y), factor);
			sample.clear();
			sample.draw_sub_bitmap(Point(0,0), source, region);

			u32 color = sample.calculate_color_sum();
			if( color >= factor.calculate_area()/2 ){
				bmap()->pen.color = 1;
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
	cursor_y.set(*this, Point(0,0));
	for(sg_size_t y = 0; y < height(); y++){
		cursor_x = cursor_y;
		for(sg_size_t x = 0; x < width(); x++){
			color += cursor_x.get_pixel();
		}
		cursor_y.increment_y();
	}
	return color;
}






