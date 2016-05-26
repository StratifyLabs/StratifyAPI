/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef BASE64_HPP_
#define BASE64_HPP_

namespace calc {

/*! \brief Base64 Encode/Decode Class
 * \details This class includes methods to encode and decode data
 * using the base64 algorithm.  Base64 is useful for representing binary
 * data using only valid text characters.  This can be useful when
 * transmitting data over certain serial links that do not support binary
 * transfers.
 */
class Base64 {
public:
	Base64();

	/*! \details This method encodes data to the base64 format.
	 *
	 * @param dest Pointer to destination memory
	 * @param src Pointer to source data (binary)
	 * @param nbyte Number of bytes to encode
	 * @return Number of bytes in the encoded string
	 */
	static int encode(char * dest, const void * src, int nbyte);
	/*! \details Calculate encoded size of \a nbyte.  This function
	 * can be used to allocate memory for a buffer that can
	 * be used to hold encoded data
	 *
	 * @param nbyte Number of bytes to encode
	 * @return The string length if the bytes are encoded
	 */
	static int calc_encoded_size(int nbyte);
	/*! \details This methods decodes base64 encoded data.
	 *
	 * @param dest Pointer to destination memory (binary format)
	 * @param src Pointer to source data (base64 encoded)
	 * @param nbyte Pointer the number of bytes to decode (src size)
	 * @return Zero
	 */
	static int decode(void * dest, const char * src, int nbyte);
	/*! \details Calculate decoded size of \a nbyte.  This function
	 * can be used to allocate memory for a buffer that can
	 * be used to hold decoded data
	 *
	 * @param nbyte Number of bytes to decode
	 * @return The string length if the bytes are encoded
	 *
	 */
	static int calc_decoded_size(int nbyte);

};
};

#endif /* BASE64_HPP_ */
