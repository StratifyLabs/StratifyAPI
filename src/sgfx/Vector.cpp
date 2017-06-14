//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "sgfx/Vector.hpp"
#include <cstdio>

using namespace sgfx;



void VectorMap::set_dim(const Dim & dim){
	data()->size.h = dim.h();
	data()->size.w = dim.w();
}

void VectorMap::set_shift(const Point & p){
	data()->shift.x = p.x();
	data()->shift.y = p.y();
}

void VectorMap::set_dim(sg_size_t w, sg_size_t h){
	data()->size.w = w;
	data()->size.h = h;
}

void VectorMap::set_shift(sg_int_t x, sg_int_t y){
	data()->shift.x = x;
	data()->shift.y = y;
}


VectorMap::VectorMap(const Bitmap & bitmap, const Pen & pen, s16 rotation){
	set_bitmap_center(bitmap, pen, rotation);
}

void VectorMap::set_bitmap_center(const Bitmap & bitmap, const Pen & pen, s16 rotation){
	u8 thickness = pen.thickness();
	data()->size.w = (bitmap.w() - 2*thickness)*2;
	data()->size.h = (bitmap.h() - 2*thickness)*2;
	data()->shift.x = (data()->size.w + 2*thickness)/4;
	data()->shift.y = (data()->size.h + 2*thickness)/4;
	data()->rotation = rotation;
}


void Vector::draw(Bitmap & bitmap, const sg_vector_icon_t & icon, const sg_vector_map_t & map, sg_bounds_t * bounds){
	sg_api()->vector_draw_icon(bitmap.bmap(), &icon, &map, bounds);
}

sg_vector_primitive_t Vector::line(sg_int_t x1, sg_int_t y1, sg_int_t x2, sg_int_t y2){
	sg_vector_primitive_t ret;
	memset(&ret, 0, sizeof(ret));
	ret.type = SG_LINE | SG_ENABLE_FLAG;
	ret.shift.x = x1;
	ret.shift.y = y1;
	ret.line.p.x = x2;
	ret.line.p.y = y2;
	ret.rotation = 0;
	return ret;
}


sg_vector_primitive_t Vector::circle(sg_int_t x, sg_int_t y, sg_size_t r){
	sg_vector_primitive_t ret;
	memset(&ret, 0, sizeof(ret));
	ret.type = SG_ARC | SG_ENABLE_FLAG;
	ret.shift.x = x;
	ret.shift.y = y;
	ret.arc.rx = r;
	ret.arc.ry = r;
	ret.arc.start = 0;
	ret.arc.stop = SG_TRIG_POINTS;
	return ret;
}

sg_vector_primitive_t Vector::arc(sg_int_t x, sg_int_t y, sg_size_t rx, sg_size_t ry, sg_int_t start, sg_int_t stop, sg_int_t rotation){
	sg_vector_primitive_t ret;
	memset(&ret, 0, sizeof(ret));
	ret.type = SG_ARC | SG_ENABLE_FLAG;
	ret.shift.x = x;
	ret.shift.y = y;
	ret.arc.rx = rx;
	ret.arc.ry = ry;
	ret.arc.start = start;
	ret.arc.stop = stop;
	ret.rotation = rotation;
	return ret;
}

sg_vector_primitive_t Vector::fill(sg_int_t x1, sg_int_t y1){
	sg_vector_primitive_t ret;
	memset(&ret, 0, sizeof(ret));
	ret.type = SG_FILL | SG_ENABLE_FLAG;
	ret.shift.x = x1;
	ret.shift.y = y1;
	ret.rotation = 0;

	return ret;
}


void Vector::show(const sg_vector_primitive_t & mg){
	size_t i;
	const unsigned char * p = (const unsigned char *)(&mg);
	printf("{ ");
	for(i=0; i < sizeof(sg_vector_primitive_t); i++){
		printf("0x%X", p[i]);
		if( i < (sizeof(sg_vector_primitive_t)-1) ){
			printf(",");
		}
		printf(" ");
	}
	printf("}");
}

void Vector::show(const sg_vector_icon_t & icon){
	u8 i;
	for(i=0; i < icon.total; i++){
		show(icon.primitives[i]);
		if( i < (icon.total-1) ){
			printf(",");
		}
		printf("\n");
	}
}





void Vector::draw_remove(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){

	int total = 2;
	sg_vector_primitive_t objs[total];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.total = total;
	icon.fill_total = 0;
	objs[0] = line(-SG_MAX/2, -SG_MAX/2, SG_MAX/2, SG_MAX/2);
	objs[1] = line(-SG_MAX/2, SG_MAX/2, SG_MAX/2, -SG_MAX/2);

	draw(bitmap, icon, map.item());
	if( show ){
		Vector::show(icon);
	}
}

void Vector::draw_ok(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){
	int total = 2;
	sg_vector_primitive_t objs[total];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.total = total;
	icon.fill_total = 0;
	objs[0] = line(-SG_MAX/2, SG_MAX/4, -SG_MAX/4, SG_MAX/2);
	objs[1] = line(-SG_MAX/4, SG_MAX/2, SG_MAX/2, -SG_MAX/4);

	draw(bitmap, icon, map.item());
	if( show ){
		Vector::show(icon);
	}
}

void Vector::draw_bars(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){
	int total = 3;
	sg_vector_primitive_t objs[total];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.total = total;
	icon.fill_total = 0;
	objs[0] = Vector::line(SG_MIN/2, SG_MIN/2, SG_MAX/2, SG_MIN/2);
	objs[1] = Vector::line(SG_MIN/2, 0, SG_MAX/2, 0);
	objs[2] = Vector::line(SG_MIN/2, SG_MAX/2, SG_MAX/2, SG_MAX/2);

	draw(bitmap, icon, map.item());
	if( show ){
		Vector::show(icon);
	}
}

void Vector::draw_circle(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){
	int total = 1;
	sg_vector_primitive_t objs[total];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.total = total;
	icon.fill_total = 0;
	objs[0] = circle(0, 0, SG_MAX/2);
	draw(bitmap, icon, map.item());
	if( show ){
		Vector::show(icon);
	}
}

void Vector::draw_circle_fill(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){
	int total = 2;
	sg_vector_primitive_t objs[total];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.total = total;
	icon.fill_total = 0;
	objs[0] = circle(0, 0, SG_MAX/2);
	objs[1] = fill(0, 0);
	draw(bitmap, icon, map.item());
	if( show ){
		Vector::show(icon);
	}
}

void Vector::draw_toggle_off(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){
	int total = 5;
	sg_vector_primitive_t objs[total];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.total = total;
	icon.fill_total = 0;
	objs[0] = line(-SG_MAX/4, -SG_MAX/4, SG_MAX/4, -SG_MAX/4);
	objs[1] = line(-SG_MAX/4, SG_MAX/4, SG_MAX/4, SG_MAX/4);
	objs[2] = arc(-SG_MAX/4, 0, SG_MAX/4, SG_MAX/4, SG_TRIG_POINTS/4, SG_TRIG_POINTS*3/4);
	objs[3] = arc(SG_MAX/4, 0, SG_MAX/4, SG_MAX/4, SG_TRIG_POINTS*3/4, SG_TRIG_POINTS*5/4);
	objs[4] = circle(-SG_MAX/4, 0, SG_MAX/4);

	draw(bitmap, icon, map.item());
	if( show ){
		Vector::show(icon);
	}
}

void Vector::draw_toggle_on(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){
	int total = 6;
	sg_vector_primitive_t objs[total];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.total = total;
	icon.fill_total = 0;
	objs[0] = line(-SG_MAX/4, -SG_MAX/4, SG_MAX/4, -SG_MAX/4);
	objs[1] = line(-SG_MAX/4, SG_MAX/4, SG_MAX/4, SG_MAX/4);
	objs[2] = arc(-SG_MAX/4, 0, SG_MAX/4, SG_MAX/4, SG_TRIG_POINTS/4, SG_TRIG_POINTS*3/4);
	objs[3] = arc(SG_MAX/4, 0, SG_MAX/4, SG_MAX/4, SG_TRIG_POINTS*3/4, SG_TRIG_POINTS*5/4);
	objs[4] = circle(SG_MAX/4, 0, SG_MAX/4);
	objs[5] = fill(-SG_MAX/4, 0);

	draw(bitmap, icon, map.item());
	if( show ){
		Vector::show(icon);
	}
}

void Vector::draw_power(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){
	int total = 2;
	sg_vector_primitive_t objs[total];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.total = total;
	icon.fill_total = 0;
	objs[0] = line(0, 0, 0, -SG_MAX/2);
	objs[1] = arc(0, 0, SG_MAX/2, SG_MAX/2, SG_TRIG_POINTS*7/8, SG_TRIG_POINTS*7/8 + SG_TRIG_POINTS*3/4);

	draw(bitmap, icon, map.item());
	if( show ){
		Vector::show(icon);
	}
}

void Vector::draw_chevron(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){
	int total = 2;
	sg_vector_primitive_t objs[total];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.total = total;
	icon.fill_total = 0;
	objs[0] = line(-SG_MAX/8, -SG_MAX/2, SG_MAX/8, 0);
	objs[1] = line(SG_MAX/8, 0, -SG_MAX/8, SG_MAX/2);

	draw(bitmap, icon, map.item());
	if( show ){
		Vector::show(icon);
	}
}

void Vector::draw_arrow(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){
	int total = 3;
	sg_vector_primitive_t objs[total];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.total = total;
	icon.fill_total = 0;
	objs[0] = line(-SG_MAX/2, 0, SG_MAX/2, 0);
	objs[1] = line(0, SG_MAX/2, SG_MAX/2, 0);
	objs[2] = line(0, -SG_MAX/2, SG_MAX/2, 0);

	draw(bitmap, icon, map.item());
	if( show ){
		Vector::show(icon);
	}
}

void Vector::draw_zoom(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){
	int total = 2;
	sg_vector_primitive_t objs[total];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.total = total;
	icon.fill_total = 0;
	objs[0] = circle(SG_MAX/6, -SG_MAX/6, SG_MAX*3/10);
	objs[1] = line(-SG_MAX/20, SG_MAX/20, -SG_MAX/2, SG_MAX/2);

	draw(bitmap, icon, map.item());
	if( show ){
		Vector::show(icon);
	}
}

void Vector::draw_reset(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){
	int total = 3;
	sg_vector_primitive_t objs[total];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.total = total;
	icon.fill_total = 0;
	objs[0] = line(SG_MAX*3/10, 0, SG_MAX*5/10, -SG_MAX*2/20);
	objs[1] = line(SG_MAX*3/10, 0, SG_MAX*1/10, -SG_MAX*2/20);
	objs[2] = arc(-SG_MAX/40, 0, SG_MAX*3/10 + SG_MAX*3/40, SG_MAX*3/10 + SG_MAX*3/40, SG_TRIG_POINTS/12, SG_TRIG_POINTS);

	draw(bitmap, icon, map.item());
	if( show ){
		Vector::show(icon);
	}
}

void Vector::draw_lightning(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){
	int total = 3;
	sg_vector_primitive_t objs[total];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.total = total;
	icon.fill_total = 0;
	objs[0] = line(0, -SG_MAX/2, -SG_MAX/4, SG_MAX/10);
	objs[1] = line(-SG_MAX/4, SG_MAX/10, SG_MAX/4, -SG_MAX/10);
	objs[2] = line(SG_MAX/4, -SG_MAX/10, 0, SG_MAX/2);

	draw(bitmap, icon, map.item());
	if( show ){
		Vector::show(icon);
	}
}

void Vector::draw_play(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){
	int total = 4;
	sg_vector_primitive_t objs[total];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.total = total;
	icon.fill_total = 0;
	objs[0] = line(-SG_MAX/2, -SG_MAX/2, -SG_MAX/2, SG_MAX/2);
	objs[1] = line(-SG_MAX/2, SG_MAX/2, SG_MAX/2, 0);
	objs[2] = line(SG_MAX/2, 0, -SG_MAX/2, -SG_MAX/2);
	objs[3] = fill(0, 0);

	draw(bitmap, icon, map.item());
	if( show ){
		Vector::show(icon);
	}
}


void Vector::draw_pause(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){
	int total = 2;
	sg_vector_primitive_t objs[total];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.total = total;
	icon.fill_total = 0;
	objs[0] = line(-SG_MAX/4, -SG_MAX/2, -SG_MAX/4, SG_MAX/2);
	objs[1] = line(SG_MAX/4, -SG_MAX/2, SG_MAX/4, SG_MAX/2);

	draw(bitmap, icon, map.item());
	if( show ){
		Vector::show(icon);
	}
}

void Vector::draw_button_bar(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){
	int total = 7;
	sg_vector_primitive_t objs[total];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.total = total;
	icon.fill_total = 0;
	objs[0] = Vector::line(SG_MIN/2, 0, SG_MAX/2, 0);
	objs[1] = Vector::line(SG_MIN/2+SG_MAX/32, SG_MAX/4, SG_MAX/2-SG_MAX/32, SG_MAX/4);
	objs[2] = Vector::line(SG_MIN/2, 0, SG_MIN/2, SG_MAX/4 - SG_MAX/32);
	objs[3] = Vector::line(SG_MAX/2, 0, SG_MAX/2, SG_MAX/4 - SG_MAX/32);
	objs[4] = Vector::arc(SG_MAX/2-SG_MAX/32, SG_MAX/4-SG_MAX/32, SG_MAX/32, SG_MAX/32, 0, SG_TRIG_POINTS/4);
	objs[5] = Vector::arc(SG_MIN/2+ SG_MAX/32, SG_MAX/4-SG_MAX/32, SG_MAX/32, SG_MAX/32, SG_TRIG_POINTS/4, SG_TRIG_POINTS*2/4);
	objs[6] = Vector::fill(0, SG_MAX/8);

	draw(bitmap, icon, map.item());
	if( show ){
		Vector::show(icon);
	}
}

void Vector::draw_mic(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){
	int total = 8;
	sg_vector_primitive_t objs[total];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.total = total;
	icon.fill_total = 0;


	objs[0] = Vector::arc(0, -SG_MAX/6, SG_MAX/6, SG_MAX/6, SG_TRIG_POINTS/2, SG_TRIG_POINTS);
	objs[1] = Vector::line(-SG_MAX/6, -SG_MAX/6, -SG_MAX/6, 0);
	objs[2] = Vector::line(SG_MAX/6, -SG_MAX/6, SG_MAX/6, 0);
	objs[3] = Vector::arc(0, 0, SG_MAX/6, SG_MAX/6, 0, SG_TRIG_POINTS/2);
	objs[4] = Vector::arc(0, 0, SG_MAX/3, SG_MAX/3, 0, SG_TRIG_POINTS/2);
	objs[5] = Vector::line(0, SG_MAX/3, 0, SG_MAX/2);
	objs[6] = Vector::line(-SG_MAX/5, SG_MAX/2, SG_MAX/5, SG_MAX/2);
	objs[7] = Vector::fill(0, -SG_MAX/8);

	draw(bitmap, icon, map.item());
	if( show ){
		Vector::show(icon);
	}
}

void Vector::draw_clock(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){
	int total = 3;
	sg_vector_primitive_t objs[total];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.total = total;
	icon.fill_total = 0;


	objs[0] = Vector::circle(0, 0, SG_MAX/2);
	objs[1] = Vector::line(0, 0, SG_MAX/4, 0);
	objs[2] = Vector::line(0, 0, 0, -SG_MAX/3);

	draw(bitmap, icon, map.item());
	if( show ){
		Vector::show(icon);
	}
}

void Vector::draw_heart(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){
	int total = 5;
	sg_vector_primitive_t objs[total];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.total = total;
	icon.fill_total = 0;


	objs[0] = Vector::arc(-SG_MAX/4, 0, SG_MAX/4, SG_MAX/4, SG_TRIG_POINTS*3/8, SG_TRIG_POINTS);
	objs[1] = Vector::arc(SG_MAX/4, 0, SG_MAX/4, SG_MAX/4, SG_TRIG_POINTS/2, SG_TRIG_POINTS + SG_TRIG_POINTS/8);
	objs[2] = Vector::line(-SG_MAX/2 + SG_MAX*1/20, SG_MAX*3/10 - SG_MAX*3/20, 0, SG_MAX/2);
	objs[3] = Vector::line(SG_MAX/2 - SG_MAX*1/20, SG_MAX*3/10 - SG_MAX*3/20, 0, SG_MAX/2);
	objs[4] = Vector::fill(SG_MAX/4, 0);

	draw(bitmap, icon, map.item());
	if( show ){
		Vector::show(icon);
	}
}

void Vector::draw_plot(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){
	int total = 6;
	sg_vector_primitive_t objs[total];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.total = total;
	icon.fill_total = 0;


	objs[0] = Vector::line(-SG_MAX/2, -SG_MAX/2, -SG_MAX/2, SG_MAX/2);
	objs[1] = Vector::line(-SG_MAX/2, SG_MAX/2, SG_MAX/2, SG_MAX/2);
	objs[2] = Vector::line(-SG_MAX/2, SG_MAX/2, -SG_MAX/4, -SG_MAX/3);
	objs[3] = Vector::line(-SG_MAX/4, -SG_MAX/3, 0, 0);
	objs[4] = Vector::line(0, 0, SG_MAX/4, -SG_MAX/4);
	objs[5] = Vector::line(SG_MAX/4, -SG_MAX/4, SG_MAX/2, SG_MAX/4);

	draw(bitmap, icon, map.item());
	if( show ){
		Vector::show(icon);
	}
}

void Vector::draw_bike(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){
	int total = 11;
	sg_vector_primitive_t objs[total];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.total = total;
	icon.fill_total = 0;

	objs[0] = Vector::circle(-SG_MAX/4, 0, SG_MAX/6);
	objs[1] = Vector::circle(SG_MAX/4, 0, SG_MAX/6);
	objs[2] = Vector::line(-SG_MAX/4, 0, 0, 0);
	objs[3] = Vector::line(-SG_MAX/4, 0, -SG_MAX/8, -SG_MAX/4);
	objs[4] = Vector::line(0, 0, SG_MAX*1/8, -SG_MAX/4);
	objs[5] = Vector::line(-SG_MAX/8, -SG_MAX/4, SG_MAX*1/8, -SG_MAX/4);
	objs[6] = Vector::line(SG_MAX/4, 0, SG_MAX*1/8, -SG_MAX/4);
	objs[7] = Vector::line(SG_MAX*1/8, -SG_MAX*3/8, SG_MAX*1/8, -SG_MAX/4);
	objs[8] = Vector::line(-SG_MAX*3/16, -SG_MAX*5/16, -SG_MAX*1/8, -SG_MAX/4);
	objs[9] = Vector::line(SG_MAX*1/16, -SG_MAX*3/8, SG_MAX*3/16, -SG_MAX*3/8);
	objs[10] = Vector::line(-SG_MAX*4/16, -SG_MAX*5/16, -SG_MAX*2/16, -SG_MAX*5/16);

	draw(bitmap, icon, map.item());
	if( show ){
		Vector::show(icon);
	}
}

void Vector::draw_mountain(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){
	int total = 6;
	sg_vector_primitive_t objs[total];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.total = total;
	icon.fill_total = 0;

	objs[0] = Vector::line(-SG_MAX/2, SG_MAX/2, -SG_MAX/4, -SG_MAX/3);
	objs[1] = Vector::line(-SG_MAX/4, -SG_MAX/3, 0, 0);
	objs[2] = Vector::line(0, 0, SG_MAX/4, -SG_MAX/4);
	objs[3] = Vector::line(SG_MAX/4, -SG_MAX/4, SG_MAX/2, SG_MAX/2);
	objs[4] = Vector::line(-SG_MAX/2, SG_MAX/2, SG_MAX/2, SG_MAX/2);
	objs[5] = Vector::fill(SG_MAX/8, 0);

	draw(bitmap, icon, map.item());
	if( show ){
		Vector::show(icon);
	}
}

void Vector::draw_compass(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){
	int total = 6;
	sg_vector_primitive_t objs[total];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.total = total;
	icon.fill_total = 0;


	objs[0] = Vector::line(-SG_MAX/8, 0, SG_MAX/8, 0);
	objs[1] = Vector::line(-SG_MAX/8, 0, 0, -SG_MAX/4);
	objs[2] = Vector::line(SG_MAX/8, 0, 0, -SG_MAX/4);
	objs[3] = Vector::line(-SG_MAX/8, 0, 0, SG_MAX/4);
	objs[4] = Vector::line(SG_MAX/8, 0, 0, SG_MAX/4);
	objs[5] = Vector::fill(0, -SG_MAX/8);

	draw(bitmap, icon, map.item());
	if( show ){
		Vector::show(icon);
	}
}

void Vector::draw_compass_outer(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){
	int total = 9;
	sg_vector_primitive_t objs[total];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.total = total;
	icon.fill_total = 0;


	objs[0] = Vector::circle(0, 0, SG_MAX*4/10);
	objs[1] = Vector::line(SG_MAX*3/10, 0, SG_MAX*5/10, 0);
	objs[2] = Vector::line(-SG_MAX*3/10, 0, -SG_MAX*5/10, 0);
	objs[3] = Vector::line(0, -SG_MAX*3/10, 0, -SG_MAX*5/10);
	objs[4] = Vector::line(0, SG_MAX*3/10, 0, SG_MAX*5/10);
	objs[5] = Vector::line(SG_MAX*5/20, SG_MAX*5/20, SG_MAX*7/20, SG_MAX*7/20);
	objs[6] = Vector::line(-SG_MAX*5/20, SG_MAX*5/20, -SG_MAX*7/20, SG_MAX*7/20);
	objs[7] = Vector::line(SG_MAX*5/20, -SG_MAX*5/20, SG_MAX*7/20, -SG_MAX*7/20);
	objs[8] = Vector::line(-SG_MAX*5/20, -SG_MAX*5/20, -SG_MAX*7/20, -SG_MAX*7/20);

	draw(bitmap, icon, map.item());
	if( show ){
		Vector::show(icon);
	}
}

void Vector::draw_panel(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){
	int total = 8;
	sg_vector_primitive_t objs[total];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.total = total;
	icon.fill_total = 0;

	objs[0] = Vector::arc(-SG_MAX/2+SG_MAX/16, -SG_MAX/2+SG_MAX/16, SG_MAX/16, SG_MAX/16, SG_TRIG_POINTS/2, SG_TRIG_POINTS*3/4);
	objs[1] = Vector::arc(-SG_MAX/2+SG_MAX/16, SG_MAX/2-SG_MAX/16, SG_MAX/16, SG_MAX/16, SG_TRIG_POINTS/4, SG_TRIG_POINTS/2);
	objs[2] = Vector::arc(SG_MAX/2-SG_MAX/16, -SG_MAX/2+SG_MAX/16, SG_MAX/16, SG_MAX/16, SG_TRIG_POINTS*3/4, SG_TRIG_POINTS);
	objs[3] = Vector::arc(SG_MAX/2-SG_MAX/16, SG_MAX/2-SG_MAX/16, SG_MAX/16, SG_MAX/16, 0, SG_TRIG_POINTS/4);
	objs[4] = Vector::line(-SG_MAX/2+SG_MAX/16, -SG_MAX/2, SG_MAX/2-SG_MAX/16, -SG_MAX/2);
	objs[5] = Vector::line(-SG_MAX/2+SG_MAX/16, SG_MAX/2, SG_MAX/2-SG_MAX/16, SG_MAX/2);
	objs[6] = Vector::line(-SG_MAX/2, -SG_MAX/4+SG_MAX/16, -SG_MAX/2, SG_MAX/4-SG_MAX/16);
	objs[7] = Vector::line(SG_MAX/2, -SG_MAX/4+SG_MAX/16, SG_MAX/2, SG_MAX/4-SG_MAX/16);

	draw(bitmap, icon, map.item());
	if( show ){
		Vector::show(icon);
	}
}

void Vector::draw_panel_fill(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){
	int total = 9;
	sg_vector_primitive_t objs[total];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.total = total;
	icon.fill_total = 0;


	objs[0] = Vector::arc(-SG_MAX/2+SG_MAX/16, -SG_MAX/2+SG_MAX/16, SG_MAX/16, SG_MAX/16, SG_TRIG_POINTS/2, SG_TRIG_POINTS*3/4);
	objs[1] = Vector::arc(-SG_MAX/2+SG_MAX/16, SG_MAX/2-SG_MAX/16, SG_MAX/16, SG_MAX/16, SG_TRIG_POINTS/4, SG_TRIG_POINTS/2);
	objs[2] = Vector::arc(SG_MAX/2-SG_MAX/16, -SG_MAX/2+SG_MAX/16, SG_MAX/16, SG_MAX/16, SG_TRIG_POINTS*3/4, SG_TRIG_POINTS);
	objs[3] = Vector::arc(SG_MAX/2-SG_MAX/16, SG_MAX/2-SG_MAX/16, SG_MAX/16, SG_MAX/16, 0, SG_TRIG_POINTS/4);
	objs[4] = Vector::line(-SG_MAX/2+SG_MAX/16, -SG_MAX/2, SG_MAX/2-SG_MAX/16, -SG_MAX/2);
	objs[5] = Vector::line(-SG_MAX/2+SG_MAX/16, SG_MAX/2, SG_MAX/2-SG_MAX/16, SG_MAX/2);
	objs[6] = Vector::line(-SG_MAX/2, -SG_MAX/4+SG_MAX/16, -SG_MAX/2, SG_MAX/4-SG_MAX/16);
	objs[7] = Vector::line(SG_MAX/2, -SG_MAX/4+SG_MAX/16, SG_MAX/2, SG_MAX/4-SG_MAX/16);
	objs[8] = Vector::fill(0, 0);

	draw(bitmap, icon, map.item());
	if( show ){
		Vector::show(icon);
	}
}

void Vector::draw_sun(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){
	int total = 9;
	sg_vector_primitive_t objs[total];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.total = total;
	icon.fill_total = 0;


	objs[0] = Vector::circle(0, 0, SG_MAX*2/10);
	objs[1] = Vector::line(SG_MAX*3/10, 0, SG_MAX*5/10, 0);
	objs[2] = Vector::line(-SG_MAX*3/10, 0, -SG_MAX*5/10, 0);
	objs[3] = Vector::line(0, -SG_MAX*3/10, 0, -SG_MAX*5/10);
	objs[4] = Vector::line(0, SG_MAX*3/10, 0, SG_MAX*5/10);
	objs[5] = Vector::line(SG_MAX*5/20, SG_MAX*5/20, SG_MAX*7/20, SG_MAX*7/20);
	objs[6] = Vector::line(-SG_MAX*5/20, SG_MAX*5/20, -SG_MAX*7/20, SG_MAX*7/20);
	objs[7] = Vector::line(SG_MAX*5/20, -SG_MAX*5/20, SG_MAX*7/20, -SG_MAX*7/20);
	objs[8] = Vector::line(-SG_MAX*5/20, -SG_MAX*5/20, -SG_MAX*7/20, -SG_MAX*7/20);

	draw(bitmap, icon, map.item());
	if( show ){
		Vector::show(icon);
	}
}

void Vector::draw_battery(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){
	int total = 7;
	sg_vector_primitive_t objs[total];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.total = total;
	icon.fill_total = 0;


	objs[0] = Vector::line(-SG_MAX/2, -SG_MAX/4, -SG_MAX/2, SG_MAX/4);
	objs[1] = Vector::line(-SG_MAX/2, -SG_MAX/4, SG_MAX*4/10, -SG_MAX/4);
	objs[2] = Vector::line(-SG_MAX/2, SG_MAX/4, SG_MAX*4/10, SG_MAX/4);
	objs[3] = Vector::line(SG_MAX*4/10, -SG_MAX/4, SG_MAX*4/10, SG_MAX/4);
	objs[4] = Vector::line(SG_MAX*4/10, SG_MAX*1/10, SG_MAX*5/10, SG_MAX*1/10);
	objs[5] = Vector::line(SG_MAX*4/10, -SG_MAX*1/10, SG_MAX*5/10, -SG_MAX*1/10);
	objs[6] = Vector::line(SG_MAX*5/10, -SG_MAX*1/10, SG_MAX*5/10, SG_MAX*1/10);

	draw(bitmap, icon, map.item());
	if( show ){
		Vector::show(icon);
	}
}

void Vector::draw_arrowhead(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){
	int total = 4;
	sg_vector_primitive_t objs[total];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.total = total;
	icon.fill_total = 0;


	objs[0] = Vector::line(0, -SG_MAX/2, SG_MAX/4, SG_MAX/2);
	objs[1] = Vector::line(0, -SG_MAX/2, -SG_MAX/4, SG_MAX/2);
	objs[2] = Vector::line(SG_MAX/4, SG_MAX/2, 0, SG_MAX/4);
	objs[3] = Vector::line(-SG_MAX/4, SG_MAX/2, 0, SG_MAX/4);

	draw(bitmap, icon, map.item());
	if( show ){
		Vector::show(icon);
	}
}

void Vector::draw_dot(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){
	int total = 1;
	sg_vector_primitive_t objs[total];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.total = total;
	icon.fill_total = 0;

	objs[0] = Vector::circle(0, 0, SG_MAX*1/10);

	draw(bitmap, icon, map.item());
	if( show ){
		Vector::show(icon);
	}
}

void Vector::draw_dash(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){
	int total = 1;
	sg_vector_primitive_t objs[total];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.total = total;
	icon.fill_total = 0;

	objs[0] = Vector::line(-SG_MAX/4, 0, SG_MAX/4, 0);

	draw(bitmap, icon, map.item());
	if( show ){
		Vector::show(icon);
	}

}

void Vector::draw_message(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){
	int total = 6;
	sg_vector_primitive_t objs[total];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.total = total;
	icon.fill_total = 0;

	objs[0] = Vector::line(-SG_MAX/2, -SG_MAX/4, SG_MAX/2, -SG_MAX/4);
	objs[1] = Vector::line(-SG_MAX/2, SG_MAX/4, SG_MAX/2, SG_MAX/4);
	objs[2] = Vector::line(SG_MAX/2, -SG_MAX/4, SG_MAX/2, SG_MAX/4);
	objs[3] = Vector::line(-SG_MAX/2, -SG_MAX/4, -SG_MAX/2, SG_MAX/4);
	objs[4] = Vector::line(-SG_MAX/2, -SG_MAX/4, 0, 0);
	objs[5] = Vector::line(0, 0, SG_MAX/2, -SG_MAX/4);

	draw(bitmap, icon, map.item());
	if( show ){
		Vector::show(icon);
	}

}

void Vector::draw_ALPHA(Bitmap & bitmap, const VectorMap & map, sg_bounds_t * bounds, bool show){

	sg_vector_primitive_t objs[6];
	sg_vector_icon_t icon;
	icon.primitives = objs;
	icon.fill_total = 0;

	//A
	icon.total = 3;
	objs[0] = Vector::line(-SG_MAX/4, SG_MAX/2, 0, -SG_MAX/2);
	objs[1] = Vector::line(SG_MAX/4, SG_MAX/2, 0, -SG_MAX/2);
	objs[2] = Vector::line(-SG_MAX/8, 0, SG_MAX/8, 0);
	//draw(bitmap, icon, map.item());
	if( show ){ Vector::show(icon); }

	//B
	icon.total = 6;

	//C
	icon.total = 3;
	objs[0] = Vector::line(-SG_MAX/4, -SG_MAX/4, -SG_MAX/4, SG_MAX/4);
	objs[1] = Vector::arc(0,SG_MAX/4,SG_MAX/4,0,0,SG_TRIG_POINTS/2);
	objs[2] = Vector::arc(0,-SG_MAX/4,SG_MAX/4,SG_MAX/4,SG_TRIG_POINTS/2,SG_TRIG_POINTS);
	//draw(bitmap, icon, map.item());
	if( show ){ Vector::show(icon); }

	//D
	icon.total = 2;

	//E
	icon.total = 4;
	objs[0] = Vector::line(-SG_MAX/4, SG_MAX/2, SG_MAX/4, SG_MAX/2);
	objs[1] = Vector::line(-SG_MAX/4, 0, 0, 0);
	objs[2] = Vector::line(-SG_MAX/4, -SG_MAX/2, SG_MAX/4, -SG_MAX/2);
	objs[3] = Vector::line(-SG_MAX/4, -SG_MAX/2, -SG_MAX/4, SG_MAX/2);
	//draw(bitmap, icon, map.item());
	if( show ){ Vector::show(icon); }

	//F
	icon.total = 3;
	objs[0] = Vector::line(-SG_MAX/4, -SG_MAX/2, SG_MAX/4, -SG_MAX/2);
	objs[1] = Vector::line(-SG_MAX/4, 0, 0, 0);
	objs[2] = Vector::line(-SG_MAX/4, -SG_MAX/2, -SG_MAX/4, SG_MAX/2);
	//draw(bitmap, icon, map.item());
	if( show ){ Vector::show(icon); }

	//G
	icon.total = 5;

	//H
	icon.total = 3;
	objs[0] = Vector::line(-SG_MAX/4, 0, SG_MAX/4, 0);
	objs[1] = Vector::line(SG_MAX/4, -SG_MAX/2, SG_MAX/4, SG_MAX/2);
	objs[2] = Vector::line(-SG_MAX/4, -SG_MAX/2, -SG_MAX/4, SG_MAX/2);
	//draw(bitmap, icon, map.item());
	if( show ){ Vector::show(icon); }

	//I
	icon.total = 3;
	objs[0] = Vector::line(-SG_MAX/4, -SG_MAX/2, SG_MAX/4, -SG_MAX/2);
	objs[1] = Vector::line(-SG_MAX/4, SG_MAX/2, SG_MAX/4, SG_MAX/2);
	objs[2] = Vector::line(0, -SG_MAX/2, 0, SG_MAX/2);
	//draw(bitmap, icon, map.item());
	if( show ){ Vector::show(icon); }

	//J
	icon.total = 2;

	//K
	icon.total = 3;
	objs[0] = Vector::line(-SG_MAX/4, -SG_MAX/2, -SG_MAX/4, SG_MAX/2);
	objs[1] = Vector::line(-SG_MAX/4, 0, SG_MAX/4, SG_MAX/2);
	objs[2] = Vector::line(-SG_MAX/4, 0, SG_MAX/4, -SG_MAX/2);
	//draw(bitmap, icon, map.item());
	if( show ){ Vector::show(icon); }

	//L
	icon.total = 2;
	objs[0] = Vector::line(-SG_MAX/4, -SG_MAX/2, -SG_MAX/4, SG_MAX/2);
	objs[1] = Vector::line(-SG_MAX/4, SG_MAX/2, SG_MAX/4, SG_MAX/2);
	//draw(bitmap, icon, map.item());
	if( show ){ Vector::show(icon); }

	//M
	icon.total = 4;
	objs[0] = Vector::line(-SG_MAX/4, SG_MAX/2, -SG_MAX/4, -SG_MAX/2);
	objs[1] = Vector::line(SG_MAX/4, SG_MAX/2, SG_MAX/4, -SG_MAX/2);
	objs[2] = Vector::line(-SG_MAX/4, -SG_MAX/2, 0, SG_MAX/4);
	objs[3] = Vector::line(SG_MAX/4, -SG_MAX/2, 0, SG_MAX/4);
	//draw(bitmap, icon, map.item());
	if( show ){ Vector::show(icon); }

	//N
	icon.total = 3;
	objs[0] = Vector::line(-SG_MAX/4, SG_MAX/2, -SG_MAX/4, -SG_MAX/2);
	objs[1] = Vector::line(SG_MAX/4, SG_MAX/2, SG_MAX/4, -SG_MAX/2);
	objs[2] = Vector::line(-SG_MAX/4, -SG_MAX/2, SG_MAX/4, SG_MAX/2);
	//draw(bitmap, icon, map.item());
	if( show ){ Vector::show(icon); }

	//O
	icon.total = 4;
	objs[0] = Vector::line(-SG_MAX/4, -SG_MAX/4, -SG_MAX/4, SG_MAX/4);
	objs[1] = Vector::line(SG_MAX/4, -SG_MAX/4, SG_MAX/4, SG_MAX/4);
	objs[2] = Vector::arc(0,SG_MAX/4,SG_MAX/4,SG_MAX/4,0,SG_TRIG_POINTS/2);
	objs[3] = Vector::arc(0,-SG_MAX/4,SG_MAX/4,SG_MAX/4,SG_TRIG_POINTS/2,SG_TRIG_POINTS);
	//draw(bitmap, icon, map.item());
	if( show ){ Vector::show(icon); }

	//P
	icon.total = 4;
	objs[0] = Vector::line(-SG_MAX/4, -SG_MAX/2, -SG_MAX/4, SG_MAX/2);
	objs[1] = Vector::line(-SG_MAX/4, -SG_MAX/2, 0, -SG_MAX/2);
	objs[2] = Vector::line(-SG_MAX/4, 0, 0, 0);
	objs[3] = Vector::arc(0,-SG_MAX/4,SG_MAX/4,SG_MAX/4,SG_TRIG_POINTS*3/4,SG_TRIG_POINTS*5/4);
	//draw(bitmap, icon, map.item());
	if( show ){ Vector::show(icon); }

	//Q
	icon.total = 5;
	objs[0] = Vector::line(-SG_MAX/4, -SG_MAX/4, -SG_MAX/4, SG_MAX/4);
	objs[1] = Vector::line(SG_MAX/4, -SG_MAX/4, SG_MAX/4, SG_MAX/4);
	objs[2] = Vector::arc(0,SG_MAX/4,SG_MAX/4,SG_MAX/4,0,SG_TRIG_POINTS/2);
	objs[3] = Vector::arc(0,-SG_MAX/4,SG_MAX/4,SG_MAX/4,SG_TRIG_POINTS/2,SG_TRIG_POINTS);
	objs[4] = Vector::line(SG_MAX/4, SG_MAX/4, SG_MAX/4, SG_MAX/2);
	//draw(bitmap, icon, map.item());
	if( show ){ Vector::show(icon); }

	//R
	icon.total = 5;
	objs[0] = Vector::line(-SG_MAX/4, -SG_MAX/2, -SG_MAX/4, SG_MAX/2);
	objs[1] = Vector::line(-SG_MAX/4, -SG_MAX/2, 0, -SG_MAX/2);
	objs[2] = Vector::line(-SG_MAX/4, 0, 0, 0);
	objs[3] = Vector::arc(0,-SG_MAX/4,SG_MAX/4,SG_MAX/4,SG_TRIG_POINTS*3/4,SG_TRIG_POINTS*5/4);
	objs[4] = Vector::line(0, 0, SG_MAX/4, SG_MAX/2);
	//draw(bitmap, icon, map.item());
	if( show ){ Vector::show(icon); }


	//S
	icon.total = 2;
	objs[0] = Vector::arc(0,-SG_MAX/4,SG_MAX/4,SG_MAX/4,SG_TRIG_POINTS/4,SG_TRIG_POINTS);
	objs[1] = Vector::arc(0,SG_MAX/4,SG_MAX/4,SG_MAX/4,SG_TRIG_POINTS*3/4,SG_TRIG_POINTS*3/2);
	draw(bitmap, icon, map.item());
	if( show ){ Vector::show(icon); }

	return;

	//T
	icon.total = 2;
	objs[0] = Vector::line(-SG_MAX/4, SG_MAX/2, SG_MAX/4, SG_MAX/2);
	objs[1] = Vector::line(0, SG_MAX/2, 0, -SG_MAX/2);
	draw(bitmap, icon, map.item());
	if( show ){ Vector::show(icon); }

	//U
	icon.total = 3;
	objs[0] = Vector::line(-SG_MAX/4, -SG_MAX/4, -SG_MAX/4, SG_MAX/2);
	objs[1] = Vector::line(SG_MAX/4, -SG_MAX/4, SG_MAX/4, SG_MAX/2);
	objs[2] = Vector::arc(0,-SG_MAX/4,0,0,SG_TRIG_POINTS/2,SG_TRIG_POINTS);
	draw(bitmap, icon, map.item());
	if( show ){ Vector::show(icon); }

	//V
	icon.total = 2;
	objs[0] = Vector::line(-SG_MAX/4, SG_MAX/2, 0, -SG_MAX/2);
	objs[1] = Vector::line(SG_MAX/4, SG_MAX/2, 0, -SG_MAX/2);
	draw(bitmap, icon, map.item());
	if( show ){ Vector::show(icon); }

	//W
	icon.total = 4;
	objs[0] = Vector::line(-SG_MAX/4, -SG_MAX/2, -SG_MAX/4, SG_MAX/2);
	objs[1] = Vector::line(SG_MAX/4, -SG_MAX/2, SG_MAX/4, SG_MAX/2);
	objs[2] = Vector::line(-SG_MAX/4, -SG_MAX/2, 0, SG_MAX/4);
	objs[3] = Vector::line(SG_MAX/4, -SG_MAX/2, 0, SG_MAX/4);
	draw(bitmap, icon, map.item());
	if( show ){ Vector::show(icon); }

	//X
	icon.total = 2;
	objs[0] = Vector::line(-SG_MAX/4, -SG_MAX/2, SG_MAX/4, SG_MAX/2);
	objs[1] = Vector::line(-SG_MAX/4, SG_MAX/2, SG_MAX/4, -SG_MAX/2);
	draw(bitmap, icon, map.item());
	if( show ){ Vector::show(icon); }

	//Y
	icon.total = 3;
	objs[0] = Vector::line(-SG_MAX/4, -SG_MAX/2, 0, 0);
	objs[1] = Vector::line(SG_MAX/4, SG_MAX/2, 0, 0);
	objs[2] = Vector::line(0, 0, 0, -SG_MAX/2);
	draw(bitmap, icon, map.item());
	if( show ){ Vector::show(icon); }

	//Z
	icon.total= 3;
	objs[0] = Vector::line(-SG_MAX/4, -SG_MAX/2, SG_MAX/4, -SG_MAX/2);
	objs[1] = Vector::line(-SG_MAX/4, SG_MAX/2, SG_MAX/4, SG_MAX/2);
	objs[2] = Vector::line(-SG_MAX/4, -SG_MAX/2, SG_MAX/4, SG_MAX/2);
	draw(bitmap, icon, map.item());
	if( show ){ Vector::show(icon); }
}







