//Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#include "sgfx/Vector.hpp"
#include <cstdio>

using namespace sgfx;


VectorMap& VectorMap::set_region(const Region & region){
	m_value.region = region;
	return *this;
}

VectorMap& VectorMap::calculate_for_bitmap(const Bitmap & bitmap){
	return calculate_for_region(bitmap.region());
}

VectorMap& VectorMap::calculate_for_region(const Region & region){
	u32 max_width;
	u32 max_height;
	s32 tmp;
	max_width = (region.width()) * 1414UL / 1000UL ;
	max_height = (region.height()) * 1414UL / 1000UL;
	m_value.region.area.width = max_width;
	m_value.region.area.height = max_height;
	tmp = region.width() - max_width + 1;
	m_value.region.point.x = region.x() + tmp/2;
	tmp = region.height() - max_height + 1;
	m_value.region.point.y = region.y() + tmp/2;
	return *this;
}

void VectorPath::shift(Point shift){
	u32 i;
	for(i=0; i < m_path.icon.count; i++){
		sg_vector_path_description_t * description = (sg_vector_path_description_t *)m_path.icon.list + i;
		switch(description->type){
			case SG_VECTOR_PATH_MOVE:
				description->move.point = Point(description->move.point) + shift;
				break;
			case SG_VECTOR_PATH_LINE:
				description->line.point = Point(description->line.point) + shift;
				break;
			case SG_VECTOR_PATH_POUR:
				description->pour.point = Point(description->pour.point) + shift;
				break;
			case SG_VECTOR_PATH_QUADRATIC_BEZIER:
				description->quadratic_bezier.control = Point(description->quadratic_bezier.control) + shift;
				description->quadratic_bezier.point = Point(description->quadratic_bezier.point) + shift;
				break;
			case SG_VECTOR_PATH_CUBIC_BEZIER:
				description->cubic_bezier.control[0] = Point(description->cubic_bezier.control[0]) + shift;
				description->cubic_bezier.control[1] = Point(description->cubic_bezier.control[1]) + shift;
				description->cubic_bezier.point = Point(description->cubic_bezier.point) + shift;
				break;
			case SG_VECTOR_PATH_CLOSE:
				break;
		}
	}
}

void VectorPath::scale(float x_scale, float y_scale){
	for(u32 i=0; i < m_path.icon.count; i++){
		sg_vector_path_description_t * description = (sg_vector_path_description_t *)m_path.icon.list + i;
		switch(description->type){
			case SG_VECTOR_PATH_MOVE:
				description->move.point.x = description->move.point.x * x_scale;
				description->move.point.y = description->move.point.y * y_scale;
				break;
			case SG_VECTOR_PATH_LINE:
				description->line.point.x = description->line.point.x * x_scale;
				description->line.point.y = description->line.point.y * y_scale;
				break;
			case SG_VECTOR_PATH_POUR:
				description->pour.point.x = description->pour.point.x * x_scale;
				description->pour.point.y = description->pour.point.y * y_scale;
				break;
			case SG_VECTOR_PATH_QUADRATIC_BEZIER:
				description->quadratic_bezier.control.x = description->quadratic_bezier.control.x * x_scale;
				description->quadratic_bezier.point.x = description->quadratic_bezier.point.x * x_scale;
				description->quadratic_bezier.control.y = description->quadratic_bezier.control.y * y_scale;
				description->quadratic_bezier.point.y = description->quadratic_bezier.point.y * y_scale;
				break;
			case SG_VECTOR_PATH_CUBIC_BEZIER:
				description->cubic_bezier.control[0].x = description->cubic_bezier.control[0].x * x_scale;
				description->cubic_bezier.control[1].x = description->cubic_bezier.control[1].x * x_scale;
				description->cubic_bezier.point.x = description->cubic_bezier.point.x * x_scale;
				description->cubic_bezier.control[0].y = description->cubic_bezier.control[0].y * y_scale;
				description->cubic_bezier.control[1].y = description->cubic_bezier.control[1].y * y_scale;
				description->cubic_bezier.point.y = description->cubic_bezier.point.y * y_scale;
				break;
			case SG_VECTOR_PATH_CLOSE:
				break;
		}
	}
}


void VectorPath::scale(float scale){
	u32 i;
	for(i=0; i < m_path.icon.count; i++){
		sg_vector_path_description_t * description = (sg_vector_path_description_t *)m_path.icon.list + i;
		switch(description->type){
			case SG_VECTOR_PATH_MOVE:
				description->move.point = Point(description->move.point) * scale;
				break;
			case SG_VECTOR_PATH_LINE:
				description->line.point = Point(description->line.point) * scale;
				break;
			case SG_VECTOR_PATH_POUR:
				description->pour.point = Point(description->pour.point) * scale;
				break;
			case SG_VECTOR_PATH_QUADRATIC_BEZIER:
				description->quadratic_bezier.control = Point(description->quadratic_bezier.control) * scale;
				description->quadratic_bezier.point = Point(description->quadratic_bezier.point) * scale;
				break;
			case SG_VECTOR_PATH_CUBIC_BEZIER:
				description->cubic_bezier.control[0] = Point(description->cubic_bezier.control[0]) * scale;
				description->cubic_bezier.control[1] = Point(description->cubic_bezier.control[1]) * scale;
				description->cubic_bezier.point = Point(description->cubic_bezier.point) * scale;
				break;
			case SG_VECTOR_PATH_CLOSE:
				break;
		}
	}
}

void Vector::draw(Bitmap & bitmap, VectorPath & path, const VectorMap & map){
	api()->vector_draw_path(bitmap.bmap(), &path.path(), &map.map());
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


Region Vector::find_active_region(const Bitmap & bitmap){
	sg_region_t region;
	sg_int_t right, bottom;

	region.point.x = find_left(bitmap);
	region.point.y = find_top(bitmap);

	right = find_right(bitmap);
	bottom = find_bottom(bitmap);

	region.area.width = right - region.point.x;
	region.area.height = bottom - region.point.y;

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








