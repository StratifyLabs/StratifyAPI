/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved


#ifndef VAR_CHARTMESSAGE_HPP_
#define VAR_CHARTMESSAGE_HPP_

#include "../hal/Dev.hpp"
#include "String.hpp"
#include "Message.hpp"

namespace var {


class ChartMessage : public Message {
public:

	ChartMessage(const hal::Dev & device, int max_capacity);

	//methods for adding data to the chart
	virtual int create();

	int write_title(const String & str);
	int read_title(String & str);

	enum {
		AXIS_WHICH_X,
		AXIS_WHICH_Y
	};

	int write_axis(const String & str, int which, int min, int max);
	int read_axis(String & str, int which, int & min, int & max);

	int write_axis(const String & str, int which, float min, float max);
	int read_axis(String & str, int which, float & min, float & max);

	int write_xy(const int * x, const int * y, int samples);
	int read_xy(int * x, int * y, int samples);

	int write_xy(const float * x, const float * y, int samples);
	int read_xy(float * x, float * y, int samples);

private:
	int write_point(int x, int y);
	int write_point(float x, float y);
};

};



#endif /* VAR_CHARTMESSAGE_HPP_ */
