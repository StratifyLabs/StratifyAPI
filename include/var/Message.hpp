/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved


#ifndef VAR_MESSAGE_HPP_
#define VAR_MESSAGE_HPP_

#include "../fmt/Son.hpp"
#include "../hal/Dev.hpp"

namespace var {

/*! \brief Message Class
 * \details The Message Class is used to send and receive
 * data in object notation.
 *
 * The Message Class formats messages using fmt::Son<4> objects.
 * The messages are sent and received using a device (usually a
 * FIFO but could be sent over a UART or SPI etc).
 *
 * Here is an example of sending a message.
 *
 * \code
 * Message message("/dev/multistream", 256, 2, 3); //read on multistream channel 2 and write on 3
 *
 * message.create(); //create a blank message
 * message.write("name", "HelloWorld"); //write a key value pair
 * message.write("version", "1.0"); //write another pair
 * message.write("size", 1024);
 *
 * message.set_timeout(500); //allow the receiver 500ms to receive the message
 *
 * if( message.send() < 0 ){
 *  printf("Failed to send message\n"); //this happens if no process is ready to read the message
 * }
 *
 * \endcode
 *
 * To receive a message:
 *
 * \code
 *
 * Message message("/dev/multistream", 256, 3, 2);
 *
 * if( message.recv() > 0 ){
 *  //got a message
 *  var::String value;
 *  message.read_str("name", value);
 *  printf("Name: %s\n", value.c_str()); //name is "HelloWorld"
 * }
 *
 * \endcode
 *
 *
 */
class Message : public Data, public fmt::Son<4> {
public:

	/*! \details Constructs a new message object.
	 *
	 * @param device The device used to send/receive messages
	 * @param max_capacity The maximum size of the message
	 * @param read_channel Device channel for reading
	 * @param write_channel Device channel for writing
	 */
	Message(const hal::Dev & device, int max_capacity);
	~Message();

	/*! \details Returns true if device is open.	 */
	bool is_open() const { return m_dev.fileno() >= 0; }

	/*! \details Creates a new empty message.
	 *
	 * @return Zero on success
	 *
	 * If a message previously existed, it will be discarded.
	 *
	 */
	virtual int create();

	/*! \details Configures the message for editing.
	 *
	 * @return Zero on success
	 */
	int edit();

	/*! \details Configures the message for reading.
	 *
	 * @return Zero on success
	 */
	int read();

	int close(){
		if( (m_state == CREATE_STATE) || (m_state == EDIT_STATE) || (m_state == READ_STATE) ){
			m_state = CLOSE_STATE;
			return Son<4>::close();
		}
		return 0;
	}

	/*! \details Calculates the current size of the
	 * message.
	 *
	 * @return Number of bytes currently occupied by the message.
	 *
	 * The message must be in read mode in order to calculate the size.
	 *
	 * See read().
	 */
	u32 calc_size();

	/*! \details Saves the message to a file in SON format.
	 *
	 * @param path The path to the file to create
	 * @return Zero on success
	 */
	int save(const char * path);

	/*! \details Loads a message from a file.
	 *
	 * @param path The path to load the message file
	 * @return Zero on success
	 */
	int load(const char * path);

	/*! \details Sends the message over the open connection.
	 *
	 * @return Zero on success
	 *
	 * \code
	 * Message message("/dev/multistream", 0, 1, 128);
	 * message.create();
	 * message.write("name", "Message");
	 * message.close();
	 * if( message.send() < 0 ){
	 *   printf("Failed to send message");
	 * }
	 * \endcode
	 */
	int send();

	/*! \details Receives a message on the connection.
	 *
	 * @return Zero on success
	 *
	 * \code
	 * Message message("/dev/multistream", 0, 1, 128);
	 *
	 * if( message.recv() == 0 ){
	 * 	printf("Message number is %d\n", message.read_num("num");
	 * }
	 * \endcode
	 */
	int recv();


	/*! \details Returns the timeout in milliseconds.
	 *
	 * The timeout defines how many milliseconds to wait before
	 * aborting when sending and receiving messages.
	 *
	 * The timeout is not absolute. If the channel becomes idle
	 * for timeout() milliseconds, the operation is aborted.
	 */
	int timeout() const { return m_timeout; }

	/*! \details Sets the timeout in milliseconds. */
	void set_timeout(int value) { m_timeout = value; }

protected:

	enum {
		INIT_STATE,
		CREATE_STATE,
		EDIT_STATE,
		READ_STATE,
		CLOSE_STATE
	};

	u8 state() const { return m_state; }


private:

	const hal::Dev & m_dev;

	typedef struct MCU_PACK {
		u32 start;
		u32 size;
		u32 checksum;
	} message_t;

	enum {
		MESSAGE_START = 0x01234567
	};

	u16 m_timeout;
	u8 m_state;


	bool recv_start();


	int send_data(const u8 *  data, int nbytes) const;
	int recv_data(u8 * data, int nbytes) const;


};

} /* namespace var */

#endif /* VAR_MESSAGE_HPP_ */
