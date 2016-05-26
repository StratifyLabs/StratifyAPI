/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef RLE_HPP_
#define RLE_HPP_

#include <mcu/types.h>

#include "../sys/Appfs.hpp"
#include "../sys/File.hpp"

namespace calc {

/*! \brief Run Length Encoding Class */
/*! \details This class implements Run length encoding and
 * decoding algorithms.
 *
 */
class Rle {
public:
	Rle();

	/*! \details This method encodes a block of data.
	 *
	 * @param dest A pointer to the destination data
	 * @param dest_size Pass the max size of dest, this will hold the number of encoded bytes upon return
	 * @param src A pointer to the source data
	 * @param src_size The number of bytes to encode
	 * @return Number of un-encoded bytes that were processed
	 */
	static int encode(void * dest, ssize_t & dest_size, const void * src, ssize_t src_size);

	/*! \details This method decodes a block of data.
	 *
	 * @param dest A pointer to the destination data
	 * @param dest_size Pass the max size of dest, this will hold the number of decoded bytes upon return
	 * @param src A pointer to the encoded data
	 * @param src_size The number of encoded bytes to process
	 * @return Number of encoded bytes that were processed
	 */
	static int decode(void * dest, ssize_t & dest_size, const void * src, ssize_t src_size);

	/*! \details This methods calculates the number of bytes that will be used by the
	 * compressed data.
	 *
	 * @param src A pointer to the source memory
	 * @param size The number of bytes to process
	 * @return The number of bytes the data would occupy after compression
	 */
	static int calc_size(const void * src, int nbyte);

private:
	typedef struct MCU_PACK {
		u8 size;
		u8 data;
	} element_t;

	typedef struct MCU_PACK {
		u32 size;
		u32 data;
	} element32_t;

};

/*! \brief Class for reading/writing compressed file data */
class RleFile : public Rle, public sys::File {
public:

	/*! \details This method encodes then writes the data to a file.
	 *
	 * @param buf The source data
	 * @param nbyte The number of bytes to encode and write
	 * @return The number of un-encoded bytes that were written
	 */
	int write(const void * buf, int nbyte);


	/*! \details This method reads and decodes data from a file.
	 *
	 * @param buf A pointer to the destination memory
	 * @param nbyte The maximum number of bytes to read
	 * @return The number of bytes read after decoding
	 */
	int read(void * buf, int nbyte);

private:
	enum {
		BUF_SIZE = 256
	};
	char buf[BUF_SIZE];
};

/*! \brief Class for reading/writing compressed file data using Appfs */
class RleAppfs : public Rle, public sys::Appfs {
public:
	RleAppfs();

	/*! \details This method encodes then writes the data to a file.
	 *
	 * @param buf The source data
	 * @param nbyte The number of bytes to encode and write
	 * @return The number of un-encoded bytes that were written
	 */
	int write(const void * buf, int nbyte);


	/*! \details This method reads and decodes data from a file.
	 *
	 * @param buf A pointer to the destination memory
	 * @param nbyte The number of bytes to read
	 * @return The number of bytes read after decoding
	 */
	int read(void * buf, int nbyte);

};

};
#endif /* RLE_HPP_ */
