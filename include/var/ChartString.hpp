/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved


#ifndef VAR_CHARTSTRING_HPP_
#define VAR_CHARTSTRING_HPP_

#include "JsonString.hpp"

namespace var {

class ChartString : public JsonString {
public:
	ChartString();

	void set_point(int x, int y);
	void set_pointf(float x, float y);

	void start_points();
	void end_points();

	void append_point(int x, int y);
	void append_pointf(float x, float y);

	static const char * x_key(){ return "x"; }
	static const char * y_key(){ return "y"; }
	static const char * point_key(){ return "point"; }
	static const char * points_key(){ return "points"; }
};

} /* namespace var */

#endif /* VAR_CHARTSTRING_HPP_ */
