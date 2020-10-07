/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef SAPI_CALC_BASE64_HPP_
#define SAPI_CALC_BASE64_HPP_

#include "var/View.hpp"

namespace var {

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
class Base64 : public api::Object {
public:
  var::String encode(var::View input);
  var::Data decode(var::StringView input);

private:
  static int encode(char *dest, const void *src, int nbyte);
  static int decode(void *dest, const char *src, int nbyte);
  static int calc_decoded_size(int nbyte);
  static int calc_encoded_size(int nbyte);
  static char encode_six(u8 six_bit_value);
  static char decode_eigth(u8 eight_bit_value);
};

} // namespace var

#endif /* SAPI_CALC_BASE64_HPP_ */
