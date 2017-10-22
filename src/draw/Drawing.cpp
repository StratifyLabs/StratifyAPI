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

drawing_point_t draw::drawing_point(drawing_int_t x, drawing_int_t y){
	drawing_point_t p;
	p.x = x;
	p.y = y;
	return p;
}

drawing_dim_t draw::drawing_dim(drawing_size_t w, drawing_size_t h){
	drawing_dim_t d;
	d.width = w;
	d.height = h;
	return d;
}

DrawingAttr::DrawingAttr(){
	memset(&m_attr, 0, sizeof(drawing_attr_t));
}

DrawingAttr::DrawingAttr(const drawing_attr_t & attr){
	m_attr = attr;
}


void DrawingAttr::set(Bitmap & b, drawing_point_t p, drawing_dim_t d, Bitmap * scratch){
	set_bitmap(b);
	set_point(p);
	set_dim(d);
	set_scratch(scratch);
}

drawing_size_t DrawingAttr::calc_width(drawing_size_t v) const {
	return m_attr.dim.width * v / Drawing::scale();
}

drawing_size_t DrawingAttr::calc_height(drawing_size_t v) const {
	return m_attr.dim.height * v / Drawing::scale();
}

DrawingAttr DrawingAttr::operator+ (drawing_point_t p) const {
	DrawingAttr attr;
	attr = *this;
	attr.attr().point.x += calc_width(p.x);
	attr.attr().point.y += calc_height(p.y);
	return attr;
}

DrawingAttr DrawingAttr::operator+ (drawing_dim_t d) const {
	DrawingAttr attr;
	attr = *this;
	attr.attr().dim.width = calc_width(d.width);
	attr.attr().dim.height = calc_height(d.height);
	return attr;
}

drawing_dim_t DrawingAttr::calc_square(drawing_size_t v) const {
	drawing_dim_t dim;
	if( width() > height() ){
		dim = calc_square_h(v);
	} else {
		dim = calc_square_w(v);
	}
	return dim;
}

drawing_dim_t DrawingAttr::calc_square_w(drawing_size_t v) const {
	u32 pixel_width;
	u32 drawing_height;
	drawing_dim_t dim;
	sg_dim_t bitmap_dim = Drawing::dim_on_bitmap(*this);
	dim.width = v;
	pixel_width = bitmap_dim.width * v / Drawing::scale();
	drawing_height = pixel_width * Drawing::scale() * Drawing::scale() / (height() * bitmap().height());
	dim.height = drawing_height;
	return dim;
}

drawing_dim_t DrawingAttr::calc_square_h(drawing_size_t v) const {
	u32 pixel_height;
	u32 drawing_width;
	drawing_dim_t dim;
	sg_dim_t bitmap_dim = Drawing::dim_on_bitmap(*this);
	dim.height = v;
	pixel_height = bitmap_dim.height * v / Drawing::scale();
	drawing_width = pixel_height * Drawing::scale() * Drawing::scale() / (width() * bitmap().width());
	dim.width = drawing_width;
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
	attr.attr().point.x += calc_width(p.x);
	attr.attr().point.y += calc_height(p.y);
	return attr;
}

DrawingScaledAttr DrawingScaledAttr::operator+ (sg_dim_t d) const {
	DrawingScaledAttr attr;
	attr = *this;
	attr.attr().dim.width = calc_width(d.width);
	attr.attr().dim.height = calc_height(d.height);
	return attr;
}

sg_size_t DrawingScaledAttr::calc_width(drawing_size_t v) const {
	return m_attr.dim.width * v / Drawing::scale();
}

sg_size_t DrawingScaledAttr::calc_height(drawing_size_t v) const {
	return m_attr.dim.height * v / Drawing::scale();
}

Drawing::Drawing(){
	m_flags = 0;
}

bool Drawing::flag(u32 flag) const{
	return (m_flags & (1<<flag) ) != 0;
}

void Drawing::set_flag(u32 flag, bool v){
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
	Bitmap * b = &attr.bitmap();
	drawing_dim_t element_dim = attr.dim();
	h = scale_to_bitmap(element_dim.height, b->height() - (b->margin_top() + b->margin_bottom()), scale() );
	return h;
}

sg_size_t Drawing::width_on_bitmap(const DrawingAttr & attr){
	sg_size_t w;
	Bitmap * b = &attr.bitmap();
	drawing_dim_t element_dim = attr.dim();
	w = scale_to_bitmap(element_dim.width, b->width() - (b->margin_left() + b->margin_right()), scale() );
	return w;
}

sg_point_t Drawing::point_on_bitmap(const DrawingAttr & attr){
	sg_point_t p1;
	Bitmap * b = &attr.bitmap();
	drawing_point_t p = attr.point();

	p1.x = scale_to_bitmap(p.x, b->width() - (b->margin_left() + b->margin_right()), scale()) + b->margin_left();
	p1.y = scale_to_bitmap(p.y, b->height() - (b->margin_bottom() + b->margin_top()), scale()) + b->margin_top();

	return p1;
}

void Drawing::draw_rectangle(const DrawingAttr & attr, const Pen & pen){
	Pen p = attr.bitmap().pen();
	attr.bitmap().set_pen(pen);
	attr.bitmap().draw_rectangle( point_on_bitmap(attr), dim_on_bitmap(attr));
	attr.bitmap().set_pen(p);
}


void Drawing::set(const DrawingAttr & attr, sg_color_t color){
	Pen pen(color);
	draw_rectangle(attr, pen);
}

void Drawing::clear(const DrawingAttr & attr, sg_bmap_data_t v){
	Pen pen(0);
	draw_rectangle(attr, pen);
}
void Drawing::invert(const DrawingAttr & attr, sg_bmap_data_t v){
	Pen pen(0xffff);
	pen.set_invert();
	draw_rectangle(attr, pen);
}

sg_size_t Drawing::width(sg_size_t scaled, sg_dim_t d){
	return scaled * d.width / scale();
}

sg_size_t Drawing::height(sg_size_t scaled, sg_dim_t d){
	return scaled * d.height / scale();
}

void Drawing::draw(Bitmap & b, sg_int_t x, sg_int_t y, sg_size_t w, sg_size_t h){
	DrawingAttr attr;
	attr.set(b, drawing_point(x,y), drawing_dim(w,h));
	draw(attr);
}


void Drawing::draw(const DrawingAttr & attr){
	DrawingScaledAttr attr_scaled;
	attr_scaled.set(attr.bitmap(), point_on_bitmap(attr), dim_on_bitmap(attr));
	draw_to_scale(attr_scaled);
}

void Drawing::draw_scratch(const DrawingAttr & attr){
	if( attr.scratch() ){
		DrawingAttr scratch_attr = attr;
		scratch_attr.set_bitmap(*attr.scratch());
		draw(scratch_attr);
	}
}

void Drawing::draw_to_scale(Bitmap & b, sg_int_t x, sg_int_t y, sg_size_t w, sg_size_t h){
	DrawingScaledAttr attr_scaled;
	attr_scaled.set(b, sg_point(x,y), sg_dim(w,h));
	draw_to_scale(attr_scaled);
}


void Drawing::draw_to_scale(const DrawingScaledAttr & attr){}


