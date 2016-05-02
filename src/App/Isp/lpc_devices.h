/*! \addtogroup LPC21
 * @{
 *
 */

/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef LPC_DEVICES_H_
#define LPC_DEVICES_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int32_t lpc_device_get_checksum_addr(const char * dev);
uint32_t lpc_device_get_ram_start(const char * dev);
uint32_t lpc_device_get_sector_number(const char * dev, uint32_t addr);

#ifdef __cplusplus
}
#endif

#endif /* LPC_DEVICES_H_ */


/*! @} */
