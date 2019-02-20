//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include <stdint.h>
#include <cstdio>
#include <cstring>
#include "calc/Base64.hpp"

using namespace calc;

int Base64::encode(const sys::File & input, sys::File & output, u32 size){
	u32 chunk_size = 96;
	const u32 output_buffer_size = calc_encoded_size(chunk_size);
	char input_buffer[chunk_size];
	char output_buffer[output_buffer_size];
	u32 size_processed = 0;
	int result;
	do {
		if( size - size_processed < chunk_size ){ chunk_size = size - size_processed; }
		result = input.read(input_buffer, chunk_size);
		if( result > 0 ){
			size_processed += result;
			int len = encode(output_buffer, input_buffer, result);
			if( output.write(output_buffer, len) != len ){
				result = 0;
			}
		}
	} while( (result > 0) && (size > size_processed) );
	return size_processed;
}

int Base64::decode(const sys::File & input, sys::File & output, u32 size){
	u32 chunk_size = 96;
	const u32 output_buffer_size = calc_decoded_size(chunk_size);
	char input_buffer[chunk_size];
	char output_buffer[output_buffer_size];
	u32 size_processed = 0;
	int result;
	do {
		if( size - size_processed < chunk_size ){ chunk_size = size - size_processed; }
		result = input.read(input_buffer, chunk_size);
		if( result > 0 ){
			size_processed += result;
			int len = decode(output_buffer, input_buffer, result);
			if( output.write(output_buffer, len) != len ){
				result = 0;
			}

		}
	} while( (result > 0) && (size > size_processed) );
	return size_processed;
}

var::String Base64::encode(const var::Data & input){
	var::String result;

	if( result.set_capacity( calc_encoded_size(input.size()) ) < 0 ){
		return var::String();
	}

	encode(result.to<char>(), input.to_void(), input.size());
	return result;
}

var::Data Base64::decode(const var::String & input){
	var::Data result;
	if( result.set_size( calc_decoded_size( input.length() ) ) < 0 ){
		return var::Data();
	}

	decode(result.to_void(), input.to_char(), input.length());
	return result;
}

int Base64::encode(char * dest, const void * src, int nbyte){
	int i;
	int j;
	int k;
	int len;
	u8 six_bits[4];
	len = nbyte;
	const char * data;
	data = (const char *)src;

	k = 0;
	//We need to encode three bytes at a time in to four encoded bytes
	for(i=0; i < len; i+=3){
		//First the thress bytes are broken down into six-bit sections
		six_bits[0] = (data[i] >> 2) & 0x3F;
		six_bits[1] = ((data[i] << 4) & 0x30) + ((data[i+1]>>4) & 0x0F);
		six_bits[2] = ((data[i+1] << 2) & 0x3C) + ((data[i+2]>>6) & 0x03);
		six_bits[3] = data[i+2] & 0x3F;
		//now we use the helper function to convert from six-bits to base64
		for(j=0; j < 4; j++){
			dest[k+j] = encode_six(six_bits[j]);
		}
		k+=4;
	}

	//at the end, we add = if the input is not divisible by 3
	if( (len % 3) == 1 ){
		//two equals at end
		dest[k-2] = '=';
		dest[k-1] = '=';
	} else if ( (len % 3) == 2 ){
		dest[k-1] = '=';
	}

	//finally, zero terminate the output string
	dest[k] = 0;

	return strlen(dest);
}

int Base64::calc_encoded_size(int nbyte){
	return ((((nbyte*4+2)/3) + 3)/4)*4;
}

int Base64::decode(void * dest, const char * src, int nbyte){
	char * data = (char *)dest;
	int i;
	int j;

	uint8_t eight_bits[4];

	for(i=0,j=0; j < nbyte; i+=3,j+=4){
		eight_bits[0] = decode_eigth(src[j]);
		eight_bits[1] = decode_eigth(src[j+1]);
		eight_bits[2] = decode_eigth(src[j+2]);
		eight_bits[3] = decode_eigth(src[j+3]);
		data[i] = (eight_bits[0] << 2) | (eight_bits[1] >> 4);
		data[i+1] = (eight_bits[1] << 4) | (eight_bits[2] >> 2);
		data[i+2] = (eight_bits[2] << 6) | (eight_bits[3]);
	}

	return 0;
}

int Base64::calc_decoded_size(int nbyte){
	return (nbyte*3+3)/4;
}

//This is a helper function to convert a six-bit value to base64
char Base64::encode_six(uint8_t six_bit_value){
	uint8_t x;
	char c = -1;
	x = six_bit_value & ~0xC0; //remove top two bits (should be zero anyway)
	if( x < 26 ){
		c = 'A' + x;
	} else if ( x < 52 ){
		c = 'a' + (x - 26);
	} else if( x < 62 ){
		c = '0' + (x - 52);
	} else if( x == 62 ){
		c = '+';
	} else if (x == 63 ){
		c = '/';
	}
	return c;
}

char Base64::decode_eigth(uint8_t eight_bit_value){
	uint8_t x;
	x = eight_bit_value;
	if( (x >= 'A') && (x <= 'Z') ){
		return x - 'A';
	}

	if( (x >= 'a') && (x <= 'z') ){
		return x - 'a' + 26;
	}

	if( (x >= '0') && (x <= '9') ){
		return x - '0' + 52;
	}

	if( x == '+' ){
		return 62;
	}

	if( x == '/' ){
		return 63;
	}

	return 0;
}

