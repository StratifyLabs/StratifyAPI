/* Copyright 2016 tgil All Rights Reserved */

#ifndef UI_BUTTON_HPP_
#define UI_BUTTON_HPP_


#include <mcu/types.h>

#include "../ev/Event.hpp"
#include "../api/UiObject.hpp"

namespace ui {

/*! \brief Button Class
 * \details This class implements a button
 * that can be pressed by the user.  It implements the timing
 * for button actuations, button holds, presses and releases.  It
 * is an abstract class where the inheriting classes must implement
 * the ui::Button::get_value() method which returns the value of input.
 */
class Button : public api::UiWorkObject {
public:


    /*! \details This method returns the max held duration which will effectively disable the
     * reporting of button holds
     *
     *
     *  \code
     *  Button::set_held_duration( Button::max_held_duration() );
     *  \endcode
     */
    static u32 max_held_duration(){ return (u32)-1; }

	/*! \details This method checks the state of the actuation and then returns
	 * an Event if needed.
	 *
	 * It will report the following events
	 *
	 *  - ui::Event::BUTTON_PRESSED
	 *  - ui::Event::BUTTON_RELEASED
	 *  - ui::Event::BUTTON_ACTUATED
     *  - ui::Event::BUTTON_HELD
	 *
	 */
    ev::Event get_event();
    ev::Event event(){ return get_event(); }

	/*! \details Returns the duration of the last button press.
	 * This method will only return a non-zero value once per button press.
	 */
    virtual u32 get_duration() = 0;

	/*! \details Returns true if the button has been pressed. This will
	 * only return true once per button press.
	 *
	 */
    virtual bool get_pressed() = 0;

	/*! \details Returns true if the button has been released.
	 * This will only return true one time for each button press.
	 */
    virtual bool get_released() = 0;

	/*! \details Returns true if the button was pressed then released
	 * This will return true only once per button press.
	 */
    virtual bool get_actuated() = 0;

	/*! \details Returns true if the button has been held for the specified duration
	 * This will only return true once per button press.
	 *
     */
    virtual bool get_held() = 0;

	/*! \details Returns true if the button is currently in the active state.
	 * The active state is updated each time Button::update() is called.
	 *
	 */
    virtual bool is_active() const = 0;

    /*! \details Access the event ID of the button */
    virtual enum ev::Event::button_id event_id() const = 0;


    /*! \details Reset the state of the button */
    virtual void reset();


protected:
    /*! \details This will update the state of the button. This method
     * should be called periodically.
     */
    virtual void update();
};

}

#endif /* UI_BUTTON_HPP_ */
