/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_CALC_BASE64_HPP_
#define SAPI_CALC_BASE64_HPP_

#include "../api/CalcObject.hpp"
#include "../var/String.hpp"
#include "../fs/File.hpp"

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
	 * @return Number of bytes in the encoded string
	 *
	 * ```
	 * #include <sapi/calc.hpp>
	 * #include <sapi/var.hpp>
	 *
	 * Data raw_data(64); //raw binary data that needs to be encoded
	 *
	 * String result = encode(raw_data);
	 * if( result.is_empty() ){
	 *  //failed to encode
	 * }
	 * ```
	 *
	 */
	static var::String encode(
			const arg::ImplicitSourceData input
			);


	/*! \details Reads binary data from *input* and writes a Base64
	 * encoded string to *output*.
	 *
	 * @return Number of encoded bytes written to *output*
	 *
	 * The method reads *size* bytes (or to EOF if *size* is zero) from *input*
	 * start at the current location. The output string is written to *output*
	 * at the current location.
	 *
	 * ```
	 * #include <sapi/calc.hpp>
	 * #include <sapi/fs.hpp>
	 *
	 * File source;
	 * File destination;
	 *
	 * source.open(
	 *   arg::FilePath("/home/raw_data.dat"),
	 *   OpenFile::read_only()
	 *   );
	 *
	 * destination.create(
	 *   arg::DestinationFilePath("/home/base64_encoded.txt"),
	 *   arg::IsOverwrite(true)
	 * );
	 *
	 * Base64::encode(
	 *   arg::SourceFile(source),
	 *   arg::DestinationFile(destination),
	 *   );
	 * ```
	 *
	 *
	 *
	 */
	static int encode(
			const arg::SourceFile source,
			const arg::DestinationFile destination,
			const arg::Size size = arg::Size(0)
			);

	/*! \details Decodes base64 encoded data.
	 *
	 * @return Zero
	 *
	 * ```
	 * #include <sapi/calc.hpp>
	 * #include <sapi/var.hpp>
	 *
	 * String encoded_string;
	 * //assign base64 encoded string
	 * Data raw_data;
	 *
	 * raw_data = Base64::decode(encoded_string);
	 * ```
	 *
	 */
	static var::Data decode(
			const arg::ImplicitBase64EncodedString input
			);

	/*! \details Reads base64 encoded data from *input* and writes raw,
	 * decoded data to *output*.
	 *
	 * @return Number of decoded bytes written to *output*
	 *
	 * The method reads *size* bytes (or to EOF if *size* is zero) from *input*
	 * start at the current location. The output string is written to *output*
	 * at the current location.
	 *
	 *
	 */
	static int decode(
			const arg::SourceFile input,
			const arg::DestinationFile output,
			const arg::Size size = arg::Size(0)
			);


private:
	static int encode(char * dest, const void * src, int nbyte);
	static int decode(void * dest, const char * src, int nbyte);
	static int calc_decoded_size(int nbyte);
	static int calc_encoded_size(int nbyte);
	static char encode_six(uint8_t six_bit_value);
	static char decode_eigth(uint8_t eight_bit_value);


};

}

#endif /* SAPI_CALC_BASE64_HPP_ */
