/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef SAPI_UX_LAYOUT_HPP
#define SAPI_UX_LAYOUT_HPP

#include "Component.hpp"
#include "TouchGesture.hpp"

namespace ux {

class LayoutComponent {
public:
	LayoutComponent(
			Component * component
			){
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

#define LAYOUT_COMPONENT_SIGNATURE COMPONENT_SIGNATURE('l','y','o','t')

class Layout : public ComponentAccess<
		Layout, LAYOUT_COMPONENT_SIGNATURE
		> {
public:

	using IsRecursive = arg::Argument<bool, struct LayoutIsRecursiveTag>;
	using EventHandlerFunction = std::function<void(Layout * layout, const Event & event)>;

	static u32 whatis_signature(){
		return LAYOUT_COMPONENT_SIGNATURE;
	}

	enum flow {
		flow_vertical,
		flow_horizontal,
		flow_free
	};

	Layout(
			const var::String & name,
			EventLoop * event_loop
			);
	virtual ~Layout();

	Layout& add_component(
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

	Layout& set_event_handler(EventHandlerFunction event_handler){
		m_event_handler = event_handler;
		return *this;
	}

	void update_drawing_area(
			const Component * component,
			const DrawingArea & area
			);

	void update_drawing_area(
			const var::String & name,
			const DrawingArea & area
			){
		update_drawing_area(
					find<Component>(name), area
					);
	}

	void update_drawing_point(
			const Component * component,
			const DrawingPoint & point
			);

	void update_drawing_point(
			const var::String & name,
			const DrawingPoint & point
			){
		update_drawing_point(
					find<Component>(name), point
					);
	}

	Layout * find_layout(const var::String & name){
		for(auto cp: m_component_list){
			if( cp.component()->name() == name ){
				if( cp.component()->signature() == whatis_signature()){
					return static_cast<Layout*>(cp.component());
				}
			}
		}
		return nullptr;
	}

	template<class T> T * find(
			const var::String & name,
			IsRecursive is_recursive = IsRecursive(true)){
		for(auto cp: m_component_list){
			if( cp.component()->signature() == whatis_signature() ){
				T * result = static_cast<Layout*>(cp.component())->find<T>(
							name
							);

				if( result ){ return result;}
			}

			if( cp.component()->name() == name ){
				return static_cast<T*>(cp.component());
			}
		}
		return nullptr;
	}

	bool transition(
			const var::String & next_layout_name
			);

	void scroll(DrawingPoint value);

	virtual void draw(const DrawingAttributes & attributes);
	virtual void handle_event(const ux::Event & event);

private:
	friend class EventLoop;
	enum flow m_flow;
	DrawingPoint m_origin;
	DrawingArea m_area;
	sgfx::Point m_touch_last;
	ux::TouchGesture m_touch_gesture;
	var::Vector<LayoutComponent> m_component_list;
	EventHandlerFunction m_event_handler;

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

	void examine_visibility();

	void set_refresh_region(const sgfx::Region & region);
	void touch_drawing_attributes(){
		shift_origin(DrawingPoint(0,0));
	}
};

template<class T> class LayoutAccess : public Layout {
public:

	LayoutAccess<T>(
			const var::String & name,
			EventLoop * event_loop
			) : Layout(name, event_loop){}


	T& add_component(
			Component& component
			){
		return static_cast<T&>(Layout::add_component(component));
	}

	T& set_flow(enum flow value){
		return static_cast<T&>(Layout::set_flow(value));
	}

	T& set_vertical_scroll_enabled(bool value = true){
		return static_cast<T&>(Layout::set_vertical_scroll_enabled(value));
	}

	T& set_horizontal_scroll_enabled(bool value = true){
		return static_cast<T&>(Layout::set_horizontal_scroll_enabled(value));
	}

	T& set_enabled(bool value = true){
		Component::set_enabled_internal(value);
		return static_cast<T&>(*this);
	}

	T& set_drawing_area(const DrawingArea & value){
		Component::set_drawing_area(value);
		return static_cast<T&>(*this);
	}

	T& set_drawing_point(const DrawingPoint & value){
		Component::set_drawing_point(value);
		return static_cast<T&>(*this);
	}

	T& set_theme_style(enum sgfx::Theme::style value){
		Component::set_theme_style(value);
		return static_cast<T&>(*this);
	}

	T& set_theme_state(enum sgfx::Theme::state value){
		Component::set_theme_state(value);
		return static_cast<T&>(*this);
	}

	template<typename... Args> static T & create(
			Args... args
			){
		T * result = new T(args...);
		if( result == nullptr ){
			//assert here
			printf("failed!!!\n");
		}
		result->m_is_created = true;
		return *result;
	}

protected:

};

}

#endif // SAPI_UX_LAYOUT_HPP
