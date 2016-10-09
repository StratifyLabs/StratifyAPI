/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include <cmath>
#include "sgfx.hpp"
#include "draw/ProgressArc.hpp"
using namespace draw;

ProgressArc::ProgressArc() {
	m_offset = 0;
	m_direction = 1;

}

void ProgressArc::draw_to_scale(const DrawingScaledAttr & attr){
	//draw the progress bar on the bitmap with x, y at the top left corner
	sg_point_t p = attr.p();
	Dim d = attr.d();
	s8 dir;
	float xf, yf;
	float xf_inner, yf_inner;
	Point center(p.x + d.w()/2, p.y + d.h()/2);
	sg_size_t rx = d.w()/2;
	sg_size_t ry = d.h()/2;

	sg_size_t rx_inner = rx*2/3;
	sg_size_t ry_inner = ry*2/3;

	Point arc;
	Point arc_inner;
	Point first_point;
	size_t i;
	sg_int_t x_max;

	size_t progress;
	size_t points = d.w() * d.h();

	float two_pi = 2.0 * M_PI;
	float half_pi = M_PI / 2.0;
	float theta;
	float offset = m_offset * two_pi / 360;

	progress = (points * value()) / max();

	if( m_direction > 0 ){
		dir = 1;
	} else {
		dir = -1;
	}


	x_max = 0;
	i = 0;
	do {

		theta = (two_pi * i*dir / points - half_pi) + offset;

		xf = rx * cosf(theta);
		yf = ry * sinf(theta);

		xf_inner = rx_inner * cosf(theta);
		yf_inner = ry_inner * sinf(theta);

		arc.set(xf, yf);
		arc = arc + center;

		if( arc.x() > x_max ){
			x_max = arc.x();
		}

		arc_inner.set(xf_inner, yf_inner);
		arc_inner = arc_inner + center;

		if( i == 0 ){
			if( !dark() ){
				attr.bitmap().set_line(arc_inner, arc);
			} else {
				attr.bitmap().clr_line(arc_inner, arc);
			}
		}

		if( i == 1 ){
			first_point = arc;
		}

		if( !dark() ){
			attr.bitmap().set_pixel(arc);
			attr.bitmap().set_pixel(arc_inner);
		} else {
			attr.bitmap().clr_pixel(arc);
			attr.bitmap().clr_pixel(arc_inner);
		}
		i++;
	} while( i < progress );

	if( !dark() ){
		attr.bitmap().set_line(arc_inner, arc);
	} else {
		attr.bitmap().clr_line(arc_inner, arc);
	}

	if( dark() ){
		attr.bitmap().invert(p,d);
	}

	if( progress > 0 ){
		theta = (two_pi * 1 / points - half_pi) + offset;
		xf = rx * cosf(theta);
		yf = ((ry + ry_inner)/2)* sinf(theta);
		arc.set(xf, yf);
		arc = arc + center;
		attr.bitmap().pour(arc, Pen(1,1,true));
	}

	if( dark() ){
		attr.bitmap().invert(p,d);
	}

}
