/*! \addtogroup LPC21
 * @{
 *
 */

/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "isplib.h"
#include "lpc_devices.h"
#include <string.h>
#include <stdint.h>

typedef struct {
	char prefix[12];
	uint32_t checksum_addr;
	uint32_t ram_start;
	uint16_t sectors;
	uint16_t sector_table[128];
} lpc_device_t;


#define TOTAL_DEVICES 3

static lpc_device_t devices[] = {
		{
				.prefix = "lpc21",
				.checksum_addr = 0x14,
				.ram_start = 0x40000000
		},
		{
				.prefix = "lpc13",
				.checksum_addr = 0x1C,
				.ram_start = 0x10000000,
				.sectors = 8,
				.sector_table[0] = 4096,
				.sector_table[1] = 4096,
				.sector_table[2] = 4096,
				.sector_table[3] = 4096,
				.sector_table[4] = 4096,
				.sector_table[5] = 4096,
				.sector_table[6] = 4096,
				.sector_table[7] = 4096
		},

		{
				.prefix = "lpc17",
				.checksum_addr = 0x1C,
				.ram_start = 0x10000000,
				.sectors = 30,
				.sector_table[0] = 4096,
				.sector_table[1] = 4096,
				.sector_table[2] = 4096,
				.sector_table[3] = 4096,
				.sector_table[4] = 4096,
				.sector_table[5] = 4096,
				.sector_table[6] = 4096,
				.sector_table[7] = 4096,
				.sector_table[8] = 4096,
				.sector_table[9] = 4096,
				.sector_table[10] = 4096,
				.sector_table[11] = 4096,
				.sector_table[12] = 4096,
				.sector_table[13] = 4096,
				.sector_table[14] = 4096,
				.sector_table[15] = 4096,
				.sector_table[16] = 32768,
				.sector_table[17] = 32768,
				.sector_table[18] = 32768,
				.sector_table[19] = 32768,
				.sector_table[20] = 32768,
				.sector_table[21] = 32768,
				.sector_table[22] = 32768,
				.sector_table[23] = 32768,
				.sector_table[24] = 32768,
				.sector_table[25] = 32768,
				.sector_table[26] = 32768,
				.sector_table[27] = 32768,
				.sector_table[28] = 32768,
				.sector_table[29] = 32768,
				.sector_table[30] = 32768,
				.sector_table[31] = 32768


		}
};


static uint32_t lpc_device_lookup_sector_by_dev(int dev, uint32_t addr);

int32_t lpc_device_get_checksum_addr(const char * dev){
	int i;
	for(i=0; i < TOTAL_DEVICES; i++){
		isplib_debug(3, "Checking for %s at 0x%X\n", devices[i].prefix, devices[i].checksum_addr);
		if ( !strncmp(dev, devices[i].prefix, 5) ){
			return devices[i].checksum_addr;
		}
	}
	return -1;
}


uint32_t lpc_device_get_ram_start(const char * dev){
	int i;
	for(i=0; i < TOTAL_DEVICES; i++){
		if ( !strncmp(dev, devices[i].prefix, 5) ){
			return devices[i].ram_start;
		}
	}
	return -1;
}

uint32_t lpc_device_get_sector_number(const char * dev, uint32_t addr){
	int i;
	for(i=0; i < TOTAL_DEVICES; i++){
		if ( !strncmp(dev, devices[i].prefix, 5) ){
			return lpc_device_lookup_sector_by_dev(i, addr);
		}
	}
	return -1;
}

uint32_t lpc_device_lookup_sector_by_dev(int dev, uint32_t addr){
	uint32_t tmp = 0;
	int i;
	i = 0;
	do {
		tmp += devices[dev].sector_table[i++];
		if ( i == devices[dev].sectors ){
			break;
		}
	} while( tmp <= addr);
	return --i;
}



/*! @} */

