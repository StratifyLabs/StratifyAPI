//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#ifndef BINARY_H_
#define BINARY_H_

#include <stdint.h>

/* turn a numeric literal into a hex constant
(avoids problems with leading zeroes)
8-bit constants max value 0x11111111, always fits in unsigned long
*/
#define HEX__(n) 0x##n##LU

/* 8-bit conversion function */
#define B8__(x) ((x&0x0000000FLU)?1:0)	\
+((x&0x000000F0LU)?2:0)	\
+((x&0x00000F00LU)?4:0)	\
+((x&0x0000F000LU)?8:0)	\
+((x&0x000F0000LU)?16:0)	\
+((x&0x00F00000LU)?32:0)	\
+((x&0x0F000000LU)?64:0)	\
+((x&0xF0000000LU)?128:0)


#define B8(d) ((unsigned char)B8__(HEX__(d)))

/* for upto 16-bit binary constants, MSB first */
#define B16(dmsb,dlsb) (((unsigned short)B8(dmsb)<<8)	\
+ B8(dlsb))

/* for upto 32-bit binary constants, MSB first */
#define B32(dmsb,db2,db3,dlsb) (((unsigned long)B8(dmsb)<<24)	 \
+ ((unsigned long)B8(db2)<<16) \
+ ((unsigned long)B8(db3)<<8)	 \
+ B8(dlsb))



#endif /* BINARY_H_ */
