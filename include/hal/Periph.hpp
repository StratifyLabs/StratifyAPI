/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved

#ifndef PHY_PERIPH_HPP_
#define PHY_PERIPH_HPP_

#include <sos/link.h>
#include <fcntl.h>
#include <cstdlib>
#include <cstring>
#include <signal.h>


#include "Device.hpp"


namespace hal {

/*! \brief Periph Object Class
 * \details This class handles general peripheral IO. It re-implments
 * open(), close(), read(), write(), ioctl() in order to manage
 * the file descriptors for MCU peripherals.
 *
 */
class PeriphObject : public Device {
public:

    /*! \details Defines the type to use when specifying a peripheral port. */
    typedef unsigned int port_t;

    /*!  \details This method opens the peripheral.  For each instance, the peripheral
     * only needs to be opened one time.  The port is typically opened with open() and
     * configured with set_attr().  After that, other instances of the peripheral can read
     * and write without opening again.
     */
    int open(int o_mode = READWRITE);
    int ioctl(int req, void * arg) const;
    int seek(int loc, int whence) const;
    int fileno() const;
    int read(void * buf, int nbyte) const;
    int write(const void * buf, int nbyte) const;
#ifndef __link
    int read(sys::Aio & aio) const;
    int write(sys::Aio & aio) const;
#endif
    int close();

    using File::ioctl;
    using File::read;
    using File::write;

protected:
    u16 m_periph_port;

    int open(const char * name, int flags);

    void update_fileno() const;
    int lookup_fileno() const;

private:
    static u16 m_fd_map[LINK_OPEN_MAX];

};

template<typename attr_t> class PeriphAttr {
public:

    PeriphAttr(){
        m_port = 0;
        memset(&m_attr, 0, sizeof(m_attr));
    }

    /*! \details Accesses the value of the port. */
    u8 port() const { return m_port; }

    /*! \details Sets the value of the port. */
    void set_port(u8 port){ m_port = port; }

    /*! \details Returns a copy of the attributes. */
    operator attr_t() const { return m_attr; }

    /*! \details Returns a reference to the attributes object. */
    const attr_t & attr() const { return m_attr; }

    attr_t & attr(){ return m_attr; }

protected:
    attr_t m_attr;

private:
    u8 m_port;
};

template<typename attr_t, typename pin_assignment_t>
class PinAssignmentPeriphAttr {
public:

    PinAssignmentPeriphAttr(){
        m_port = 0;
        memset(&m_attr, 0, sizeof(m_attr));
        memset(&m_attr.pin_assignment, 0xff, sizeof(pin_assignment_t));
    }

    /*! \details Accesses the value of the port. */
    u8 port() const { return m_port; }

    /*! \details Sets teh value of the port. */
    void set_port(u8 port){ m_port = port; }

    /*! \details Returns a copy of the attributes. */
    operator attr_t() const { return m_attr; }

    /*! \details Returns a reference to the attributes object. */
    const attr_t & attr() const { return m_attr; }

    /*! \details Accesses the value of the flags. */
    u8 o_flags() const { return m_attr.o_flags; }

    /*! \details Sets the value of the flags. */
    void set_flags(u32 o_flags){ m_attr.o_flags = o_flags; }

    /*! \details Access the frequency. */
    u32 freq() const { return m_attr.freq; }

    /*! \details Set the frequency.
     *
     * @param freq_hz The frequency in Hertz
     */
    void set_freq(u32 freq_hz){ m_attr.freq = freq_hz; }

    /*! \details Gets a pointer of the pin assignment. */
    const pin_assignment_t * pin_assignment() const { return &m_attr.pin_assignment; }

protected:
    attr_t m_attr;

private:
    u8 m_port;

};


/*! \brief Peripheral Class
 * \details This is an abstract class for a microcontroller peripheral.
 *
 * All peripherals have a common interface based on the following function:
 * - open()
 * - close()
 * - read()
 * - write()
 * - ioctl()
 *
 * The classes that inherit Periph implement a method for each ioctl() call available
 * on the peripheral.  For example, the UART has an ioctl request called I_UART_SETATTR so
 * the method Uart::set_attr() implements the ioctl request.
 *
 */
template<typename info_t, typename attr_t, char ident_char> class Periph : public PeriphObject {
public:

    /*!
     * \details Constructs an MCU peripheral object
     * \param periph Core peripheral value
     * \param port Port number
     */
    Periph(core_periph_t periph, port_t port){
        m_periph_port = (periph << 8) | port;
        m_fd = lookup_fileno();
    }

    /*! \details Gets the version of the peripheral driver.
     *
     * @return The BCD version of the driver
     *
     * The version is a 24-bit BCD value. For example, 0x010203 is
     * version 1.2.3 where:
     *
     * - 1 is the major version
     * - 2 is the minor version
     * - 3 is the patch version
     *
     * If the major version of the driver is not the same as the major version of
     * the driver interface header in the SDK, there will be problems using the hardware.
     *
     * Here is a code sample using a Pwm object (which inherits this object)
     *
     * \code
     * #include <sapi.hal.hpp>
     *
     * Pwm pwm(1); //pwm inherits this object
     * pwm.open();
     * if( (pwm.get_version()) >> 16 != (PWM_VERSION >>16) ){
     *  printf("Major versions are not the same\n");
     * } else {
     *  printf("Major versions match\n");
     * }
     * \endcode
     *
     */
    int get_version() const {
        return ioctl(_IOCTL(ident_char, I_MCU_GETVERSION));
    }


    /*! \details Gets the peripheral info.
     *
     * @param info A reference to the info object that will be written.
     * @return Zero on success, less than one for an error
     *
     */
    int get_info(info_t & info) const {
        return ioctl(_IOCTLR(ident_char, I_MCU_GETINFO, info_t), &info);
    }

    /*! \details Sets the default attributes on the hardware.
     *
     * @return Zero on success
     *
     * For this method to work correctly, the board support package
     * must include the configuration information for the device.
     * If the information is not provided, this method will return an error.
     *
     */
    int set_attr() const {
        return ioctl(_IOCTLW(ident_char, I_MCU_SETATTR, attr_t), 0);
    }

    /*! \details Initializes the hardware using the default attributes.
     */
    int init(){
        int ret = open();
        if( ret < 0 ){
            return ret;
        }
        return set_attr();
    }

    int set_attr(const attr_t & attr) const {
        return ioctl(_IOCTLW(ident_char, I_MCU_SETATTR, attr_t), (const attr_t*)&attr);
    }

    int set_action(mcu_action_t & action) const {
        return ioctl(_IOCTLW(ident_char, I_MCU_SETACTION, mcu_action_t), &action);
    }



    /*! \details Sets the action callback for the event.
     *
     * @param channel
     * @param o_events
     * @param prio
     * @param callback
     * @param context
     * @return
     */

    int set_action(u32 channel, u32 o_events, s8 prio = 0, mcu_callback_t callback = 0, void * context = 0) const {
        mcu_action_t action;
        action.prio = prio;
        action.channel = channel;
        action.o_events = o_events;
        action.handler.callback = callback;
        action.handler.context = context;
        return set_action(action);
    }

    /*! \details Adjusts the priority of the hardware interrupt for the MCU peripheral.
     *
     * @return Zero on success
     */
    int set_priority(s8 priority, u32 o_events, int channel = 0){
        return set_action(channel, o_events, priority);
    }

    port_t port() const{ return m_periph_port & 0xFF; }

protected:


    int set_channel(const mcu_channel_t & channel, int request) const {
        return ioctl(_IOCTLR(ident_char, request, mcu_channel_t), &channel);
    }

    int set_channel(u32 loc, u32 value, int request) const {
        mcu_channel_t channel;
        channel.loc = loc;
        channel.value = value;
        return ioctl(request, &channel);
    }

    u32 get_channel(u32 loc, int request) const {
        mcu_channel_t channel;
        channel.loc = loc;
        if( ioctl(request, &channel) < 0 ){
            return (u32)-1;
        }
        return channel.value;
    }

};

}

#endif /* PHY_PERIPH_HPP_ */
