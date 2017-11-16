/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved


#ifndef VAR_MESSAGE_HPP_
#define VAR_MESSAGE_HPP_

#include "../fmt/Son.hpp"
#include "../sys/File.hpp"

namespace var {

class Message : public Data, public fmt::Son<4> {
public:
	Message(const char * device, int read_channel, int write_channel, int max_capacity);
	~Message();

	/*! \details Creates a new empty message.
	 *
	 * @return Zero on success
	 *
	 * If a message previously existed, it will be discarded.
	 *
	 */
	int create();

	/*! \details Configures the message for editing.
	 *
	 * @return Zero on success
	 */
	int edit();

	int size();

	/*! \details Saves the message to a file.
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


private:

	int m_fd;

	typedef struct MCU_PACK {
		u32 start;
		u32 size;
		u32 data_checksum;
		u32 checksum;
	} message_t;

	enum {
		MESSAGE_START = 0x01234567
	};

	u16 m_timeout;

	u16 m_read_channel;
	u16 m_write_channel;


};

} /* namespace var */

#endif /* VAR_MESSAGE_HPP_ */
