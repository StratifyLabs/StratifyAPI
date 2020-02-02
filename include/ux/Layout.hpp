#ifndef SAPI_UX_LAYOUT_HPP
#define SAPI_UX_LAYOUT_HPP

#include "Component.hpp"

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

	bool is_visible() const {
		return m_is_visible;
	}

	void set_visible(bool value = true){
		m_is_visible = value;
	}


private:
	Component * m_component;
	DrawingPoint m_drawing_point;
	DrawingArea m_drawing_area;
	bool m_is_visible;
};

class Layout : public Component {
public:

	enum flow {
		flow_vertical,
		flow_horizontal,
		flow_grid
	};

	Layout();
	virtual ~Layout();

	Layout& add_component(
			const var::String & name,
			Component& component
			);

	u16 columns() const {
		return m_columns;
	}

	void scroll(DrawingPoint value){
		shift_origin(value);
	}

	virtual void draw(const DrawingAttributes & attributes);
	virtual void handle_event(const ux::Event & event);

private:
	enum flow m_flow;
	u16 m_columns;
	DrawingPoint m_origin;
	sgfx::Point m_touch_last;
	bool m_is_drag_active;

	var::Vector<LayoutComponent> m_component_list;
	bool m_is_initialized = false;

	void enter();
	void shift_origin(DrawingPoint shift);

	DrawingPoint calculate_next_point();

};

}

#endif // SAPI_UX_LAYOUT_HPP
