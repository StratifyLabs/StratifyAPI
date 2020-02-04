/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
//Copyright 2011-2020 Tyler Gilbert; All Rights Reserved


#include "sgfx.hpp"
#include "ux/Drawing.hpp"
using namespace ux;

drawing_size_t Drawing::m_scale = 1000;

sg_color_t ux::color_transparent(){
	return (sg_color_t)-1;
}

DrawingArea DrawingArea::maximum(){
	return DrawingArea(DrawingAttributes::scale(), DrawingAttributes::scale());
}

static sg_int_t scale_to_bitmap(
		drawing_int_t d,
		sg_size_t s,
		drawing_size_t max){
	int tmp;
	tmp = (d * s + max/2) / max;
	return tmp;
}

drawing_point_t ux::drawing_point(drawing_int_t x, drawing_int_t y){
	drawing_point_t p;
	p.x = x;
	p.y = y;
	return p;
}

drawing_area_t ux::drawing_area(drawing_size_t w, drawing_size_t h){
	drawing_area_t d;
	d.width = w;
	d.height = h;
	return d;
}

DrawingAttributes::DrawingAttributes(){
	memset(&m_attr, 0, sizeof(drawing_attr_t));
}

DrawingAttributes::DrawingAttributes(const drawing_attr_t & attr){
	m_attr = attr;
}

DrawingAttributes::DrawingAttributes(sgfx::Bitmap & bitmap){
	m_attr.bitmap = &bitmap;
	m_attr.region.point.x = 0;
	m_attr.region.point.y = 0;
}

DrawingAttributes::DrawingAttributes(sgfx::Bitmap & bitmap, const DrawingRegion & region){
	m_attr.bitmap = &bitmap;
	m_attr.region = region;
}

sg_area_t DrawingAttributes::calculate_area_on_bitmap(const DrawingAttributes & attr){
	sg_area_t d;
	d = sg_dim(calculate_width_on_bitmap(attr), calculate_height_on_bitmap(attr));
	return d;
}

sg_size_t DrawingAttributes::calculate_height_on_bitmap(const DrawingAttributes & attr, drawing_size_t value){
	sg_size_t h;
	const Bitmap & b = attr.bitmap();
	h = scale_to_bitmap(value, b.height() - (b.margin_top() + b.margin_bottom()), scale() );
	return h;
}

sg_size_t DrawingAttributes::calculate_height_on_bitmap(const DrawingAttributes & attr){
	return calculate_height_on_bitmap(attr, attr.height());
}

sg_size_t DrawingAttributes::calculate_width_on_bitmap(const DrawingAttributes & attr, drawing_size_t value){
	sg_size_t w;
	const Bitmap & b = attr.bitmap();
	w = scale_to_bitmap(value, b.width() - (b.margin_left() + b.margin_right()), scale() );
	return w;
}

sg_size_t DrawingAttributes::calculate_width_on_bitmap(const DrawingAttributes & attr){
	return calculate_width_on_bitmap(attr, attr.width());
}

sg_point_t DrawingAttributes::calculate_point_on_bitmap(const DrawingAttributes & attr){
	sg_point_t p1;
	const Bitmap & b = attr.bitmap();
	drawing_point_t p = attr.point();

	p1.x = scale_to_bitmap(p.x, b.width() - (b.margin_left() + b.margin_right()), scale()) + b.margin_left();
	p1.y = scale_to_bitmap(p.y, b.height() - (b.margin_bottom() + b.margin_top()), scale()) + b.margin_top();

	return p1;
}

sg_region_t DrawingAttributes::calculate_region_on_bitmap(const DrawingAttributes & attr){
	sg_region_t region;
	region.point = calculate_point_on_bitmap(attr);
	region.area = calculate_area_on_bitmap(attr);
	return region;
}


drawing_int_t DrawingAttributes::calculate_width(drawing_int_t v) const {
	return m_attr.region.area.width * v / DrawingAttributes::scale();
}

drawing_int_t DrawingAttributes::calculate_height(drawing_int_t v) const {
	return m_attr.region.area.height * v / DrawingAttributes::scale();
}

DrawingAttributes DrawingAttributes::operator+ (const drawing_point_t & p) const {
	DrawingAttributes attr;
	attr = *this;
	attr.attributes().region.point.x += calculate_width(p.x);
	attr.attributes().region.point.y += calculate_height(p.y);
	return attr;
}

DrawingAttributes DrawingAttributes::operator+ (const drawing_area_t & d) const {
	DrawingAttributes attr;
	attr = *this;
	attr.attributes().region.area.width = calculate_width(d.width);
	attr.attributes().region.area.height = calculate_height(d.height);
	return attr;
}

DrawingAttributes DrawingAttributes::operator+ (const DrawingPoint & d) const {
	return *this + d.point();
}

DrawingAttributes DrawingAttributes::operator+ (const DrawingArea & d) const {
	return *this + d.area();
}

drawing_area_t DrawingAttributes::calculate_square(drawing_size_t v) const {
	drawing_area_t dim;
	if( width() > height() ){
		dim = calculate_square_height(v);
	} else {
		dim = calculate_square_width(v);
	}
	return dim;
}

drawing_area_t DrawingAttributes::calculate_square_width(drawing_size_t v) const {
	u32 pixel_width;
	u32 drawing_height;
	drawing_area_t area;
	sg_area_t bitmap_area = calculate_area_on_bitmap();
	area.width = v;
	pixel_width = bitmap_area.width * v / DrawingAttributes::scale();
	drawing_height = pixel_width * DrawingAttributes::scale() * DrawingAttributes::scale() / (height() * bitmap().height());
	area.height = drawing_height;
	return area;
}

drawing_area_t DrawingAttributes::calculate_square_height(drawing_size_t v) const {
	u32 pixel_height;
	u32 drawing_width;
	drawing_area_t area;
	sg_area_t bitmap_area = calculate_area_on_bitmap();
	area.height = v;
	pixel_height = bitmap_area.height * v / DrawingAttributes::scale();
	drawing_width = pixel_height * DrawingAttributes::scale() * DrawingAttributes::scale() / (width() * bitmap().width());
	area.width = drawing_width;
	return area;
}

void DrawingScaledAttributes::set(Bitmap & b, sg_point_t p, sg_area_t d){
	set_bitmap(b);
	set_point(p);
	set_area(d);
}

DrawingScaledAttributes DrawingScaledAttributes::operator+ (const sgfx::Point & p) const {
	DrawingScaledAttributes result;
	result = *this;
	result.attributes().region.point.x += p.x();
	result.attributes().region.point.y += p.y();
	return result;
}

DrawingScaledAttributes DrawingScaledAttributes::operator+ (const sgfx::Area & a) const {
	DrawingScaledAttributes result;
	result = *this;
	result.attributes().region.area.width = a.width();
	result.attributes().region.area.height = a.height();
	return result;
}

sg_size_t DrawingScaledAttributes::calculate_width(drawing_size_t v) const {
	return m_attr.region.area.width * v / DrawingAttributes::scale();
}

sg_size_t DrawingScaledAttributes::calculate_height(drawing_size_t v) const {
	return m_attr.region.area.height * v / DrawingAttributes::scale();
}

Drawing::Drawing(){}

void Drawing::draw_rectangle(const DrawingAttributes & attr, const Pen & pen){
	attr.bitmap() << pen;
	attr.bitmap().draw_rectangle(
				attr.calculate_point_on_bitmap(),
				attr.calculate_area_on_bitmap()
				);
}

void Drawing::clear(const DrawingAttributes & attr){
	draw_rectangle(attr, Pen().set_color(0));
}

sg_size_t Drawing::width(sg_size_t scaled, sg_area_t d){
	return scaled * d.width / DrawingAttributes::scale();
}

sg_size_t Drawing::height(sg_size_t scaled, sg_area_t d){
	return scaled * d.height / DrawingAttributes::scale();
}

void Drawing::draw(Bitmap & b, sg_int_t x, sg_int_t y, sg_size_t w, sg_size_t h){
	DrawingAttributes attr;
	//attr.set(b, drawing_point(x,y), drawing_dim(w,h));
	draw(attr);
}


void Drawing::draw(const DrawingAttributes & attr){
	DrawingScaledAttributes attr_scaled(attr);
	draw_to_scale(attr_scaled);
}

void Drawing::draw_to_scale(Bitmap & b, sg_int_t x, sg_int_t y, sg_size_t w, sg_size_t h){
	DrawingScaledAttributes attr_scaled;
	attr_scaled.set(b, sg_point(x,y), sg_dim(w,h));
	draw_to_scale(attr_scaled);
}


void Drawing::draw_to_scale(const DrawingScaledAttributes & attr){}


