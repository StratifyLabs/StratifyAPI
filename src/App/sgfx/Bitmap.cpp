//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <stdlib.h>

#include "calc/Rle.hpp"
#include "sys/File.hpp"
#include "sgfx/Bitmap.hpp"

using namespace sgfx;
using namespace sys;
using namespace calc;


void Bitmap::calc_members(sg_size_t w, sg_size_t h){
	m_bmap.columns = sg_calc_byte_width(w);
	m_bmap.dim.w = w;
	m_bmap.dim.h = h;
	m_bmap.data = (sg_bmap_data_t*)data_const();
}

void Bitmap::init_members(){
	m_bmap.margin_bottom_right.dim = 0;
	m_bmap.margin_top_left.dim = 0;
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
	alloc(d.w,d.h);
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

int Bitmap::calc_byte_width(int w){
	return (w + 7) >> 3;
}

int Bitmap::calc_word_width(int w){
	return (w + 31) / 32;
}

sg_point_t Bitmap::calc_center() const{
	sg_point_t p;
	p.x = w()/2;
	p.y = h()/2;
	return p;
}

int Bitmap::set_size(sg_size_t w, sg_size_t h, sg_size_t offset){
	if( calc_size(w,h) <= capacity() ){
		m_bmap.dim.w = w;
		m_bmap.dim.h = h;
		m_bmap.columns = sg_calc_byte_width(w);
		return 0;
	}
	return -1;
}

sg_bmap_data_t * Bitmap::data(sg_point_t p) const {

	if( data() == 0 ){
		return 0;
	}

	return sg_data(bmap_const(),p);
}

sg_bmap_data_t * Bitmap::data(sg_int_t x, sg_int_t y) const{
	return sg_data(bmap_const(), sg_point(x,y));
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

	if( set_size(hdr.w, hdr.h) < 0 ){
		//couln't resize using existing memory -- try resizing
		if( alloc(hdr.w, hdr.h) < 0 ){
			f.close();
			return -1;
		}
	}

	src = data();


	if( f.read(src, hdr.size) != (ssize_t)hdr.size ){
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
	return Dim(hdr.w, hdr.h);
}

int Bitmap::load(const char * path, sg_point_t p){
	sg_bitmap_hdr_t hdr;
	File f;
	void * src;
	sg_int_t j;
	size_t w;

	if( f.open(path, File::READONLY) < 0 ){
		return -1;
	}

	if( f.read(&hdr, sizeof(hdr)) != sizeof(hdr) ){
		f.close();
		return -1;
	}

	//see if bitmap will fit

	w = calc_byte_width(hdr.w);
	if( (int)w > (columns() - p.x/8) ){
		w = columns() - p.x/8;
	}

	for(j=0; (j < hdr.h) && (p.y+j < h()); j++){
		src = data(sg_point(p.x,p.y+j));
		if( f.read(src, w) != (int)w ){
			f.close();
			return -1;
		}
	}

	if( f.close() < 0 ){
		return -1;
	}

	return 0;
}

int Bitmap::save(const char * path) const{
	sg_bitmap_hdr_t hdr;

	hdr.w = w();
	hdr.h = h();
	hdr.size = calc_size();

	File f;
	if( f.create(path, true) < 0 ){
		return -1;
	}


	if( f.write(&hdr, sizeof(hdr)) < 0 ){
		f.close();
		unlink(path);
		return -1;
	}

	if( f.write(data(), hdr.size) != (ssize_t)hdr.size ){
		f.close();
		unlink(path);
		return -1;
	}


	if( f.close() < 0 ){
		return -1;
	}

	return 0;
}


int Bitmap::set_bitmap_column(const Bitmap & bitmap, sg_point_t p, sg_int_t col){
	return set_bitmap_column(bitmap, p, col, bitmap.h());
}

int Bitmap::set_bitmap_column(const Bitmap & bitmap, sg_point_t p, sg_int_t col, sg_size_t h){
	sg_point_t i;
	if( data() == 0 ){
		return -1;
	}

	if( col >= bitmap.w() ){
		return 0; //nothing to do
	}

	i.x = col;
	for(i.y=0; i.y < h; i.y++){
		if( bitmap.tst_pixel(i) ){
			set_pixel(p);
		} else {
			clr_pixel(p);
		}
		p.y++;
	}

	return 0;
}

/*
bool Bitmap::tst_pixel(sg_point_t p) const {
	return sg_icon_primitive_tst_pixel(bmap_const(), p) != 0;
}

void Bitmap::set_pixel(sg_point_t p){
	sg_set_pixel(bmap(), p);
}

void Bitmap::inv_pixel(sg_point_t p){
	sg_inv_pixel(bmap(), p);
}
*/

void Bitmap::invert(){
	sg_inv_area(bmap(), sg_point_origin(), dim().dim(), 0xFF);
}

void Bitmap::invert(sg_point_t p, sg_dim_t d, sg_bmap_data_t v){
	sg_inv_area(bmap(), p, d, v);
}

void Bitmap::fill(sg_bmap_data_t v, sg_int_t start, sg_size_t h){
	sg_fill(bmap(), v, start, h);
}

void Bitmap::fill(sg_bmap_data_t v){
	memset(data(), v, calc_size());
}


void Bitmap::set_vline(sg_int_t x, sg_int_t ymin, sg_int_t ymax, sg_size_t thickness){
	sg_set_vline(bmap(),x,ymin,ymax,thickness);
}

void Bitmap::set_hline(sg_int_t xmin, sg_int_t xmax, sg_int_t y, sg_size_t thickness){
	sg_set_hline(bmap(),xmin,xmax,y,thickness);
}

void Bitmap::clr_vline(sg_int_t x, sg_int_t ymin, sg_int_t ymax, sg_size_t thickness){
	sg_clr_vline(bmap(),x,ymin,ymax,thickness);
}

void Bitmap::clr_hline(sg_int_t xmin, sg_int_t xmax, sg_int_t y, sg_size_t thickness){
	sg_clr_hline(bmap(),xmin,xmax,y,thickness);
}

void Bitmap::clr_line(sg_point_t p1, sg_point_t p2, sg_size_t thickness){
	sg_clr_line(bmap(), p1, p2, thickness);
}


void Bitmap::set_line(sg_point_t p1, sg_point_t p2, sg_size_t thickness){
	sg_set_line(bmap(), p1, p2, thickness);
}

void Bitmap::pour(sg_point_t p){
	sg_pour(bmap(), p);
}


void Bitmap::shift_right(int count){
	shift_right(count, h());
}

void Bitmap::shift_right(int count, sg_size_t h){
	//sg_shift_right(bmap(), count, 0, h);
}

void Bitmap::shift_left(int count){
	shift_left(count, h());
}

void Bitmap::shift_left(int count, sg_size_t h){
	//sg_shift_left(bmap(), count, 0, h);
}


void Bitmap::show() const{
	sg_show(bmap_const());
}

void Bitmap::flip_x(){
	sg_flip_x(bmap());
}

void Bitmap::flip_y(){
	sg_flip_y(bmap());
}

void Bitmap::flip_xy(){
	sg_flip_xy(bmap());
}





