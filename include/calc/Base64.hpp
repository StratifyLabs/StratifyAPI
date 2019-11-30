/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_CALC_BASE64_HPP_
#define SAPI_CALC_BASE64_HPP_

#include "../api/CalcObject.hpp"
#include "../var/Reference.hpp"
#include "../fs/File.hpp"

namespace calc {

/*! \brief Base64 Encode/Decode Class
 * \details This class includes methods to encode and decode data
 * using the base64 algorithm.  Base64 is useful for representing binary
 * data using text characters.  This can be useful when
 * transmitting data over certain serial links that do not support binary
 * transfers.
 *
 * The following example can be used to encode and decode
 * using Base64.
 *
 * First, include the Stratify API headers that we will
 * need for these examples.
 *
 * \code
 * //md2code:include
 * #include <sapi/calc.hpp>
 * #include <sapi/var.hpp>
 * #include <sapi/fs.hpp>
 * \endcode
 *
 * Now do some encoding and decoding like a boss.
 *
 * \code
 * //md2code:main
 * Data data_to_encode(128);
 * data_to_encode.fill<u8>(32);
 *
 * String encoded_string = Base64::encode(
 *   arg::SourceData(data_to_encode)
 * );
 *
 * //You can then decode the data using this code snippet:
 *
 * Data original_data = Base64::decode(encoded_string);
 *
 * if( original_data == data_to_encode ){
 *   printf("It works!\n");
 * }
 * \endcode
 *
 */
class Base64 : public api::InfoObject {
public:

	using SourceReference = var::Reference::Source;
	using DestinationReference = var::Reference::Destination;

	using SourceString = var::String::Source;
	using DestinationString = var::String::Destination;

	using Size = var::Reference::Size;
	using SourceFile = fs::File::Source;
	using DestinationFile = fs::File::Destination;

	/*! \details Encodes data to the base64 format.
	 *
	 * @return Number of bytes in the encoded string
	 *
	 * ```
	 * //md2code:main
	 *	Data raw_data(64); //raw binary data that needs to be encoded
	 *	raw_data.fill<u8>(0xaa);
	 *	String result = Base64::encode(arg::SourceData(raw_data));
	 *	printf("Encoded string is '%s'\n", result.cstring());
	 * ```
	 *
	 */
	static var::String encode(
			const var::Reference input
			);

	static int encode(
			SourceFile source,
			DestinationFile destination,
			Size size
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
	 *
	 * ```
	 * //md2code:main
	 *	File source;
	 *	File destination;
	 *
	 *	source.open(
	 *   "/home/raw_data.dat",
	 *   OpenFlags::read_only()
	 *   );
	 *
	 *	destination.create(
	 *   "/home/base64_encoded.txt",
	 *   File::IsOverwrite(true)
	 *   );
	 *
	 *	Base64::encode(
	 *   arg::SourceFile(source),
	 *   arg::DestinationFile(destination)
	 *   );
	 * ```
	 *
	 *
	 *
	 */
	static int encode(
			const var::Reference & source,
			var::String & destination,
			const Size size = Size(0)
			);

	/*! \details Decodes base64 encoded data.
	 *
	 * @return Zero
	 *
	 *
	 * ```
	 * //md2code:main
	 *	String encoded_string;
	 *	//assign base64 encoded string
	 *	Data raw_data;
	 *
	 *	raw_data = Base64::decode(encoded_string);
	 * ```
	 *
	 */
	static var::Data decode(
			const var::String & input
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
			SourceFile input,
			DestinationFile output,
			Size size = Size(0)
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
