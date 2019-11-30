/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_DRAW_GRAPH_HPP_
#define SAPI_DRAW_GRAPH_HPP_

#include "Drawing.hpp"
#include "../var/String.hpp"

namespace draw {

/*! \brief Data Set Class
 */
class DataSet : public api::InfoObject {
public:

	/*! \brief Construct an empy data set */
	DataSet(){ set(0,0); }

	/*! \brief Construct a data set with \a data and \a n_data points */
	DataSet(float * data, u32 n_data){ set(data,n_data); }

	/*! \brief Returns the data at point \a i */
	virtual float at(u32 i) const {
		if( i < size() ){
			return m_data[i];
		}
		return i * 1.0;
	}

	/*! \brief Returns the number of data points in the set */
	virtual u32 size() const { return m_size; };

	/*! \brief Set the data pointer and number of points
	 *
	 * \details This sets the data to use in the graph.  The caller
	 * needs to manage the memory for the data points.  This object
	 * simply has a pointer to the external data.
	 *
	 * @param data A pointer to the data
	 * @param n The number of data points
	 */
	void set(const float * data, u32 n){
		m_data = data; m_size = n;
	}

	/*! \brief Returns the maximum value in the set */
	float max() const;
	/*! \brief Retuns the minimum value in the set */
	float min() const;

private:
	const float * m_data;
	u32 m_size;
};

/*! \brief Graph Axis Class
 *
 */
class Axis : public api::InfoObject {
public:
	Axis(){ m_min = 0.0; m_max = 10.0; m_tick = 1.0; }
	Axis(float min, float max, const var::String & label){
		m_min = min; m_max = max; m_tick = (max - min) / 10.0;
		m_label = label;
	}

	inline const char * label() const { return m_label.cstring(); }


	inline void set(float min, float max, float tick = -1.0){
		m_min = min; m_max = max;
		if( tick < 0 ){
			m_tick = (max-min)/10.0;
		}
	}
	inline void set_min(float value){ m_min = value; }
	inline void set_max(float value){ m_max = value; }

	inline float min() const { return m_min; }
	inline float max() const { return m_max; }
	float range() const;

private:
	float m_min, m_max;
	float m_tick;
	var::String m_label;
};

/*! \brief Abstract Graph Class
 */
class Graph : public Drawing {
public:
	Graph();

	/*! \brief Return a reference to the graph's X axis */
	Axis & x_axis(){ return m_x_axis; }
	/*! \brief Return a reference to the graph's Y axis */
	Axis & y_axis(){ return m_y_axis; }
	/*! \brief Return a reference to the graph's X data */
	DataSet & x_data(){ return m_x_data; }
	/*! \brief Return a reference to the graph's Y data */
	DataSet & y_data(){ return m_y_data; }

protected:
	sgfx::Point point_on_bitmap(sgfx::Bitmap * b, float x, float y, const sgfx::Area & d);

private:
	Axis m_x_axis;
	Axis m_y_axis;
	DataSet m_x_data;
	DataSet m_y_data;

};

}

#endif /* SAPI_DRAW_GRAPH_HPP_ */
