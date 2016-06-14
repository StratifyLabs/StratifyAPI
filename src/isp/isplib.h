//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved



/*! \addtogroup ISPLIB ISP Library
 * @{
 *
 */

/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef ISPLIB_H_
#define ISPLIB_H_

#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG_LEVEL_MAX 100
#define DEBUG_ERROR

#if (DEBUG_LEVEL_MAX > 0) && defined __link
#include <stdio.h>
#define isplib_debug(LEVEL, ...) do { if ( LEVEL <= DEBUG_LEVEL_MAX){ printf("%s: ", __func__); printf(__VA_ARGS__); fflush(stdout); }} while(0)
#define isplib_error(...) do { printf("ERROR:%s: ", __func__); printf(__VA_ARGS__); fflush(stdout); } while(0)
#else
#define isplib_debug(LEVEL, ...)
#endif

#if defined DEBUG_ERROR && defined __link
#define isplib_error(...) do { printf("ERROR:%s: ", __func__); printf(__VA_ARGS__); fflush(stdout); } while(0)
#else
#define isplib_error(...)
#endif


#ifdef __cplusplus
}
#endif

#endif /* ISPLIB_H_ */

/*! @} */
