/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef MPOINT_HPP_
#define MPOINT_HPP_

#include <stfy/mg.h>

namespace draw {

class MDim {
public:
	MDim(){ v.h = 0; v.w = 0; }
	MDim(mg_size_t w, mg_size_t h){ v.w = w; v.h = h; }
	MDim(mg_dim_t d){ v = d; }

	operator mg_dim_t() const { return v; }
	mg_dim_t dim() const { return v; }

	inline void set(mg_size_t w, mg_size_t h){ v.w = w; v.h = h; }
	inline void set_w(mg_size_t w){ v.w = w; }
	inline void set_h(mg_size_t h){ v.h = h; }

	inline u32 size() const { return v.w * v.h; }

	inline mg_size_t w() const { return v.w; }
	inline mg_size_t h() const { return v.h; }

private:
	mg_dim_t v;
};

/*
 *
 */
class MPoint {
public:
	MPoint(){ d.x = 0; d.y = 0; }
	//MPoint(const MPoint & p ){ d = p; }
	MPoint(mg_point_t p){ d = p; }
	MPoint(const mg_t & a);
	MPoint(mg_int_t x, mg_int_t y){ d.x = x; d.y = y; }

	mg_point_t point() const { return d; }

	operator mg_point_t(){ return d; }


	inline void set(mg_int_t x, mg_int_t y){ d.x = x; d.y = y; }

	inline mg_int_t x() const { return d.x; }
	inline mg_int_t y() const { return d.y; }


	void map(const mg_map_t & m){ mg_point_map(&d, &m); }

	static mg_size_t map_pixel_size(const mg_map_t & m){ return mg_point_map_pixel_size(&m); }

	MPoint & operator=(const mg_point_t & a){ d = a; return *this; }
	MPoint & operator+(const mg_point_t & a){ mg_point_shift(&d, a); return *this; }
	MPoint & operator-(const mg_point_t & a){ mg_point_subtract(&d, &a); return *this; }

	void rotate(i16 angle){ mg_point_rotate(&d, angle); }
	void scale(u16 a){ mg_point_scale(&d, a); }
	void shift(i16 x, i16 y){ mg_point_shift(&d, mg_point(x,y)); }
	void shift(mg_point_t p){ mg_point_shift(&d, p); }

private:
	mg_point_t d;
};

class MObject {
public:
	MObject(){}
	MObject(mg_int_t x, mg_int_t y, mg_size_t w, mg_size_t h){
		_object.point.x = x;
		_object.point.y = y;
		_object.dim.w = w;
		_object.dim.h = h;
	}

	MObject(const MPoint & point, const MDim & dim){
		_object.point = point.point();
		_object.dim = dim.dim();
	}

	mg_object_t object() const { return _object; }

	operator mg_object_t(){ return _object; }

	inline mg_int_t x() const { return _object.point.x; }
	inline mg_int_t y() const { return _object.point.y; }
	inline mg_int_t w() const { return _object.dim.w; }
	inline mg_int_t h() const { return _object.dim.h; }


private:
	mg_object_t _object;
};

};




#endif /* MPOINT_HPP_ */
