/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef BASE64_HPP_
#define BASE64_HPP_

#include "../api/CalcObject.hpp"
#include "../var/String.hpp"

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
 * #include <sapi/calc.hpp>
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
 * #include <sapi/calc.hpp>
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
class Base64 : public api::CalcInfoObject {
public:

	/*! \details Encodes data to the base64 format.
	 *
	 * @param dest Pointer to destination memory
	 * @param src Pointer to source data (binary)
	 * @param nbyte Number of bytes to encode
	 * @return Number of bytes in the encoded string
	 *
	 * \code
	 * #include <sapi/calc.hpp>
	 * #include <sapi/var.hpp>
	 *
	 * Data raw_data(64); //raw binary data that needs to be encoded
	 *
	 * String result = encode(raw_data);
	 * if( result.is_empty() ){
	 *  //failed to encode
	 * }
	 * \endcode
	 *
	 */
	static var::String encode(const var::Data & input);

	/*! \details Decodes base64 encoded data.
	 *
	 * @param dest Pointer to destination memory (binary format)
	 * @param src Pointer to source data (base64 encoded)
	 * @param nbyte Pointer the number of bytes to decode (src size)
	 * @return Zero
	 *
	 * \code
	 * #include <sapi/calc.hpp>
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
	 */
	static var::Data decode(const var::String & input);


private:
	static int encode(char * dest, const void * src, int nbyte);
	static int decode(void * dest, const char * src, int nbyte);
	static int calc_decoded_size(int nbyte);
	static int calc_encoded_size(int nbyte);
	static char encode_six(uint8_t six_bit_value);
	static char decode_eigth(uint8_t eight_bit_value);


};

}

#endif /* BASE64_HPP_ */
