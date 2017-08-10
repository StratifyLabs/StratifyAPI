//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <stdlib.h>

#include "calc/Rle.hpp"
#include "sys/File.hpp"
#include "sgfx/Bitmap.hpp"

using namespace sgfx;
using namespace sys;
using namespace calc;

void Bitmap::calc_members(sg_size_t w, sg_size_t h){
	sg_api()->bmap_set_data(&m_bmap, (sg_bmap_data_t*)data_const(), sg_dim(w,h));
}

void Bitmap::init_members(){
	m_bmap.margin_bottom_right.dim = 0;
	m_bmap.margin_top_left.dim = 0;
	m_bmap.pen.thickness = 1;
	m_bmap.pen.o_flags = 0;
	m_bmap.pen.color = 65535;
}


void Bitmap::set_data(sg_bmap_data_t * mem, sg_size_t w, sg_size_t h, bool readonly){

	Data::set(mem, calc_size(w,h), readonly);
	calc_members(w,h);
}

void Bitmap::set_data(sg_bitmap_hdr_t * hdr, bool readonly){
	char * ptr;
	ptr = (char*)hdr;
	ptr += sizeof(sg_bitmap_hdr_t);
	Data::set(ptr, calc_size(hdr->w, hdr->h), readonly);
	calc_members(hdr->w, hdr->h);
}

int Bitmap::alloc(sg_size_t w, sg_size_t h){
	if( Data::alloc(calc_size(w,h)) < 0 ){
		return -1;
	}
	calc_members(w,h);
	return 0;
}

void Bitmap::free(){
	if( Data::free() == 0 ){
		calc_members(0, 0);
	}
}

Bitmap::Bitmap(){
	init_members();
	calc_members(0, 0);
}

Bitmap::Bitmap(sg_size_t w, sg_size_t h){
	init_members();
	alloc(w,h);
}

Bitmap::Bitmap(sg_dim_t d){
	init_members();
	alloc(d.width,d.height);
}


Bitmap::Bitmap(sg_bmap_data_t * mem, sg_size_t w, sg_size_t h, bool readonly){
	init_members();
	set_data(mem, w, h, readonly);
}

Bitmap::Bitmap(sg_bitmap_hdr_t * hdr, bool readonly){
	init_members();
	set_data(hdr, readonly);
}

Bitmap::~Bitmap(){
	free();
}

sg_point_t Bitmap::calc_center() const{
	sg_point_t p;
	p.x = width()/2;
	p.y = height()/2;
	return p;
}

bool Bitmap::set_size(sg_size_t w, sg_size_t h, sg_size_t offset){
	if( calc_size(w,h) <= capacity() ){
		sg_api()->bmap_set_data(&m_bmap, (sg_bmap_data_t*)data_const(), sg_dim(w,h));
		return true;
	}
	return false;
}

sg_bmap_data_t * Bitmap::data(sg_point_t p) const {

	if( data() == 0 ){
		return 0;
	}

	return sg_api()->bmap_data(bmap_const(),p);
}

sg_bmap_data_t * Bitmap::data(sg_int_t x, sg_int_t y) const{
	return sg_api()->bmap_data(bmap_const(), sg_point(x,y));
}

const sg_bmap_data_t * Bitmap::data_const(sg_point_t p) const {
	if( data_const() == 0 ){
		return 0;
	}

	return data_const() + p.x / 8 + p.y * m_bmap.columns;
}



int Bitmap::load(const char * path){
	sg_bitmap_hdr_t hdr;
	File f;
	void * src;

	if( f.open(path, File::READONLY) < 0 ){
		return -1;
	}

	if( f.read(&hdr, sizeof(hdr)) != sizeof(hdr) ){
		f.close();
		return -1;
	}

	if( (hdr.version != sg_api()->version) || (hdr.bits_per_pixel != sg_api()->bits_per_pixel) ){
		f.close();
		return -1;
	}

	if( set_size(hdr.w, hdr.h) == false ){
		//couln't resize using existing memory -- try resizing
		if( alloc(hdr.w, hdr.h) < 0 ){
			f.close();
			return -1;
		}
	}

	src = data();


	if( f.read(src, hdr.size) != (s32)hdr.size ){
		f.close();
		return -1;
	}

	if( f.close() < 0 ){
		return -1;
	}

	return 0;
}


Dim Bitmap::load_dim(const char * path){
	sg_bitmap_hdr_t hdr;
	File f;
	if( f.open(path, File::READONLY) < 0 ){
		return Dim();
	}

	if( f.read(&hdr, sizeof(hdr)) != sizeof(hdr) ){
		f.close();
		return Dim();
	}

	f.close();

	if( (hdr.version != sg_api()->version) || (hdr.bits_per_pixel != sg_api()->bits_per_pixel) ){
		return Dim(0,0);
	}

	return Dim(hdr.w, hdr.h);
}

int Bitmap::save(const char * path) const{
	sg_bitmap_hdr_t hdr;

	hdr.w = width();
	hdr.h = height();
	hdr.size = calc_size(width(), height());
	hdr.bits_per_pixel = sg_api()->bits_per_pixel;
	hdr.version = sg_api()->version;

	File f;
	if( f.create(path, true) < 0 ){
		return -1;
	}

	if( f.write(&hdr, sizeof(hdr)) < 0 ){
		f.close();
		unlink(path);
		return -1;
	}

	if( f.write(data(), hdr.size) != (s32)hdr.size ){
		f.close();
		unlink(path);
		return -1;
	}

	if( f.close() < 0 ){
		return -1;
	}

	return 0;
}

void Bitmap::showidth() const{
	//sg_api()->show(bmap_const());
	sg_size_t i,j;

	sg_color_t color;
	sg_cursor_t y_cursor;
	sg_cursor_t x_cursor;

	sg_api()->cursor_set(&y_cursor, bmap_const(), sg_point(0,0));

	for(i=0; i < bmap_const()->dim.height; i++){
		sg_cursor_copy(&x_cursor, &y_cursor);
		for(j=0; j < bmap_const()->dim.width; j++){
			color = sg_api()->cursor_get_pixel(&x_cursor);
			if( sg_api()->bits_per_pixel > 8 ){
				printf("%04X", color);
			} else if(sg_api()->bits_per_pixel > 4){
				printf("%02X", color);
			} else {
				printf("%X", color);
			}
			if( (j < bmap_const()->dim.width - 1) && (sg_api()->bits_per_pixel > 4)){
				printf(" ");
			}
		}
		printf("\n");
		sg_api()->cursor_inc_y(&y_cursor);
	}
}





