//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "sgfx/Vector.hpp"
#include <cstdio>

using namespace sgfx;


void VectorMap::set_region(const sg_region_t & region, s16 rotation){
	data()->region = region;
	data()->rotation = rotation;
}

void VectorMap::set_dim(const sg_dim_t & dim){
	data()->region.dim = dim;
}

void VectorMap::set_point(const sg_point_t & p){
	data()->region.point = p;
}

void VectorMap::set_point(sg_int_t x, sg_int_t y){
	data()->region.point.x = x;
	data()->region.point.y = y;
}


void VectorMap::set_dim(sg_size_t w, sg_size_t h){
	data()->region.dim.width = w;
	data()->region.dim.height = h;
}

VectorMap::VectorMap(const Bitmap & bitmap, s16 rotation){
	set_bitmap_center(bitmap, rotation);
}

void VectorMap::set_bitmap_center(const Bitmap & bitmap, s16 rotation){
	u8 thickness = bitmap.pen_thickness();
	u32 max_width;
	u32 max_height;
	s32 tmp;
	max_width = (bitmap.width() - bitmap.margin_left() - bitmap.margin_right()) * 1414UL / 1000UL ;
	max_height = (bitmap.height() - bitmap.margin_top() - bitmap.margin_bottom()) * 1414UL / 1000UL;
	data()->region.dim.width = max_width - 2*thickness;
	data()->region.dim.height = max_height - 2*thickness;
	tmp = bitmap.width() - max_width + 1;
	data()->region.point.x = tmp/2;
	tmp = bitmap.height() - max_height + 1;
	data()->region.point.y = tmp/2;
	data()->rotation = rotation;
}

void VectorMap::fill_region(const sg_region_t & region, s16 rotation){
	u32 max_width;
	u32 max_height;
	s32 tmp;
	max_width = (region.dim.width) * 1414UL / 1000UL ;
	max_height = (region.dim.height) * 1414UL / 1000UL;
	data()->region.dim.width = max_width;
	data()->region.dim.height = max_height;
	tmp = region.dim.width - max_width + 1;
	data()->region.point.x = region.point.x + tmp/2;
	tmp = region.dim.height - max_height + 1;
	data()->region.point.y = region.point.y + tmp/2;
	data()->rotation = rotation;
}


void Vector::draw(Bitmap & bitmap, const sg_vector_icon_t & icon, const sg_vector_map_t & map, sg_region_t * bounds){
	api()->vector_draw_icon(bitmap.bmap(), &icon, &map, bounds);
}

void Vector::draw_path(Bitmap & bitmap, sg_vector_path_t & path, const sg_vector_map_t & map){
	api()->vector_draw_path(bitmap.bmap(), &path, &map);
}

sg_vector_primitive_t Vector::line(const Point & p1, const Point & p2){
	sg_vector_primitive_t ret;
	memset(&ret, 0, sizeof(ret));
	ret.type = SG_LINE | SG_ENABLE_FLAG;
	ret.line.p1 = p1;
	ret.line.p2 = p2;
	return ret;
}

sg_vector_primitive_t Vector::cubic_bezier(const Point & p1, const Point & p2, const Point & p3, const Point & p4){
	sg_vector_primitive_t ret;
	memset(&ret, 0, sizeof(ret));
	ret.type = SG_CUBIC_BEZIER | SG_ENABLE_FLAG;
	ret.cubic_bezier.p1 = p1;
	ret.cubic_bezier.p2 = p2;
	ret.cubic_bezier.p3 = p3;
	ret.cubic_bezier.p4 = p4;
	return ret;
}

sg_vector_primitive_t Vector::quadratic_bezier(const Point & p1, const Point & p2, const Point & p3){
	sg_vector_primitive_t ret;
	memset(&ret, 0, sizeof(ret));
	ret.type = SG_QUADRATIC_BEZIER | SG_ENABLE_FLAG;
	ret.quadratic_bezier.p1 = p1;
	ret.quadratic_bezier.p2 = p2;
	ret.quadratic_bezier.p3 = p3;
	return ret;
}


sg_vector_primitive_t Vector::circle(const Point & p, sg_size_t r){
	sg_vector_primitive_t ret;
	memset(&ret, 0, sizeof(ret));
	ret.type = SG_ARC | SG_ENABLE_FLAG;
	ret.arc.center = p;
	ret.arc.rx = r;
	ret.arc.ry = r;
	ret.arc.start = 0;
	ret.arc.stop = SG_TRIG_POINTS;
	return ret;
}

sg_vector_primitive_t Vector::arc(const Point & p, sg_size_t rx, sg_size_t ry, sg_int_t start, sg_int_t stop, sg_int_t rotation){
	sg_vector_primitive_t ret;
	memset(&ret, 0, sizeof(ret));
	ret.type = SG_ARC | SG_ENABLE_FLAG;
	ret.arc.center = p;
	ret.arc.rx = rx;
	ret.arc.ry = ry;
	ret.arc.start = start;
	ret.arc.stop = stop;
	ret.arc.rotation = rotation;
	return ret;
}

sg_vector_primitive_t Vector::fill(const Point & p){
	sg_vector_primitive_t ret;
	memset(&ret, 0, sizeof(ret));
	ret.type = SG_POUR | SG_ENABLE_FLAG;
	ret.pour.center = p;
	return ret;
}

sg_vector_path_description_t Vector::get_path_move(const Point & p){
	sg_vector_path_description_t ret;
	ret.type = SG_VECTOR_PATH_MOVE;
	ret.move.point = p;
	return ret;
}

sg_vector_path_description_t Vector::get_path_line(const Point & p){
	sg_vector_path_description_t ret;
	ret.type = SG_VECTOR_PATH_LINE;
	ret.line.point = p;
	return ret;
}

sg_vector_path_description_t Vector::get_path_pour(const Point & p){
	sg_vector_path_description_t ret;
	ret.type = SG_VECTOR_PATH_POUR;
	ret.pour.point = p;
	return ret;
}

sg_vector_path_description_t Vector::get_path_quadratic_bezier(const Point & control, const Point & point){
	sg_vector_path_description_t ret;
	ret.type = SG_VECTOR_PATH_QUADRATIC_BEZIER;
	ret.quadratic_bezier.control = control;
	ret.quadratic_bezier.point = point;
	return ret;
}

sg_vector_path_description_t Vector::get_path_cubic_bezier(const Point & control0, const Point & control1, const Point & point){
	sg_vector_path_description_t ret;
	ret.type = SG_VECTOR_PATH_CUBIC_BEZIER;
	ret.cubic_bezier.control[0] = control0;
	ret.cubic_bezier.control[1] = control1;
	ret.cubic_bezier.point = point;
	return ret;
}

sg_vector_path_description_t Vector::get_path_close(){
	sg_vector_path_description_t ret;
	ret.type = SG_VECTOR_PATH_CLOSE;
	return ret;
}


void Vector::show(const sg_vector_primitive_t & mg){
	u32 i;
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


Region Vector::find_active_region(const Bitmap & bitmap){
	sg_region_t region;
	sg_int_t right, bottom;

	region.point.x = find_left(bitmap);
	region.point.y = find_top(bitmap);

	right = find_right(bitmap);
	bottom = find_bottom(bitmap);

	region.dim.width = right - region.point.x;
	region.dim.height = bottom - region.point.y;

	return region;
}

sg_int_t Vector::find_top(const Bitmap & bitmap){
	sg_int_t x, y;
	sg_int_t y_min = bitmap.height();

	for(x = bitmap.margin_left(); x < bitmap.width() - bitmap.margin_left() - bitmap.margin_right(); x++){
		y = bitmap.margin_top();
		while( (bitmap.get_pixel(sg_point(x,y)) == 0) && (y < bitmap.height() - bitmap.margin_top() - bitmap.margin_bottom()) ){
			y++;
		}
		if( y < y_min ){
			y_min = y;
		}
	}
	return y_min;
}

sg_int_t Vector::find_bottom(const Bitmap & bitmap){
	sg_int_t x, y;
	sg_int_t y_max = 0;

	for(x = bitmap.margin_left(); x < bitmap.width() - bitmap.margin_left() - bitmap.margin_right(); x++){
		y = bitmap.height() - bitmap.margin_bottom() - bitmap.margin_top();
		do{
			y--;
		} while( (bitmap.get_pixel(sg_point(x,y)) == 0) && (y > bitmap.margin_top()) );

		if( y > y_max ){
			y_max = y;
		}
	}
	return y_max;
}

sg_int_t Vector::find_left(const Bitmap & bitmap){
	sg_int_t x, y;
	sg_int_t x_min = bitmap.width();

	for(y = 0; y < bitmap.height(); y++){
		x = bitmap.margin_left();
		while( (bitmap.get_pixel(sg_point(x,y)) == 0) && (x < bitmap.width() - bitmap.margin_left() - bitmap.margin_right()) ){
			x++;
		}
		if( x < x_min ){
			x_min = x;
		}
	}
	return x_min;
}

sg_int_t Vector::find_right(const Bitmap & bitmap){
	sg_int_t x, y;
	sg_int_t x_max = 0;

	for(y = 0; y < bitmap.height(); y++){
		x = bitmap.width() - bitmap.margin_left() - bitmap.margin_right();
		do{
			x--;
		} while( (bitmap.get_pixel(sg_point(x,y)) == 0) && (x > bitmap.margin_left()) );

		if( x > x_max ){
			x_max = x;
		}
	}
	return x_max;
}


void Vector::draw_remove(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){

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

void Vector::draw_ok(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){
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

void Vector::draw_bars(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){
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

void Vector::draw_circle(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){
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

void Vector::draw_circle_fill(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){
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

void Vector::draw_toggle_off(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){
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

void Vector::draw_toggle_on(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){
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

void Vector::draw_power(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){
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

void Vector::draw_chevron(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){
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

void Vector::draw_arrow(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){
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

void Vector::draw_zoom(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){
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

void Vector::draw_reset(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){
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

void Vector::draw_lightning(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){
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

void Vector::draw_play(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){
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


void Vector::draw_pause(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){
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

void Vector::draw_button_bar(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){
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

void Vector::draw_mic(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){
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

void Vector::draw_clock(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){
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

void Vector::draw_heart(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){
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

void Vector::draw_plot(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){
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

void Vector::draw_bike(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){
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

void Vector::draw_mountain(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){
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

void Vector::draw_compass(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){
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

void Vector::draw_compass_outer(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){
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

void Vector::draw_panel(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){
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

void Vector::draw_panel_fill(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){
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

void Vector::draw_sun(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){
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

void Vector::draw_battery(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){
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

void Vector::draw_arrowhead(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){
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

void Vector::draw_dot(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){
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

void Vector::draw_dash(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){
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

void Vector::draw_message(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){
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

void Vector::draw_ALPHA(Bitmap & bitmap, const VectorMap & map, sg_region_t * bounds, bool show){

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







