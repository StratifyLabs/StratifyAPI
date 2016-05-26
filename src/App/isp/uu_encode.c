//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved



#include "../isp/uu_encode.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "../isp/isplib.h"

static void uu_encode_word24(char * dest_uu, char * src);
static void uu_decode_word24(char * dest, char * src_uu);
static char uu_encode_char6(char in);
static char uu_decode_char8(char in);
unsigned short CRC = 0x0000;  /* CRC-16 accumulator */

/*! \brief encodes a six bit word.
 *
 */
static char uu_encode_char6(char in){
	char out;
	out = (in & 0x3F) + ' ';
	if ( out == ' ' ){
		out = '`';
	}
	return out;
}


static char uu_decode_char8(char in){
	char out;
	out = (in - ' ') & 0x3F;
	return out;
}


static void uu_encode_word24(char * dest_uu, char * src){
	//encodes 3 bytes of data using UU encoding
	int i;
	char b[4];
	isplib_debug(5, "Input is %X %X %X\n",
				(uint8_t)src[0],
				(uint8_t)src[1],
				(uint8_t)src[2] );
	b[0] = (src[0] & 0xFC) >> 2;
	b[1] = ((src[0] & 0x03) << 4) + ((src[1] & 0xF0) >> 4);
	b[2] = ((src[1] & 0x0F) << 2) + ((src[2] & 0xC0) >> 6);
	b[3] = (src[2] & 0x3F);
	for(i=0; i < 4; i++){
		dest_uu[i] = uu_encode_char6( b[i] );
	}
	isplib_debug(5, "Output is %X %X %X %X\n",
					(uint8_t)dest_uu[0],
					(uint8_t)dest_uu[1],
					(uint8_t)dest_uu[2],
					(uint8_t)dest_uu[3]);
}


static void uu_decode_word24(char * dest, char * src_uu){
	uint8_t i;
	char uu_dec[4];

	isplib_debug(5, "uu_decode_word24():Input is %X %X %X %X\n",
			(uint8_t)src_uu[0],
			(uint8_t)src_uu[1],
			(uint8_t)src_uu[2],
			(uint8_t)src_uu[3]);

	for(i=0; i < 4; i++) uu_dec[i] = uu_decode_char8(src_uu[i]);


	isplib_debug(5, "uu_decode_word24():Dec step 1 is %X %X %X %X\n",
			(uint8_t)uu_dec[0],
			(uint8_t)uu_dec[1],
			(uint8_t)uu_dec[2],
			(uint8_t)uu_dec[3]);


	dest[0] = ((uu_dec[0] & 0x3F) << 2) + ((uu_dec[1] & 0x30) >> 4);
	dest[1] = ((uu_dec[1] & 0x0F) << 4) + ((uu_dec[2] & 0x3C) >> 2);
	dest[2] = ((uu_dec[2] & 0x03) << 6) + (uu_dec[3] & 0x3F);


	isplib_debug(5, "uu_decode_word24():Output is %X %X %X\n",
			(uint8_t)dest[0],
			(uint8_t)dest[1],
			(uint8_t)dest[2]);


}

/*! \brief encodes a line using uu encoding.
 * \details This function encodes a line using uu encoding.
 * \return Length of encoded byte stream
 */
char uu_encode_line(char * dest_uu /*! Pointer to the destination (= bytes * (1 + 1/3) ) */,
		void * src /*! Pointer to the source (= to bytes) */,
		uint8_t bytes /*! The number of source bytes to encode */){

	char tmp[64];
	uint8_t i;
	uint8_t j;
	char * dest = (char*)dest_uu;
	memset(tmp, 0, 64);
	memcpy(tmp, src, bytes);
	dest[0] = (bytes & 0x3F) + ' ';  //this is the number of bytes encoded


	j = 1;
	for(i=0; i < bytes; i+=3){
		uu_encode_word24(&dest[j], &tmp[i]);
		j+=4;
	}



	dest[j] = '\r';
	dest[j+1] = '\n';
	dest[j+2] = 0;

	return strlen(dest);
}

/*! \brief decodes a uu encoded line.
 * \details This function decodes a uu encoded line.
 * \return Length of decoded byte stream
 */

char uu_decode_line(void * dest, char * src_uu, uint8_t bytes){
	uint8_t i, j;
	char * dest_p = (char*)dest;
	char * src_p = (char*)src_uu;
	uint8_t len;
	uint8_t len_check;
	//uint8_t en_bytes;

	len = 0;
	//en_bytes = (bytes * 4+2) / 3;

	//isplib_debug(4, "Encoded bytes is %d\n", en_bytes);
	//if ( en_bytes > 60 ) return 0;
	//Check for proper line length
	len_check = (src_p[0] - ' ') & 0x3F; //according to line decoding
	if ( len_check > 45 ) return 0;
	for(i=1; i < len_check*4/3+8; i++){
		if ( (src_uu[i] == '\r') || (src_uu[i] == '\n') ){
			len = i-1; //ignore the first char (length) and the last char '\r'
			break;
		}
	}
	memset(dest, 0, bytes);


	len_check = (src_p[0] - ' ') & 0x3F; //according to line decoding

	if ( ((len_check * 4) <= (len * 3 - 12)) ||
			(len_check * 4 ) > (len*3) ){
		isplib_debug(4, "check is %d, len is %d\n", len_check, len);
		//return 0;
	}

	j = 1;
	for(i=0; i < len; i+=3){
		uu_decode_word24(&dest_p[i], &src_p[j]);
		j+=4;
	}

	return len_check;
}

