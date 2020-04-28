/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include "sgfx.hpp"
#include "draw/Drawing.hpp"
#include "sys/Printer.hpp"

using namespace draw;


sys::Printer& operator << (sys::Printer& printer, const draw::DrawingPoint& a){
	printer.key("x", "%d", a.x());
	printer.key("y", "%d", a.y());
	return printer;
}

sys::Printer& operator << (sys::Printer& printer, const draw::DrawingArea& a){
	printer.key("width", "%d", a.width());
	printer.key("height", "%d", a.height());
	return printer;
}

sys::Printer& operator << (sys::Printer& printer, const draw::DrawingRegion& a){
	printer.key("x", "%d", a.point().x());
	printer.key("y", "%d", a.point().y());
	printer.key("width", "%d", a.area().width());
	printer.key("height", "%d", a.area().height());
	return printer;
}

drawing_size_t Drawing::m_scale = 1000;
sg_color_t Drawing::m_default_color = 1;

sg_color_t draw::color_transparent(){
	return (sg_color_t)-1;
}

DrawingArea DrawingArea::maximum(){
	return DrawingArea(DrawingAttributes::scale(), DrawingAttributes::scale());
}

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

drawing_area_t draw::drawing_area(drawing_size_t w, drawing_size_t h){
	drawing_area_t d;
	d.width = w;
	d.height = h;
	return d;
}

drawing_area_t draw::drawing_dim(drawing_size_t w, drawing_size_t h){
	return drawing_area(w,h);
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
	m_attr.scratch = 0;
}

DrawingAttributes::DrawingAttributes(sgfx::Bitmap & bitmap, const DrawingRegion & region){
	m_attr.bitmap = &bitmap;
	m_attr.region = region;
	m_attr.scratch = 0;
}

DrawingAttributes & DrawingAttributes::operator << (sgfx::Bitmap & bitmap){
	m_attr.bitmap = &bitmap; return *this;
}

DrawingAttributes & DrawingAttributes::operator << (const DrawingRegion & region){
	m_attr.region = region; return *this;
}

DrawingAttributes & DrawingAttributes::operator << (const DrawingPoint & point){
	m_attr.region.point = point; return *this;
}

DrawingAttributes & DrawingAttributes::operator << (const DrawingArea & area){
	m_attr.region.area = area; return *this;
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
	Bitmap * b = &attr.bitmap();
	drawing_point_t p = attr.point();

	p1.x = scale_to_bitmap(p.x, b->width() - (b->margin_left() + b->margin_right()), scale()) + b->margin_left();
	p1.y = scale_to_bitmap(p.y, b->height() - (b->margin_bottom() + b->margin_top()), scale()) + b->margin_top();

	return p1;
}

sg_region_t DrawingAttributes::calculate_region_on_bitmap(const DrawingAttributes & attr){
	sg_region_t region;
	region.point = calculate_point_on_bitmap(attr);
	region.area = calculate_area_on_bitmap(attr);
	return region;
}



void DrawingAttributes::set(Bitmap & b, drawing_point_t p, drawing_area_t d, Bitmap * scratch){
	set_bitmap(b);
	set_point(p);
	set_area(d);
	set_scratch(scratch);
}

drawing_size_t DrawingAttributes::calculate_width(drawing_size_t v) const {
	return m_attr.region.area.width * v / DrawingAttributes::scale();
}

drawing_size_t DrawingAttributes::calculate_height(drawing_size_t v) const {
	return m_attr.region.area.height * v / DrawingAttributes::scale();
}

DrawingAttributes DrawingAttributes::operator+ (const drawing_point_t & p) const {
	DrawingAttributes attr;
	attr = *this;
	attr.attr().region.point.x += calculate_width(p.x);
	attr.attr().region.point.y += calculate_height(p.y);
	return attr;
}

DrawingAttributes DrawingAttributes::operator+ (const drawing_area_t & d) const {
	DrawingAttributes attr;
	attr = *this;
	attr.attr().region.area.width = calculate_width(d.width);
	attr.attr().region.area.height = calculate_height(d.height);
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

DrawingWindow::DrawingWindow(
      const DrawingAttributes & parent_attributes,
      const DrawingRegion & window_region
      ){
   update(parent_attributes, window_region);
}

DrawingWindow& DrawingWindow::update(
      const DrawingAttributes & parent_attributes,
      const DrawingRegion & window_region
      ){
   //region is the window region mapped to the parent attributes
   DrawingAttributes modified_attributes =
         parent_attributes + window_region.point() + window_region.area();

   m_region << modified_attributes.calculate_area_on_bitmap();
   m_region << modified_attributes.calculate_point_on_bitmap();

   m_bitmap.allocate(
            m_region.area(),
            sgfx::Bitmap::BitsPerPixel(
               parent_attributes.bitmap().bits_per_pixel()
               )
            );

   //attributes are full scale and non-shifted
   m_attributes.set_area(DrawingArea(1000,1000))
         .set_point(DrawingPoint(0,0))
         .set_bitmap(m_bitmap);
   return *this;
}

void DrawingScaledAttr::set(Bitmap & b, sg_point_t p, sg_area_t d){
	set_bitmap(b);
	set_point(p);
	set_area(d);
}

DrawingScaledAttr DrawingScaledAttr::operator+ (sg_point_t p) const {
	DrawingScaledAttr attr;
	attr = *this;
	attr.attr().region.point.x += calculate_width(p.x);
	attr.attr().region.point.y += calculate_height(p.y);
	return attr;
}

DrawingScaledAttr DrawingScaledAttr::operator+ (sg_area_t d) const {
	DrawingScaledAttr attr;
	attr = *this;
	attr.attr().region.area.width = calculate_width(d.width);
	attr.attr().region.area.height = calculate_height(d.height);
	return attr;
}

sg_size_t DrawingScaledAttr::calculate_width(drawing_size_t v) const {
	return m_attr.region.area.width * v / DrawingAttributes::scale();
}

sg_size_t DrawingScaledAttr::calculate_height(drawing_size_t v) const {
	return m_attr.region.area.height * v / DrawingAttributes::scale();
}

Drawing::Drawing(){
	m_flags = 0;
	set_color( default_color() );
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

void Drawing::draw_rectangle(const DrawingAttributes & attr, const Pen & pen){
	attr.bitmap() << pen;
	attr.bitmap().draw_rectangle(
				attr.calculate_point_on_bitmap(),
				attr.calculate_area_on_bitmap()
				);
}

void Drawing::draw_checkerboard(const DrawingAttributes & attr, sg_size_t pattern_height){
	sg_bmap_data_t odd = 0xAAAAAAAA;
	sg_bmap_data_t even = 0x55555555;
	switch(pattern_height){
		case 2:
			odd = 0xCCCCCCCC;
			even = 0x33333333;
			break;
		case 4:
			odd = 0xF0F0F0F0;
			even = 0x0F0F0F0F;
			break;
		case 8:
			odd = 0xFF00FF00;
			even = 0x00FF00FF;
			break;
	}
	draw_pattern(attr, odd, even, pattern_height);
}

void Drawing::draw_pattern(const DrawingAttributes & attr, sg_bmap_data_t odd_pattern, sg_bmap_data_t even_pattern, sg_size_t pattern_height){
	attr.bitmap().draw_pattern(
				attr.calculate_point_on_bitmap(),
				attr.calculate_area_on_bitmap(),
				odd_pattern,
				even_pattern,
				pattern_height
				);
}


void Drawing::set(const DrawingAttributes & attr, sg_color_t color){
	draw_rectangle(attr, Pen().set_color(color));
}

void Drawing::clear(const DrawingAttributes & attr){
	draw_rectangle(attr, Pen().set_color(0));
}

void Drawing::invert(const DrawingAttributes & attr, sg_bmap_data_t v){
	draw_rectangle(attr, Pen().set_color(0xffff).set_invert());
}

sg_size_t Drawing::width(sg_size_t scaled, sg_area_t d){
	return scaled * d.width / DrawingAttributes::scale();
}

sg_size_t Drawing::height(sg_size_t scaled, sg_area_t d){
	return scaled * d.height / DrawingAttributes::scale();
}

void Drawing::draw(Bitmap & b, sg_int_t x, sg_int_t y, sg_size_t w, sg_size_t h){
	DrawingAttributes attr;
	attr.set(b, drawing_point(x,y), drawing_dim(w,h));
	draw(attr);
}


void Drawing::draw(const DrawingAttributes & attr){
	DrawingScaledAttributes attr_scaled(attr);
	draw_to_scale(attr_scaled);
}

void Drawing::draw_scratch(const DrawingAttributes & attr){
	if( attr.scratch() ){
		DrawingAttributes scratch_attr = attr;
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


