#ifndef SAPI_UX_DRAW_GRAPH_HPP
#define SAPI_UX_DRAW_GRAPH_HPP

#include "../var/Vector.hpp"
#include "Component.hpp"

namespace ux {

class GraphAxis {
public:
	bool is_valid() const {
		return maximum() - minimum() > 0.0f;
	}

	GraphAxis& set_minimum(float value){
		m_minimum = value;
		return *this;
	}

	GraphAxis& set_maximum(float value){
		m_maximum = value;
		return *this;
	}

	GraphAxis& set_major_tick(float value){
		m_major_tick = value;
		return *this;
	}

	GraphAxis& set_minor_tick(float value){
		m_minor_tick = value;
		return *this;
	}

	float minimum() const { return m_minimum; }
	float maximum() const { return m_maximum; }
	float major_tick() const { return m_major_tick; }
	float minor_tick() const { return m_minor_tick; }

	float is_visible(float input){
		return (input >= minimum()) && (input <= maximum());
	}

	float range() const {
		return maximum() - minimum();
	}


private:
	float m_minimum = 0.0f;
	float m_maximum = 0.0f;
	float m_major_tick = 0.0f;
	float m_minor_tick = 0.0f;
};

class GraphAxes  {
public:
	GraphAxes();

	GraphAxis& x_axis(){ return m_x_axis; }
	const GraphAxis& x_axis() const { return m_x_axis; }

	GraphAxis& y_axis(){ return m_y_axis; }
	const GraphAxis& y_axis() const { return m_y_axis; }

	GraphAxis& y2_axis(){ return m_y2_axis; }
	const GraphAxis& y2_axis() const { return m_y2_axis; }

	const DrawingArea& axes_area() const { return m_axes_area; }
	DrawingArea& axes_area(){ return m_axes_area; }


private:
	GraphAxis m_x_axis;
	GraphAxis m_y_axis;
	GraphAxis m_y2_axis;
	DrawingArea m_axes_area;

};

class LineGraphPoint {
public:
	LineGraphPoint(float x, float y) :
		m_x(x), m_y(y){
	}

	float x() const { return m_x; }
	float y() const { return m_y; }

private:
	float m_x;
	float m_y;
};

class LineGraph :
		public GraphAxes,
		public DrawingComponentProperties<LineGraph>,
		public ComponentAccess<
		LineGraph,
		COMPONENT_SIGNATURE('l','g','p','h')> {
	public:

	virtual void draw_to_scale(const DrawingScaledAttributes & attr);

	var::Vector<var::Vector<LineGraphPoint>>& data_set(){ return m_data_set; }
	const var::Vector<var::Vector<LineGraphPoint>>& data_set() const { return m_data_set; }

	private:

	sgfx::Point transform_point(
				const DrawingScaledAttributes & drawing_attributes,
				const LineGraphPoint & line_graph_point
				);

	var::Vector<var::Vector<LineGraphPoint>> m_data_set;


};

class BarGraphValue {
public:

	BarGraphValue(const var::String & label,
								float value) :
		m_label(label), m_value(value){

	}

	const var::String & label() const {
		return m_label;
	}

	float value() const	{
		return m_value;
	}


private:
	var::String m_label;
	float m_value;
};

class BarGraph :
		public GraphAxes,
		public DrawingComponentProperties<BarGraph>,
		public ComponentAccess<
		BarGraph,
		COMPONENT_SIGNATURE('l','g','p','h')>{
public:

	virtual void draw_to_scale(const DrawingScaledAttributes & attributes);


	var::Vector<var::Vector<BarGraphValue>>& data_set(){ return m_data_set; }
	const var::Vector<var::Vector<BarGraphValue>>& data_set() const { return m_data_set; }

private:
	var::Vector<var::Vector<BarGraphValue>> m_data_set;

};

}

#endif // SAPI_UX_DRAW_GRAPH_HPP
