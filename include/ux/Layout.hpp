/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef SAPI_UX_LAYOUT_HPP
#define SAPI_UX_LAYOUT_HPP

#include "Component.hpp"
#include "TouchGesture.hpp"

namespace ux {

class LayoutComponent {
public:
	LayoutComponent(Component * component){
		m_component = component;
		m_drawing_point = component->reference_drawing_attributes().point();
		m_drawing_area = component->reference_drawing_attributes().area();
	}

	Component * component() const {
		return m_component;
	}

	const DrawingPoint & drawing_point() const {
		return m_drawing_point;
	}

	const DrawingArea & drawing_area() const {
		return m_drawing_area;
	}


	void set_drawing_point(const DrawingPoint & point){
		m_drawing_point = point;
	}

	void set_drawing_area(const DrawingArea & area){
		m_drawing_area = area;
	}

private:
	Component * m_component;
	DrawingPoint m_drawing_point;
	DrawingArea m_drawing_area;
};

class Layout : public Component, public DrawingAlignment<Layout> {
public:

	enum flow {
		flow_vertical,
		flow_horizontal,
		flow_free
	};

	Layout();
	virtual ~Layout();

	Layout& add_component(
			const var::String & name,
			Component& component
			);

	Layout& set_flow(enum flow flow){
		m_flow = flow;
		return *this;
	}

	Layout& set_vertical_scroll_enabled(bool value = true){
		m_touch_gesture.set_vertical_drag_enabled(value);
		return *this;
	}

	Layout& set_horizontal_scroll_enabled(bool value = true){
		m_touch_gesture.set_horizontal_drag_enabled(value);
		return *this;
	}

	u16 columns() const {
		return m_columns;
	}

	void scroll(DrawingPoint value);

	virtual void draw(const DrawingAttributes & attributes);
	virtual void handle_event(const ux::Event & event);

	void set_visible(bool value = true);

private:
	enum flow m_flow;
	u16 m_columns;
	DrawingPoint m_origin;
	DrawingArea m_area;
	sgfx::Point m_touch_last;
	TouchGesture m_touch_gesture;

	var::Vector<LayoutComponent> m_component_list;
	bool m_is_initialized = false;

	void shift_origin(DrawingPoint shift);
	drawing_int_t handle_vertical_scroll(sg_int_t scroll);
	drawing_int_t handle_horizontal_scroll(sg_int_t scroll);

	DrawingPoint calculate_next_point(
			const DrawingPoint& point,
			const DrawingArea& area
			);

	void generate_layout_positions();
	void generate_vertical_layout_positions();
	void generate_horizontal_layout_positions();
	void generate_free_layout_positions();
};

}

#endif // SAPI_UX_LAYOUT_HPP
