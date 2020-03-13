/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef SAPI_UX_COMPONENT_HPP
#define SAPI_UX_COMPONENT_HPP

#include "../api/WorkObject.hpp"
#include "../hal/Display.hpp"
#include "../var/String.hpp"
#include "../sgfx/Theme.hpp"
#include "Drawing.hpp"
#include "Event.hpp"

#define COMPONENT_SIGNATURE(a,b,c,d) ((a << 24) | (b << 16) | (c << 8) | (d))

namespace ux {

class EventLoop;

class Component : public Drawing {
public:

	Component(
			const var::String & name,
			u32 signature = 0) :
		m_name(name), m_signature(signature){
		set_drawing_point(DrawingPoint(0,0));
		set_drawing_area(DrawingArea(1000,1000));
	}
	virtual ~Component();

	template<typename T> T * reinterpret(){
		return static_cast<T*>(this);
	}


	static u32 whatis_signature(){
		return 0;
	}

	enum sgfx::Theme::style theme_style() const {
		return m_theme_style;
	}

	enum sgfx::Theme::state theme_state() const {
		return m_theme_state;
	}

	bool is_antialias() const {
		return m_is_antialias;
	}

	Component& set_antialias(bool value = true){
		m_is_antialias = value;
		return *this;
	}

	bool is_visible() const {
		return m_is_visible;
	}

	bool is_enabled() const {
		return m_is_enabled;
	}


	const sgfx::Theme * theme() const;
	const hal::Display * display() const;
	hal::Display * display();



	//update the location of the component (allow animations)

	virtual void handle_event(const ux::Event & event){}

	const var::String & name() const {
		return m_name;
	}

	void apply_antialias_filter(const DrawingAttributes & attributes);
	void apply_antialias_filter(const DrawingScaledAttributes & attributes);

	void redraw(){
		if( is_ready_to_draw() ){
			draw(local_drawing_attributes());
			set_refresh_drawing_pending();
		}
	}

	sgfx::Region region() const {
		return m_reference_drawing_attributes.calculate_region_on_bitmap();
	}

	bool contains(const sgfx::Point & point){
		return sgfx::Region(
					m_reference_drawing_attributes.calculate_region_on_bitmap()
					).contains(point);
	}

	DrawingPoint translate_point(const sgfx::Point & point);

	void erase();

	void set_refresh_drawing_pending(){
		m_is_refresh_drawing_pending = true;
	}

	EventLoop * event_loop(){
		return m_event_loop;
	}

	const EventLoop * event_loop() const {
		return m_event_loop;
	}

	void set_drawing_area(const DrawingArea & drawing_area){
		m_reference_drawing_attributes.set_area(drawing_area);
	}

	void set_drawing_point(const DrawingPoint & drawing_point){
		m_reference_drawing_attributes.set_point(drawing_point);
	}

	void set_theme_style(enum sgfx::Theme::style value){
		m_theme_style = value;
	}

	void set_theme_state(enum sgfx::Theme::state value){
		m_theme_state = value;
	}

protected:

	bool m_is_visible = false;
	bool m_is_enabled = true;
	bool m_is_created = false;
	virtual void examine_visibility();


	virtual void touch_drawing_attributes(){}

	virtual void set_refresh_region(const sgfx::Region & region){
		m_refresh_region = region;
		if( region.width() * region.height() == 0 ){
			set_visible_internal(false);
		}
	}

	bool is_refresh_drawing_pending() const {
		return m_is_refresh_drawing_pending;
	}

	void refresh_drawing();
	friend class Layout;
	friend class LayoutComponent;

	const DrawingAttributes & local_drawing_attributes() const {
		return m_local_drawing_attributes;
	}

	const DrawingAttributes& reference_drawing_attributes() const {
		return m_reference_drawing_attributes;
	}

	DrawingAttributes& reference_drawing_attributes(){
		return m_reference_drawing_attributes;
	}

	virtual void set_enabled_internal(bool value = true){
		if( m_is_enabled != value ){
			m_is_enabled = value;
			examine_visibility();
		}
	}

	virtual void set_visible_internal(bool value = true){
		if( m_is_visible != value ){
			m_is_visible = value;
			examine_visibility();
		}
	}

	void set_event_loop(EventLoop * event_loop){
		m_event_loop = event_loop;
	}

	bool is_ready_to_draw() const {
		return is_enabled() && is_visible();
	}

	u32 signature() const {
		return m_signature;
	}


private:

	var::String m_name;
	const u32 m_signature;
	//needs to know where on the display it is drawn
	DrawingAttributes m_reference_drawing_attributes;
	DrawingAttributes m_local_drawing_attributes;
	sgfx::Bitmap m_local_bitmap;
	enum sgfx::Theme::style m_theme_style = sgfx::Theme::style_brand_primary;
	enum sgfx::Theme::state m_theme_state = sgfx::Theme::state_default;
	bool m_is_antialias = true;
	bool m_is_refresh_drawing_pending;
	sgfx::Region m_refresh_region;

	//needs a palette to use while drawing
	EventLoop * m_event_loop = nullptr;

	void set_name(const var::String & name){
		m_name = name;
	}


};

template<class T, u32 signature_value> class ComponentAccess : public Component {
public:

	ComponentAccess(const var::String & name) :
		Component(name, signature_value){}

	T& set_enabled(bool value = true){
		Component::set_enabled_internal(value);
		return static_cast<T&>(*this);
	}

	T& set_drawing_area(const DrawingArea & value){
		Component::set_drawing_area(value);
		return static_cast<T&>(*this);
	}

	T& set_drawing_area(drawing_size_t width, drawing_size_t height){
		Component::set_drawing_area(DrawingArea(width,height));
		return static_cast<T&>(*this);
	}

	T& set_drawing_point(const DrawingPoint & value){
		Component::set_drawing_point(value);
		return static_cast<T&>(*this);
	}

	T& set_drawing_point(drawing_int_t x, drawing_int_t y){
		Component::set_drawing_point(DrawingPoint(x,y));
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

#endif // SAPI_UX_COMPONENT_HPP
