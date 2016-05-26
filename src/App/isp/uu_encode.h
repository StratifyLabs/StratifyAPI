//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef UU_ENCODE_H_
#define UU_ENCODE_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


char uu_encode_line(char * dest_uu, void * src, uint8_t bytes);
char uu_decode_line(void * dest, char * src_uu, uint8_t bytes);

#ifdef __cplusplus
}
#endif

#endif /* UU_ENCODE_H_ */
