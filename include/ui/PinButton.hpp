/* Copyright 2016 tgil All Rights Reserved */

#ifndef UI_PINBUTTON_HPP_
#define UI_PINBUTTON_HPP_

#include "../hal/Pin.hpp"
#include "../chrono/Timer.hpp"
#include "Button.hpp"

namespace ui {

/*! \brief Pin Button Class
 * \details This class implements a button using a hal::Pin object.
 *
 * This class samples the pin during the ui::EventLoop. If the ui::EventLoop
 * is sluggish at times, the button presses might not be responsive (or may be missed).
 *
 * If this is the case, it is better to use DeviceButton because
 * DeviceButton samples in the the background and will queue button
 * presses if the ui::EventLoop is being slow.
 *
 */
class PinButton : public Button, public hal::Pin {
public:

	/*! \details Construct a new ButtonPin
	 *
	 * The pin must be initialized using Pin::init()
	 *
	 * @param port The port associated with the pin
	 * @param pin The pin number on the port for the button input
	 * @param active_value true for active high and false for active low
	 */
    PinButton(int port, int pin, bool active_value = true);

    /*! \details This method accesses how long the user must hold the
     * button in the application before the ui::Event::BUTTON_HELD is triggered
     * when calling event()
     *
     * @return The duration in milliseconds
     */
    static u32 held_duration(){ return m_held_duration; }

    /*! \details This method sets the duration of the hold event.
     */
    static void set_held_duration(u32 duration_ms){ m_held_duration = duration_ms; }


	/*! \details This method accesses the active value.
	 *
	 * @return True for active high and false for active low.
	 */
	bool active_value() const { return m_active_value; }

    /*! \details Set the ID of the button for Events */
    void set_id(enum ev::Event::button_id v){ m_event_id = v; }
    void set_event_id(enum ev::Event::button_id v){ set_id(v); }

    //methods documented at ui::Button
    enum ev::Event::button_id event_id() const { return m_event_id; }
    u32 get_duration();
    bool get_pressed();
    bool get_released();
    bool get_actuated();
    bool get_held();
    bool is_active() const { return get_is_active(); }
    void reset();


protected:
    void update();
    virtual bool get_is_active() const;

private:

    static u32 m_held_duration;
    enum ev::Event::button_id m_event_id;

	bool m_active_value;

    chrono::Timer m_timer;

    struct button_flags {
        unsigned press_reported:1;
        unsigned release_reported:1;
        unsigned held_reported:1;
        unsigned actuation_reported:1;
        unsigned duration_reported:1;
    };

    struct button_flags m_flags;

};

}

#endif /* UI_PINBUTTON_HPP_ */
