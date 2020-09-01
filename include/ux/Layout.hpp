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


class Layout : public ComponentAccess<Layout>{
public:
	
	using IsRecursive = arg::Argument<bool, struct LayoutIsRecursiveTag>;
	using EventHandlerFunction = std::function<void(Layout * layout, const Event & event)>;
	
	enum flow {
		flow_vertical,
		flow_horizontal,
		flow_free
	};
	
	COMPONENT_PREFIX(Layout)

	Layout(
			const var::String & name,
			EventLoop * event_loop
			);

	virtual ~Layout();
	
	Layout& add_component(
			Component& component
			);

	Layout& replace_component(
					const var::String & component_name,
					Component& component);
	
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

	bool is_owner(const Component * component){
		if( component == nullptr ){
			return false;
		}

		for(LayoutComponent& cp: m_component_list){
			if( cp.component() == component ){
				return true;
			}
			if( cp.component()->is_layout() ){
				if( static_cast<Layout*>(cp.component())->is_owner(component) ){
					return true;
				}
			}
		}
		return false;
	}
	
	Layout * find_layout(const var::String & name){
		for(LayoutComponent& cp: m_component_list){
			if( (cp.component()->name() == name) && cp.component()->is_layout() ){
				return static_cast<Layout*>(cp.component());
			}
		}
		return nullptr;
	}
	
	template<class T, bool is_fatal = true> T * search(
			const var::String & name
			){
		for(LayoutComponent& cp: m_component_list){
			if( cp.component()->is_layout() ){
				T * result = static_cast<Layout*>(cp.component())->search<T, false>(name);
				if( result ){ return result; }
			}
			
			if(
				 (cp.component()->name() == name)
				 || (cp.component()->name() == T::get_name(name))
				 ){
				return static_cast<T*>(cp.component());
			}
		}
		if( is_fatal ){
			printf("Failed to search %s\n", name.cstring());
			abort();
		}
		return nullptr;
	}

	template<class T, bool is_fatal = true> T * find(
			const var::String & name
			){
		for(LayoutComponent& cp: m_component_list){
			if(
				 (cp.component()->name() == name)
				 || (cp.component()->name() == T::get_name(name))
				 ){
				return static_cast<T*>(cp.component());
			}
		}
		if( is_fatal ){
			printf("Failed to find %s\n", name.cstring());
			abort();
		}
		return nullptr;
	}
	
	bool transition(
			const var::String & next_layout_name
			);

	bool transition(
			Layout * next_layout
			);
	
	void scroll(DrawingPoint value);
	
	virtual void draw(const DrawingAttributes & attributes);
	virtual void handle_event(const ux::Event & event);
	
protected:
	Layout(
			const var::String& prefix, const var::String & name,
			EventLoop * event_loop
			);

private:
	friend class EventLoop;
	enum flow m_flow;
	DrawingPoint m_origin;
	DrawingArea m_area;
	sgfx::Point m_touch_last;
	ux::TouchGesture m_touch_gesture;
	API_ACCESS_COMPOUND(Layout,var::Vector<LayoutComponent>,component_list);
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
			) : Layout("", name, event_loop){}
	
	
	T& add_component(
			Component& component
			){
		return static_cast<T&>(Layout::add_component(component));
	}

	T& replace_component(
			const var::String & component_name,
			Component& component){
		return static_cast<T&>(Layout::replace_component(component_name, component));
	}

	API_ACCESS_DERIVED_FUNDAMETAL(Layout,T,enum flow,flow)
	API_ACCESS_DERIVED_BOOL(Layout,T,vertical_scroll_enabled)
	API_ACCESS_DERIVED_BOOL(Layout,T,horizontal_scroll_enabled)
	API_ACCESS_DERIVED_COMPOUND(Layout,T,DrawingArea,drawing_area)
	API_ACCESS_DERIVED_COMPOUND(Layout,T,DrawingPoint,drawing_point)
	API_ACCESS_DERIVED_FUNDAMETAL(Layout,T,enum sgfx::Theme::styles,theme_style)
	API_ACCESS_DERIVED_FUNDAMETAL(Layout,T,enum sgfx::Theme::states,theme_state)
	
	T& set_enabled(bool value = true){
		Component::set_enabled_internal(value);
		return static_cast<T&>(*this);
	}
	
	COMPONENT_ACCESS_CREATE()
	
	protected:

};

}

#endif // SAPI_UX_LAYOUT_HPP
