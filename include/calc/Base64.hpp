/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef BASE64_HPP_
#define BASE64_HPP_

namespace calc {

/*! \brief Base64 Encode/Decode Class
 * \details This class includes methods to encode and decode data
 * using the base64 algorithm.  Base64 is useful for representing binary
 * data using text characters.  This can be useful when
 * transmitting data over certain serial links that do not support binary
 * transfers.
 *
 * To encode data in Base 64 format use this code:
 * \code
 * #include <stfy/calc.hpp>
 *
 * u8 raw_data[64]; //raw binary data that needs to be encoded
 * char * encoded_data;
 * int encoded_size;
 *
 * encoded_size = Base64::calc_encoded_size(64);
 * encoded_data = malloc(encoded_size);
 * Base64::encode(encoded_data, raw_data, 64);
 * \endcode
 *
 * You can then decode the data using this code snippet:
 *
 * \code
 * #include <stfy/calc.hpp>
 *
 * char encoded_data[64]; //Base64 encoded data that needs to be decoded
 * u8 * raw_data;
 * int decoded_size;
 *
 * decoded_size = Base64::calc_decoded_size(64);
 * raw_data = malloc(decoded_size);
 * Base64::decode(raw_data, encoded_data, 64);
 * \endcode
 *
 *
 *
 *
 *
 */
class Base64 {
public:

	/*! \details This method encodes data to the base64 format.
	 *
	 * \code
	 * #include <stfy/calc.hpp>
	 *
	 * u8 raw_data[64]; //raw binary data that needs to be encoded
	 * char * encoded_data;
	 * int encoded_size;
	 *
	 * encoded_size = Base64::calc_encoded_size(64);
	 * encoded_data = malloc(encoded_size);
	 * Base64::encode(encoded_data, raw_data, 64);
	 * \endcode
	 *
	 *
	 * @param dest Pointer to destination memory
	 * @param src Pointer to source data (binary)
	 * @param nbyte Number of bytes to encode
	 * @return Number of bytes in the encoded string
	 */
	static int encode(char * dest, const void * src, int nbyte);
	/*! \details Calculate encoded size of \a nbyte.  This function
	 * can be used to allocate memory for a buffer that can
	 * be used to hold encoded data.
	 *
	 * The number of encoded bytes is rougly \a nbyte * 4/3.
	 *
	 * @param nbyte Number of bytes to encode
	 * @return The string length required to encode \a nbyte bytes
	 */
	static int calc_encoded_size(int nbyte);
	/*! \details This methods decodes base64 encoded data.
	 *
	 *
	 * \code
	 * #include <stfy/calc.hpp>
	 *
	 * char encoded_data[64]; //Base64 encoded data that needs to be decoded
	 * u8 * raw_data;
	 * int decoded_size;
	 *
	 * decoded_size = Base64::calc_decoded_size(64);
	 * raw_data = malloc(decoded_size);
	 * Base64::decode(raw_data, encoded_data, 64);
	 * \endcode
	 *
	 * @param dest Pointer to destination memory (binary format)
	 * @param src Pointer to source data (base64 encoded)
	 * @param nbyte Pointer the number of bytes to decode (src size)
	 * @return Zero
	 */
	static int decode(void * dest, const char * src, int nbyte);
	/*! \details Calculate decoded size of \a nbyte.  This function
	 * can be used to allocate memory for a buffer that can
	 * be used to hold decoded data.
	 *
	 * The number of decoded bytes is rougly \a nbyte * 3/4.
	 *
	 *
	 * @param nbyte Number of bytes to decode
	 * @return The number of bytes needed to decode \a nbyte encoded bytes
	 *
	 */
	static int calc_decoded_size(int nbyte);

};
};

#endif /* BASE64_HPP_ */
