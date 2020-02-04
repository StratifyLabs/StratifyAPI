#ifndef SAPI_UX_TOUCHGESTURE_HPP
#define SAPI_UX_TOUCHGESTURE_HPP


#include "../sgfx/Region.hpp"
#include "../sgfx/Point.hpp"
#include "../chrono/Timer.hpp"
#include "Event.hpp"

namespace ux {

class TouchEvent : public EventObject<EVENT_TYPE('_','t','c','h')> {
public:
	TouchEvent(
				u32 id,
				const sgfx::Point & point
				) :
		EventObject(id){
		m_point = point;
	}

	enum touch_id {
		id_none,
		id_active,
		id_pressed,
		id_released,
	};

	const sgfx::Point & point() const {
		return m_point;
	}

private:
	sgfx::Point m_point;

};


/*! \brief TouchGesture Class
 * \details The TouchGesture class
 * processes touch events to determine
 * if the touch was a
 *
 * - Touch (press and release)
 * - Hold (press and hold for a while)
 * - Drag (press and drag either vertically or horizontally)
 *
 */
class TouchGesture
{
public:

	enum id {
		id_none,
		id_touched,
		id_dragged,
		id_complete
	};

	TouchGesture& set_vertical_drag_enabled(bool value = true){
		m_is_vertical_drag_enabled = value;
		return *this;
	}

	TouchGesture& set_horizontal_drag_enabled(bool value = true){
		m_is_horizontal_drag_enabled = value;
		return *this;
	}

	TouchGesture& set_multidimensional_drag_enabled(bool value = true){
		m_is_multidimensional_drag_enabled = value;
		return *this;
	}

	TouchGesture& set_region(const sgfx::Region & region){
		m_region = region;
		return *this;
	}

	enum id process_event(const Event & event);

	const chrono::Timer & timer() const {
		return m_timer;
	}

	const sgfx::Point & drag() const {
		return m_drag;
	}

	bool is_vertical_drag_enabled() const {
		return m_is_vertical_drag_enabled;
	}

	bool is_horizontal_drag_enabled() const {
		return m_is_horizontal_drag_enabled;
	}

	bool is_multidimensional_drag_enabled() const {
		return m_is_multidimensional_drag_enabled;
	}

private:
	sgfx::Point m_drag;
	sgfx::Point m_last_point;
	sgfx::Region m_region;
	chrono::Timer m_timer;
	s16 m_vertical_drag = 0;
	s16 m_horizontal_drag = 0;
	bool m_is_vertical_drag_enabled = false;
	bool m_is_horizontal_drag_enabled = false;
	bool m_is_multidimensional_drag_enabled = false;
	bool m_is_vertical_drag_active = false;
	bool m_is_horizontal_drag_active = false;
	bool m_is_pressed_contained = false;
	static const sg_int_t m_drag_theshold = 1;

	sgfx::Point process_drag(const TouchEvent & event);

};

}

#endif // SAPI_UX_TOUCHGESTURE_HPP
