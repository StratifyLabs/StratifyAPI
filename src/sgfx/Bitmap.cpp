//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include <stdlib.h>

#include "calc/Rle.hpp"
#include "sys/File.hpp"
#include "sgfx/Bitmap.hpp"
#include "sgfx/Cursor.hpp"

using namespace sgfx;
using namespace sys;
using namespace calc;

Region Bitmap::get_viewable_region() const {
	Point point(margin_left(), margin_top());
	Dim dim(width() - margin_left() - margin_right(), height() - margin_top() - margin_bottom());
	Region region(point, dim);
	return region;
}

void Bitmap::calculate_members(const Dim & dim){
	api()->bmap_set_data(&m_bmap, to<sg_bmap_data_t>(), dim, m_bmap.bits_per_pixel);
}

int Bitmap::set_bits_per_pixel(u8 bits_per_pixel){
	if( api()->bits_per_pixel == 0 ){
		switch(bits_per_pixel){
			case 1:
			case 2:
			case 4:
			case 8:
				m_bmap.bits_per_pixel = bits_per_pixel;
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
	m_bmap.margin_bottom_right.dim = 0;
	m_bmap.margin_top_left.dim = 0;
	m_bmap.pen.thickness = 1;
	m_bmap.pen.o_flags = 0;
	m_bmap.pen.color = 65535;
}

void Bitmap::set_data(sg_bmap_data_t * mem, sg_size_t w, sg_size_t h, bool readonly){
	Data::set(mem, calc_size(w,h), readonly);
	calculate_members(Dim(w,h));
}

void Bitmap::set_data(const sg_bmap_header_t * hdr, bool readonly){
	char * ptr;
	ptr = (char*)hdr;
	ptr += sizeof(sg_bmap_header_t);
	Data::set(ptr, calc_size(hdr->width, hdr->height), readonly);
	calculate_members(Dim(hdr->width, hdr->height));
}

int Bitmap::allocate(const Dim & dim){
	if( Data::alloc( calculate_size(dim) ) < 0 ){
		calculate_members(Dim());
		return -1;
	}
	calculate_members(dim);
	return 0;
}

void Bitmap::free(){
	if( Data::free() == 0 ){
		calculate_members(Dim());
	}
}

Bitmap::Bitmap(){
	initialize_members();
	calculate_members(Dim());
}

Bitmap::Bitmap(sg_size_t w, sg_size_t h){
	initialize_members();
	alloc(w,h);
}

Bitmap::Bitmap(sg_dim_t d){
	initialize_members();
	alloc(d.width,d.height);
}


Bitmap::Bitmap(sg_bmap_data_t * mem, sg_size_t w, sg_size_t h, bool readonly){
	initialize_members();
	set_data(mem, w, h, readonly);
}

Bitmap::Bitmap(const sg_bmap_header_t * hdr, bool readonly){
	initialize_members();
	set_data(hdr, readonly);
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



int Bitmap::load(const var::ConstString & path){
	sg_bmap_header_t hdr;
	File f;
	void * src;

	if( f.open(path, File::READONLY) < 0 ){
		return -1;
	}

	if( f.read(&hdr, sizeof(hdr)) != sizeof(hdr) ){
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


	if( f.read(src, hdr.size) != (s32)hdr.size ){
		return -1;
	}

	return 0;
}


Dim Bitmap::load_dim(const char * path){
	sg_bmap_header_t hdr;
	File f;
	if( f.open(path, File::READONLY) < 0 ){
		return Dim();
	}

	if( f.read(&hdr, sizeof(hdr)) != sizeof(hdr) ){
		return Dim();
	}

	if( (hdr.version != api()->version) || (hdr.bits_per_pixel != api()->bits_per_pixel) ){
		return Dim(0,0);
	}

	return Dim(hdr.width, hdr.height);
}

int Bitmap::save(const var::ConstString & path) const{
	sg_bmap_header_t hdr;

	hdr.width = width();
	hdr.height = height();
	hdr.size = calculate_size();
	hdr.bits_per_pixel = api()->bits_per_pixel;
	hdr.version = api()->version;

	File f;
	if( f.create(path, true) < 0 ){
		return -1;
	}

	if( f.write(&hdr, sizeof(hdr)) < 0 ){
		f.close();
		File::remove(path);
		return -1;
	}

	if( f.write(data(), hdr.size) != (s32)hdr.size ){
		f.close();
		File::remove(path);
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
	for(u32 h = 0; h < region.dim().height(); h++){
		x_cursor = y_cursor;
		for(u32 w = 0; w < region.dim().width(); w++){
			//get pixel increments the cursor
			if( x_cursor.get_pixel() != 0 ){
				return false;
			}
		}
		y_cursor.increment_y();
	}
	return true;
}

void Bitmap::downsample_bitmap(const Bitmap & source, const Dim & factor){
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
			if( color >= factor.area()/2 ){
				set_pen_color(1);
			} else {
				set_pen_color(0);
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


void Bitmap::show() const{
	//api()->show(bmap());
	sg_size_t i,j;

	sg_color_t color;
	sg_cursor_t y_cursor;
	sg_cursor_t x_cursor;

	api()->cursor_set(&y_cursor, bmap(), sg_point(0,0));

	printf("\nshow bitmap %d x %d\n", bmap()->dim.width, bmap()->dim.height);
	for(i=0; i < bmap()->dim.height; i++){
		sg_cursor_copy(&x_cursor, &y_cursor);
		for(j=0; j < bmap()->dim.width; j++){
			color = api()->cursor_get_pixel(&x_cursor);
			if( api()->bits_per_pixel > 8 ){
				::printf("%04X", color);
			} else if(api()->bits_per_pixel > 4){
				::printf("%02X", color);
			} else {
				::printf("%X", color);
			}
			if( (j < bmap()->dim.width - 1) && (api()->bits_per_pixel > 4)){
				::printf(" ");
			}
		}
		::printf("\n");
		api()->cursor_inc_y(&y_cursor);
	}
}





