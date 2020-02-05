/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "ux/Component.hpp"
#include "sys/Printer.hpp"
#include "ux/Scene.hpp"

using namespace sgfx;
using namespace ux;

SceneCollection * Component::scene_collection(){
	return scene()->scene_collection();
}

const SceneCollection * Component::scene_collection() const {
	return scene()->scene_collection();
}

Component::~Component(){
	disable();
}

void Component::enable(
		hal::Display & display
		){

	if( m_is_enabled == false ){
		m_reference_drawing_attributes.set_bitmap(display);
		m_display = &display;

		//local bitmap is a small section of the reference bitmap
		if( m_local_bitmap.allocate(
					m_reference_drawing_attributes.calculate_area_on_bitmap(),
					sgfx::Bitmap::BitsPerPixel(
						m_reference_drawing_attributes.bitmap().bits_per_pixel()
						)
					) < 0 ){
			return;
		}

		//local attributes fill local bitmap
		m_local_drawing_attributes
				.set_area(DrawingArea(1000,1000))
				.set_bitmap(m_local_bitmap);

		set_refresh_region(sgfx::Region());
		m_is_enabled = true;

		redraw();
	}
}


void Component::disable(){
	if( m_is_enabled ){
		m_is_enabled = false;
		m_local_bitmap.free();
	}
}

DrawingPoint Component::translate_point(const sgfx::Point & point){
	if( contains(point) == false ){
		return DrawingPoint(0,0);
	}

	sgfx::Point relative_point = point -
			m_reference_drawing_attributes.calculate_point_on_bitmap();

	sgfx::Area area = m_reference_drawing_attributes.calculate_area_on_bitmap();

	//now scale for width
	return DrawingPoint(
				1000 * relative_point.x() / area.width(),
				1000 * relative_point.y() / area.height()
				);
}


void Component::refresh_drawing(){
	if( m_display ){
		//use the palette if it is available

		if( scene()->scene_collection()->theme().set_display_palette(
					*m_display,
					m_theme_style,
					m_theme_state
					) < 0 ){
			printf("--failed to set display palette\n");
		}

		Region window_region =
				Region(
					Point(m_reference_drawing_attributes.calculate_point_on_bitmap())
					+ m_refresh_region.point(),
					m_refresh_region.area()
					);

		m_display->set_window(window_region);

		m_display->write(
					m_local_bitmap.create_reference(m_refresh_region)
					);

		m_is_refresh_drawing_pending = false;
	}
}

const sgfx::Theme & Component::theme() const {
	return scene()->scene_collection()->theme();
}

void Component::erase(){
	Region window_region =
			Region(
				Point(m_reference_drawing_attributes.calculate_point_on_bitmap())
				+ m_refresh_region.point(),
				m_refresh_region.area()
				);

	printf("erase %p\n", m_display);
	m_display->set_window(window_region);
	m_display->clear();
}

void Component::apply_antialias_filter(const DrawingAttributes & attributes){
	if( is_antialias() ){
#if 0
		attributes.bitmap().apply_antialias_filter(
					theme().antialias_filter(),
					attributes.bitmap().region()
					);
#endif
	}
	set_refresh_drawing_pending();
}

void Component::apply_antialias_filter(const DrawingScaledAttributes & attributes){
	if( is_antialias() ){
#if 0
		attributes.bitmap().apply_antialias_filter(
					theme().antialias_filter(),
					attributes.bitmap().region()
					);
#endif
	}
}
