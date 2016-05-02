/*
 * mbfont.h
 *
 *  Created on: Nov 22, 2013
 *      Author: tgil
 */

#ifndef MBFONT_H_
#define MBFONT_H_


#include <mcu/types.h>

/*! \brief Defines the header for a monochrome bitmap font */
/*! \details A monochrome bitmap font looks like this in memory (or a file):
 *
 * mbfont_hdr_t hdr;
 * mbfont_char_t char0;
 * mbfont_char_t char1;
 * ...
 * mbfont_char_t char94;
 * const char char0_data[char0.height][MFont::byte_width(char0.width)];
 * const char char1_data[char1.height][MFont::byte_width(char1.width)];
 * ...
 * const char char94_data[char0.height][MFont::byte_width(char94.width)];
 * mbfont_kerning_pair_t kerning0;
 * mbfont_kerning_pair_t kerning1;
 * ...
 *
 */
typedef struct MCU_PACK {
	u8 num_chars;
	u8 max_byte_width;
	u8 max_height;
	u8 resd0;
	u32 size;
	u16 kerning_pairs;
	u16 resd1;
} mbfont_hdr_t;


typedef struct MCU_PACK {
	u16 id;
	u16 offset;
	u8 width;
	u8 height;
	u8 xadvance;
	u8 resd;
	i16 xoffset;
	i16 yoffset;
} mbfont_char_t;

typedef struct MCU_PACK {
	u8 first;
	u8 second;
	i16 kerning;
} mbfont_kerning_pair_t;


#endif /* MBFONT_H_ */
