#ifndef UI_DEVICEBUTTON_HPP
#define UI_DEVICEBUTTON_HPP

#include <sos/dev/button.h>
#include "Button.hpp"
#include "../sys/MicroTime.hpp"
#include "../hal/Device.hpp"

namespace ui {


/*! \brief Device Button Class
 * \details The Device Button class implements one or more
 * buttons using the sos/dev/button.h device driver
 * interface.
 *
 * Because the button events are running at the device driver level,
 * the button responses will be better than ui::PinButton.
 *
 * Also, using a device driver for the buttons makes the applications
 * much more portable.
 *
 */
class DeviceButton : public Button, public hal::Device {
public:
    DeviceButton();

    /*! \details Returns the number of buttons available.
     *
     * This will query the device for the number of buttons
     * available. The set() method will select which button
     * is read.
     *
     */
    int count() const;

    /*! \details Sets the button to the index specified.
     *
     * @param idx The button index
     *
     * If the device supports multiple buttons, this method
     * will select which button is effective.
     *
     * One object can emit events for multiple buttons using
     * this method.
     *
     * \code
     *
     * #include <sapi/ui.hpp>
     *
     * DeviceButton button;
     *
     * button.open("/dev/button0");
     *
     * int count = button.count();
     * for(int i = 0; i < count; i++){
     *   button.set(i);
     *   Event event = button.get_event();
     * }
     *
     * \endcode
     *
     */
    void set(int location){ m_location = location; }

    /*! \details Initializes the button driver.
     *
     * @return Zero on success.
     */
    int init();

    /*! \details Sets the attributes for the specified button.
     *
     * @param loc The button index
     * @param id The Event ID to assign to the button
     * @param held_threshold The threshold to wait before button held event is triggered
     * @param actuated_threshold The minimum button press time for an actuation event
     * @returns Zero on success
     *
     */
    int set_attributes(int location, int id,
                       const sys::MicroTime held_threshold = sys::MicroTime::invalid(),
                       const sys::MicroTime actuated_threshold = sys::MicroTime::invalid());

    u32 get_duration();
    bool get_pressed();
    bool get_released();
    bool get_actuated();
    bool get_held();
    void reset();
    bool is_active() const;

    enum ev::Event::button_id event_id() const {
        return (enum ev::Event::button_id)m_status.id;
    }

protected:
    void update();


private:
    bool get_is_active() const;
    button_status_t m_status;
    int m_location;

};

}

#endif // UI_DEVICEBUTTON_HPP
