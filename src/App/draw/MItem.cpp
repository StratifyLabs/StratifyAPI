//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "draw/Mitem.hpp"
using namespace draw;

mitem_size_t MItem::m_scale = 100;

static mg_int_t scale_to_bitmap(mitem_size_t d, mg_size_t s, mitem_size_t max){
	int tmp;
	tmp = (d * s + max/2) / max;
	return tmp;
}

mitem_point_t mitem_point(mitem_int_t x, mitem_int_t y){
	mitem_point_t p;
	p.x = x;
	p.y = y;
	return p;
}

mitem_dim_t mitem_dim(mitem_size_t w, mitem_size_t h){
	mitem_dim_t d;
	d.w = w;
	d.h = h;
	return d;
}

MItemAttr::MItemAttr(){
	memset(&m_attr, 0, sizeof(mitem_attr_t));
}

MItemAttr::MItemAttr(mitem_attr_t & attr){
	m_attr = attr;
}


void MItemAttr::set(MBitmap * b, mitem_point_t p, mitem_dim_t d){
	m_attr.b = b;
	m_attr.p = p;
	m_attr.d = d;
}

mitem_size_t MItemAttr::w(mitem_size_t v) const {
	return m_attr.d.w * v / MItem::scale();
}

mitem_size_t MItemAttr::h(mitem_size_t v) const {
	return m_attr.d.h * v / MItem::scale();
}

MItemAttr MItemAttr::operator+ (mitem_point_t p) const {
	MItemAttr attr;
	attr = *this;
	attr.attr().p.x += w(p.x);
	attr.attr().p.y += h(p.y);
	return attr;
}

MItemAttr MItemAttr::operator+ (mitem_dim_t d) const {
	MItemAttr attr;
	attr = *this;
	attr.attr().d.w = w(d.w);
	attr.attr().d.h = h(d.h);
	return attr;
}

mitem_dim_t MItemAttr::square(mitem_size_t v) const {
	mitem_dim_t dim;
	if( w() > h() ){
		dim.h = v;
		dim.w = v * h() / w();
	} else {
		dim.w = v;
		dim.h = v * w() / h();
	}
	return dim;
}


mitem_dim_t MItemAttr::square_w(mitem_size_t v) const {
	mitem_dim_t dim;
	dim.w = v;
	dim.h = v * w() / h();
	return dim;
}

mitem_dim_t MItemAttr::square_h(mitem_size_t v) const {
	mitem_dim_t dim;
	dim.h = v;
	dim.w = v * h() / w();
	return dim;
}

void MItemScaledAttr::set(MBitmap * b, mg_point_t p, mg_dim_t d){
	m_attr.b = b;
	m_attr.p = p;
	m_attr.d = d;
}

MItemScaledAttr MItemScaledAttr::operator+ (mg_point_t p) const {
	MItemScaledAttr attr;
	attr = *this;
	attr.attr().p.x += w(p.x);
	attr.attr().p.y += h(p.y);
	return attr;
}

MItemScaledAttr MItemScaledAttr::operator+ (mg_dim_t d) const {
	MItemScaledAttr attr;
	attr = *this;
	attr.attr().d.w = w(d.w);
	attr.attr().d.h = h(d.h);
	return attr;
}

mg_size_t MItemScaledAttr::w(mg_size_t v) const {
	return m_attr.d.w * v / MItem::scale();
}

mg_size_t MItemScaledAttr::h(mg_size_t v) const {
	return m_attr.d.h * v / MItem::scale();
}

MItem::MItem(){
	m_flags = 0;
}

bool MItem::flag(int flag) const{
	return (m_flags & (1<<flag) ) != 0;
}

void MItem::set_flag(int flag, bool v){
	if( v ){
		m_flags |= (1<<flag);
	} else {
		m_flags &= ~(1<<flag);
	}
}


mg_dim_t MItem::dim_on_bitmap(const MItemAttr & attr){
	mg_dim_t d;
	d = mg_dim(width_on_bitmap(attr), height_on_bitmap(attr));
	return d;
}

mg_size_t MItem::height_on_bitmap(const MItemAttr & attr){
	mg_size_t h;
	MBitmap * b = attr.b();
	mitem_dim_t element_dim = attr.d();
	h = scale_to_bitmap(element_dim.h, b->h() - (b->margin_top() + b->margin_bottom()), scale() );
	return h;
}

mg_size_t MItem::width_on_bitmap(const MItemAttr & attr){
	mg_size_t w;
	MBitmap * b = attr.b();
	mitem_dim_t element_dim = attr.d();
	w = scale_to_bitmap(element_dim.w, b->w() - (b->margin_left() + b->margin_right()), scale() );
	return w;
}

mg_point_t MItem::point_on_bitmap(const MItemAttr & attr){
	mg_point_t p1;
	MBitmap * b = attr.b();
	mitem_point_t p = attr.p();

	p1.x = scale_to_bitmap(p.x, b->w() - (b->margin_left() + b->margin_right()), scale()) + b->margin_left();
	p1.y = scale_to_bitmap(p.y, b->h() - (b->margin_bottom() + b->margin_top()), scale()) + b->margin_top();

	return p1;
}

void MItem::set(const MItemAttr & attr, mg_bitmap_t v){
	attr.b()->set( point_on_bitmap(attr), dim_on_bitmap(attr), v);
}

void MItem::clear(const MItemAttr & attr, mg_bitmap_t v){
	attr.b()->clear( point_on_bitmap(attr), dim_on_bitmap(attr), v);
}
void MItem::invert(const MItemAttr & attr, mg_bitmap_t v){
	attr.b()->invert( point_on_bitmap(attr), dim_on_bitmap(attr), v);
}


mg_size_t MItem::w(mg_size_t scaled, mg_dim_t d){
	return scaled * d.w / scale();
}

mg_size_t MItem::h(mg_size_t scaled, mg_dim_t d){
	return scaled * d.h / scale();
}

void MItem::draw(MBitmap * b, mg_int_t x, mg_int_t y, mg_size_t w, mg_size_t h){
	MItemAttr attr;
	attr.set(b, mitem_point(x,y), mitem_dim(w,h));
	draw(attr);
}


void MItem::draw(const MItemAttr & attr){
	MItemScaledAttr attr_scaled;
	attr_scaled.set(attr.b(), point_on_bitmap(attr), dim_on_bitmap(attr));
	draw_to_scale(attr_scaled);
}

void MItem::draw_to_scale(MBitmap * b, mg_int_t x, mg_int_t y, mg_size_t w, mg_size_t h){
	MItemScaledAttr attr_scaled;
	attr_scaled.set(b, mg_point(x,y), mg_dim(w,h));
	draw_to_scale(attr_scaled);
}


void MItem::draw_to_scale(const MItemScaledAttr & attr){}


