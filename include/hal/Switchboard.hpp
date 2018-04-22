#ifndef SWITCHBOARD_HPP
#define SWITCHBOARD_HPP

#include <sos/dev/switchboard.h>
#include "Device.hpp"

namespace hal {

class SwitchboardTerminal {
public:

    /*!
     * \details Constructs a new Switchboard terminal.
     *
     * \param name The name of the terminal (e.g. uart0 -- not /dev/uart0)
     * \param loc The channel/location to read or write
     * \param priority Zero for default priority
     */
    SwitchboardTerminal(const char * name, int loc = 0, s8 priority = 0){
        set_name(name);
        set_loc(loc);
        set_priority(priority);
        m_terminal.bytes_transferred = 0;
    }

    /*! \details Constructs a terminal from a switchboard_terminal_t. */
    SwitchboardTerminal(const switchboard_terminal_t & terminal){
        m_terminal = terminal;
    }

    /*!
     * \details Sets the name of the terminal.
     * \param name A pointer to the terminal name
     */
    void set_name(const char * name){
        m_terminal.name[LINK_NAME_MAX-1]=0; //guarantee zero termination
        strncpy(m_terminal.name, name, LINK_NAME_MAX-1);
    }

    /*!
     * \details Sets the location/channel of the terminal.
     */
    void set_loc(int loc){ m_terminal.loc = loc; }

    /*! \details Sets the priority of the terminal. */
    void set_priority(s8 priority){ m_terminal.priority = priority; }

    const char * name() const { return m_terminal.name; }
    s8 priority() const { return m_terminal.priority; }
    int loc() const { return m_terminal.loc; }

    operator const switchboard_terminal_t & () const { return m_terminal; }

private:
    switchboard_terminal_t m_terminal;
};

class SwitchboardConnection {
public:

    SwitchboardConnection(){}
    SwitchboardConnection(const SwitchboardTerminal & input, const SwitchboardTerminal & output, s32 nbyte){
        set_input(input);
        set_output(output);
    }

    void set_input(const SwitchboardTerminal & value){
        m_connection.input = value;
    }

    void set_output(const SwitchboardTerminal & value){
        m_connection.output = value;
    }

    u16 idx() const { return m_connection.idx; }
    s32 nbyte() const { return m_connection.nbyte; }
    u32 o_flags() const { return m_connection.o_flags; }
    const switchboard_terminal_t & input() const { return m_connection.input; }
    const switchboard_terminal_t & output() const { return m_connection.output; }

private:
    friend class Switchboard;
    switchboard_connection_t m_connection;
};

class SwitchboardInfo {
public:
    SwitchboardInfo(){ memset(&m_info, 0, sizeof(m_info)); }

    u16 connection_count() const { return m_info.connection_count; }
    u16 connection_buffer_size() const { return m_info.connection_buffer_size; }
    u16 transaction_limit() const { return m_info.transaction_limit; }

    operator const switchboard_info_t & () const { return m_info; }
private:
    friend class Switchboard;
    switchboard_info_t m_info;
};

/*! \brief Switch board class
 * \detials The Switchboard class is used to interface
 * with /dev/switchboard and similar devices.
 *
 *
 * \code
 *
 * \endcode
 *
 *
 *
 */
class Switchboard : public Device {
public:
    Switchboard();


    enum {
        CONNECT = SWITCHBOARD_FLAG_CONNECT,
        DISCONNECT = SWITCHBOARD_FLAG_DISCONNECT,
        IS_PERSISTENT = SWITCHBOARD_FLAG_IS_PERSISTENT,
        IS_FIXED_SIZE = 0,
        IS_CONNECTED = SWITCHBOARD_FLAG_IS_CONNECTED,
        IS_STOPPED_ON_ERROR = SWITCHBOARD_FLAG_IS_STOPPED_ON_ERROR
    };


    /*!
     * \details Opens a switch board
     * \param name The path to the switchboard (default is /dev/swithboard)
     * \return Zero on success
     */
    int open(const char * name = "/dev/switchboard");

    SwitchboardConnection get_connection(u16 idx) const;

    /*! \details Gets a free connection that can be used
     * for a new connection.
     *
     * \return Less than zero if no connection is available
     *
     */
    int get_available_connection() const;

    /*!
     * \details Creates a new perisisten connection on the switchboard.
     *
     * \param input The input terminal
     * \param output The output terminal
     * \return The idx number of the new connection or less than zero for an error
     *
     * The connection will persist until it is disconnected using destroy_connection()
     * or it encounters a read or write error.
     *
     */
    int create_persistent_connection(
            const SwitchboardTerminal & input,
            const SwitchboardTerminal & output) const;

    /*!
     * \details create_fixed_size_connection
     * \param input The input terminal
     * \param output The output terminal
     * \param nbyte The number of bytes to transfer
     * \return Zero on success or less than zero for an error
     */
    int create_fixed_size_connection(
            const SwitchboardTerminal & input,
            const SwitchboardTerminal & output,
            s32 nbyte) const;

    /*!
     * \details Disconnects the specified connection.
     *
     * \param idx The connection to disconnect
     * \return Zero on success
     *
     */
    int destroy_connection(u16 idx) const;

    /*! \details Gets information from the switchboard. */
    int get_info(switchboard_info_t & info) const;

    /*! \details Gets information from the switchboard. */
    SwitchboardInfo get_info();

    /*! \details Sets the attributes of the switchboard.
     *
     * Use connect() and disconnect() to manage basic connections.
     * This method can do more advanced features.
     *
     */
    int set_attr(switchboard_attr_t & attr) const;

};
}

#endif // SWITCHBOARD_HPP
