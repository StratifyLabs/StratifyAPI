//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include "sgfx.hpp"
#include "draw/Drawing.hpp"
using namespace draw;

drawing_size_t Drawing::m_scale = 100;

static sg_int_t scale_to_bitmap(drawing_size_t d, sg_size_t s, drawing_size_t max){
	int tmp;
	tmp = (d * s + max/2) / max;
	return tmp;
}

drawing_point_t drawing_point(drawing_int_t x, drawing_int_t y){
	drawing_point_t p;
	p.x = x;
	p.y = y;
	return p;
}

drawing_dim_t drawing_dim(drawing_size_t w, drawing_size_t h){
	drawing_dim_t d;
	d.w = w;
	d.h = h;
	return d;
}

DrawingAttr::DrawingAttr(){
	memset(&m_attr, 0, sizeof(drawing_attr_t));
}

DrawingAttr::DrawingAttr(drawing_attr_t & attr){
	m_attr = attr;
}


void DrawingAttr::set(Bitmap & b, drawing_point_t p, drawing_dim_t d){
	set_bitmap(b);
	set_point(p);
	set_dim(d);
}

drawing_size_t DrawingAttr::w(drawing_size_t v) const {
	return m_attr.d.w * v / Drawing::scale();
}

drawing_size_t DrawingAttr::h(drawing_size_t v) const {
	return m_attr.d.h * v / Drawing::scale();
}

DrawingAttr DrawingAttr::operator+ (drawing_point_t p) const {
	DrawingAttr attr;
	attr = *this;
	attr.attr().p.x += w(p.x);
	attr.attr().p.y += h(p.y);
	return attr;
}

DrawingAttr DrawingAttr::operator+ (drawing_dim_t d) const {
	DrawingAttr attr;
	attr = *this;
	attr.attr().d.w = w(d.w);
	attr.attr().d.h = h(d.h);
	return attr;
}

drawing_dim_t DrawingAttr::square(drawing_size_t v) const {
	drawing_dim_t dim;
	if( w() > h() ){
		dim.h = v;
		dim.w = v * h() / w();
	} else {
		dim.w = v;
		dim.h = v * w() / h();
	}
	return dim;
}


drawing_dim_t DrawingAttr::square_w(drawing_size_t v) const {
	drawing_dim_t dim;
	dim.w = v;
	dim.h = v * w() / h();
	return dim;
}

drawing_dim_t DrawingAttr::square_h(drawing_size_t v) const {
	drawing_dim_t dim;
	dim.h = v;
	dim.w = v * h() / w();
	return dim;
}

void DrawingScaledAttr::set(Bitmap & b, sg_point_t p, sg_dim_t d){
	set_bitmap(b);
	set_point(p);
	set_dim(d);
}

DrawingScaledAttr DrawingScaledAttr::operator+ (sg_point_t p) const {
	DrawingScaledAttr attr;
	attr = *this;
	attr.attr().p.x += w(p.x);
	attr.attr().p.y += h(p.y);
	return attr;
}

DrawingScaledAttr DrawingScaledAttr::operator+ (sg_dim_t d) const {
	DrawingScaledAttr attr;
	attr = *this;
	attr.attr().d.w = w(d.w);
	attr.attr().d.h = h(d.h);
	return attr;
}

sg_size_t DrawingScaledAttr::w(sg_size_t v) const {
	return m_attr.d.w * v / Drawing::scale();
}

sg_size_t DrawingScaledAttr::h(sg_size_t v) const {
	return m_attr.d.h * v / Drawing::scale();
}

Drawing::Drawing(){
	m_flags = 0;
}

bool Drawing::flag(int flag) const{
	return (m_flags & (1<<flag) ) != 0;
}

void Drawing::set_flag(int flag, bool v){
	if( v ){
		m_flags |= (1<<flag);
	} else {
		m_flags &= ~(1<<flag);
	}
}


sg_dim_t Drawing::dim_on_bitmap(const DrawingAttr & attr){
	sg_dim_t d;
	d = sg_dim(width_on_bitmap(attr), height_on_bitmap(attr));
	return d;
}

sg_size_t Drawing::height_on_bitmap(const DrawingAttr & attr){
	sg_size_t h;
	Bitmap * b = &attr.b();
	drawing_dim_t element_dim = attr.d();
	h = scale_to_bitmap(element_dim.h, b->h() - (b->margin_top() + b->margin_bottom()), scale() );
	return h;
}

sg_size_t Drawing::width_on_bitmap(const DrawingAttr & attr){
	sg_size_t w;
	Bitmap * b = &attr.b();
	drawing_dim_t element_dim = attr.d();
	w = scale_to_bitmap(element_dim.w, b->w() - (b->margin_left() + b->margin_right()), scale() );
	return w;
}

sg_point_t Drawing::point_on_bitmap(const DrawingAttr & attr){
	sg_point_t p1;
	Bitmap * b = &attr.b();
	drawing_point_t p = attr.p();

	p1.x = scale_to_bitmap(p.x, b->w() - (b->margin_left() + b->margin_right()), scale()) + b->margin_left();
	p1.y = scale_to_bitmap(p.y, b->h() - (b->margin_bottom() + b->margin_top()), scale()) + b->margin_top();

	return p1;
}

void Drawing::set(const DrawingAttr & attr, sg_bmap_data_t v){
	attr.b().set( point_on_bitmap(attr), dim_on_bitmap(attr), v);
}

void Drawing::clear(const DrawingAttr & attr, sg_bmap_data_t v){
	attr.b().clear( point_on_bitmap(attr), dim_on_bitmap(attr), v);
}
void Drawing::invert(const DrawingAttr & attr, sg_bmap_data_t v){
	attr.b().invert( point_on_bitmap(attr), dim_on_bitmap(attr), v);
}


sg_size_t Drawing::w(sg_size_t scaled, sg_dim_t d){
	return scaled * d.w / scale();
}

sg_size_t Drawing::h(sg_size_t scaled, sg_dim_t d){
	return scaled * d.h / scale();
}

void Drawing::draw(Bitmap & b, sg_int_t x, sg_int_t y, sg_size_t w, sg_size_t h){
	DrawingAttr attr;
	attr.set(b, drawing_point(x,y), drawing_dim(w,h));
	draw(attr);
}


void Drawing::draw(const DrawingAttr & attr){
	DrawingScaledAttr attr_scaled;
	attr_scaled.set(attr.b(), point_on_bitmap(attr), dim_on_bitmap(attr));
	draw_to_scale(attr_scaled);
}

void Drawing::draw_to_scale(Bitmap & b, sg_int_t x, sg_int_t y, sg_size_t w, sg_size_t h){
	DrawingScaledAttr attr_scaled;
	attr_scaled.set(b, sg_point(x,y), sg_dim(w,h));
	draw_to_scale(attr_scaled);
}


void Drawing::draw_to_scale(const DrawingScaledAttr & attr){}


