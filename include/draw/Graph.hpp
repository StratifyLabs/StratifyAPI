/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#ifndef SAPI_DRAW_GRAPH_HPP_
#define SAPI_DRAW_GRAPH_HPP_

#include "Drawing.hpp"
#include "var/String.hpp"
#include "../var/Vector.hpp"

namespace draw {


class YData : public var::Vector<float> {
public:

   YData & set_axis_index(u32 value){
      m_axis_index = value;
      return *this;
   }

   YData & set_color(sg_color_t value){
      m_color = value;
      return *this;
   }

   sg_color_t color() const { return m_color; }

   u32 axis_index() const {
      return m_axis_index;
   }

private:
   sg_color_t m_color;
   u32 m_axis_index;
};

/*! \brief Graph Axis Class
 *
 */
class Axis {
public:
	Axis(){ m_minimum = 0.0; m_maximum = 10.0; m_tick = 1.0; }
	Axis(float min, float max, const var::String & label){
		m_minimum = min; m_maximum = max; m_tick = (max - min) / 10.0;
		m_label = label;
	}

	Axis & set_minimum(float value){ m_minimum = value; return *this; }
	Axis & set_maximum(float value){ m_maximum = value; return *this; }
	Axis & set_tick(float value){ m_tick = value; return *this; }

	const var::String & label() const { return m_label; }

	float range() const { return m_maximum - m_minimum; }
	float minimum() const { return m_minimum; }
	float maximum() const { return m_maximum; }

private:
	float m_minimum, m_maximum;
	float m_tick;
	var::String m_label;
};

/*! \brief Abstract Graph Class
 */
class Graph : public Drawing {
public:
	Graph();

	Axis & x_axis(){ return m_x_axis; }
	const Axis & x_axis() const { return m_x_axis; }

	Axis & y_axis_at(u32 axis_index = 0){
		if( axis_index < m_y_axes.count() ){
			return m_y_axes.at(axis_index);
		}
		return m_y_axes.at(0);
	}

	const Axis & y_axis_at(u32 axis_index = 0) const {
		if( axis_index < m_y_axes.count() ){
			return m_y_axes.at(axis_index);
		}
		return m_y_axes.at(0);
	}

	var::Vector<float> & x_data(){ return m_x_data; }
	const var::Vector<float> & x_data() const { return m_x_data; }

	var::Vector<Axis> & y_axes(){ return m_y_axes; }
	const var::Vector<Axis> & y_axes() const { return m_y_axes; }

	var::Vector<YData> & y_data(){ return m_y_data; }
	const var::Vector<YData> & y_data() const { return m_y_data; }

	YData & y_data_at(u32 data_index = 0){
		if( data_index < m_y_data.count() ){
			return m_y_data.at(data_index);
		}
		return m_y_data.at(0);
	}

	const YData & y_data_at(u32 data_index = 0) const {
		if( data_index < m_y_data.count() ){
			return m_y_data.at(data_index);
		}
		return m_y_data.at(0);
	}

protected:
	sgfx::Point transform_data_to_point(
			const sgfx::Region & region,
			const Axis & y_axis_at,
			float x,
			float y
			);

	/*! \details Draws the axes and returns
	 * the region of the bitmap that is available
	 * within the axes.
	 *
	 */
	sgfx::Region draw_axes(
			sgfx::Bitmap & bitmap,
			const sgfx::Region & region
			);

private:
	Axis m_x_axis;
	var::Vector<Axis> m_y_axes;
	var::Vector<float> m_x_data;
	var::Vector<YData> m_y_data;
};

}

#endif /* SAPI_DRAW_GRAPH_HPP_ */
