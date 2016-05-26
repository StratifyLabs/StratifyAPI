/*
 * LpcIsp.cpp
 *
 *  Created on: Jun 1, 2013
 *      Author: tgil
 */

#include <unistd.h>
#include <stdlib.h>
#include "isp/Lpc.hpp"
using namespace isp;

#include "../isp/isplib.h"
#include "../isp/lpc_devices.h"

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 2
#endif


const char * device;

static const char * device_list[] = {
		"lpc1342",
		"lpc1343",
		"lpc1751",
		"lpc1752",
		"lpc1754",
		"lpc1756",
		"lpc1757",
		"lpc1758",
		"lpc1764",
		"lpc1765",
		"lpc1767",
		"lpc1768",
		"lpc1769",
		NULL
};


int Lpc::copy_names(char * device, char * pio0, char * pio1){
	strcpy(device, "lpc");
	strcpy(pio0, "Reset");
	strcpy(pio1, "ISP Req");
	return 0;
}


int Lpc::program(const char * filename, int crystal, const char * dev, int (*progress)(float)){
	FILE * f;
	u8 * image_buffer;
	u32 size;
	u32 bytes_read;
	u32 start_address;
	u8 failed;

	device = dev;

	if ( init_prog_interface(crystal) ){
		isplib_error("Failed to start interface\n");
		return -1;
	}

	if ( erase_dev() ){
		isplib_error("Failed to erase device\n");
		return -1;
	}

	f = fopen(filename, "rb");
	if ( f != NULL ){
		fseek(f, 0, SEEK_END); // seek to end of file
		size = ftell(f); // get current file pointer
		fclose(f);
	} else {
		isplib_error("Could not open file %s\n", filename);
		return -1;
	}

	if (!size){
		isplib_error("Error:  Binary File Error\n");
		return -2;
	}

	isplib_debug(DEBUG_LEVEL, "File size is %d\n", (int)size);
	image_buffer = (u8*)malloc( size );
	if (!image_buffer){
		isplib_error("Failed to allocate memory\n");
		return -3;
	}
	memset(image_buffer, 0xFF, size);

	f = fopen(filename, "rb");

	if ( f != NULL ){
		if ( (bytes_read = fread(image_buffer, 1, size, f)) != size ){
			isplib_error("Could not read file %s (%d of %d bytes read)\n", filename, bytes_read, size);
			fclose(f);
			return -1;
		}
		fclose(f);
	} else {
		isplib_error("Could not open file %s to read image\n", filename);
		return -1;
	}

	start_address = 0;

	isplib_debug(DEBUG_LEVEL, "Starting address is %d\n", start_address);

	//Write the patch to the vector checksum
	if ( write_vector_checksum(image_buffer, dev) ) {
		isplib_error("Device %s is not supported\n", dev);
		return -1;
	}

	phy.set_ram_buffer( lpc_device_get_ram_start(dev) + 0x300 );

	//Write the program memory
	failed = 0;
	if ( !write_progmem(image_buffer, start_address, size, progress) ){
		failed = 1;
		isplib_error("Failed to write program memory\n");
		return -1;
	}

	prog_shutdown();

	if ( !failed ){
		isplib_debug(DEBUG_LEVEL, "Device Successfully Programmed\n");
	} else {
		isplib_error("Device Failed to program correctly\n");
	}

	free(image_buffer);

	return 0;

}

int Lpc::read(const char * filename, int crystal, int (*progress)(float)){
	FILE * f;
	int bytes_read;
	char data[256*1024];

	if ( init_prog_interface(crystal) ){
		return -1;
	}

	f = fopen(filename, "w");
	if (!f){
		isplib_error("Could not create file %s\n", filename);
		return -2;
	}

	while( (bytes_read = (int)read_progmem(data, 0, 256*1024, progress)) > 0 ){
		if ( fwrite(data, 1, bytes_read, f) != (u32)bytes_read ){
			fclose(f);
			isplib_error("Failed to write data to file\n");
			return -1;
		}
	}

	fclose(f);

	return 0;
}

char ** Lpc::getlist(){
	return (char**)device_list;
}


/*! \brief initializes the UART interface and starts the bootloader.
 * \details This function initializes the UART interface and starts the bootloader
 * at the specified speed.  If speed is < 0, an algorithm is used to detect the maximum
 * speed.
 */

int Lpc::init_prog_interface(int crystal){

	//Open the ISP interface using phy.open()
	if ( phy.open(crystal) == 0 ){
		return 0;
	}

	phy.close();
	return -1;
}



/*! \brief reads the program memory over the specified range.
 * \details .
 *
 * \return Number of bytes read
 */
u32 Lpc::read_progmem(void * data, u32 addr, u32 size, int (*update_disp)(float)){
	u32 buffer_size;
	u32 bytes_read;
	u16 page_size;

	//First read the buffer size
	buffer_size = LPCPHY_RAM_BUFFER_SIZE;
	bytes_read = 0;

	do {
		if ( (size-bytes_read) > buffer_size ) page_size = buffer_size;
		else page_size = size-bytes_read;

		if ( phy.readmem(addr + bytes_read, &((char*)data)[bytes_read], page_size) != page_size ){

			isplib_error("Error reading data at address 0x%04X\n", (u32)(addr + bytes_read));
			return bytes_read;
		}


		usleep(2*1000);
		bytes_read += page_size;

		if ( update_disp ){
			if ( update_disp((float)(bytes_read*100.0 / size)) ){
				return 0; //abort requested
			}
		}

	} while ( bytes_read < size );

	return bytes_read;
}

u32 Lpc::write_progmem(void * data, u32 addr, u32 size, int (*update_disp)(float)){
	int buffer_size;
	int bytes_written;
	int page_size;
	int j;
	int sector;
	//char err;

	//First read the buffer size
	buffer_size = LPCPHY_RAM_BUFFER_SIZE;
	bytes_written = 0;
	do {

		if ( (int)(size-bytes_written) > buffer_size ) page_size = buffer_size;
		else page_size = size-bytes_written;

		//Check to see if data is already all 0xFF
		for(j=0; j < page_size; j++){
			if ( ((unsigned char*)data)[bytes_written+j] != 0xFF ) break;
		}

		if ( j < page_size ){ //only write if data has non 0xFF values
			isplib_debug(DEBUG_LEVEL+1, "lpc_wr_pgmmem():Writing page starting at %d\n", addr + bytes_written);
			sector = lpc_device_get_sector_number(device, addr+bytes_written);
			if ( phy.writemem(addr + bytes_written,
					&((char*)data)[bytes_written], page_size,
					sector ) != page_size ){
				isplib_error("writing data at address 0x%04X\n", (u32)(addr + bytes_written));
				return 0;
			}
		}
		//delay_ms(2);
		bytes_written+=page_size;
		if ( update_disp ){
			if ( update_disp((float)(bytes_written*100.0 / size)) ){
				return 0; //abort requested
			}
		}
	} while( bytes_written < (int)size );

	return bytes_written;
}


int Lpc::erase_dev(){
	int sectors;
	int ret;

	//first see how many sectors there are
	sectors = 0;
	do {
		ret = phy.prep_sector(0, sectors);
		if ( !ret ){
			sectors++;
		}
	} while ( !ret );

	//Now erase all the sectors
	ret = phy.erase_sector(0, --sectors);
	if ( ret != 0 ){
		isplib_error("Failed to erase device\n");
		return -1;
	}

	ret = phy.blank_check_sector(1, sectors);
	if ( ret < 0 ){
		isplib_error("Device not blank\n");
		return ret;
	}

	return 0;
}

int Lpc::write_vector_checksum(unsigned char * hex_buffer, const char * dev){
	unsigned long checksum;
	checksum = 0;
	u16 i;
	int32_t addr;

	//Get the device specific checksum address
	addr = lpc_device_get_checksum_addr(dev);

	if ( addr < 0 ){
		isplib_error("Device %s not supported\n", dev);
		return -1;
	}


	// Clear the vector at 0x14 so it doesn't affect the checksum:
	for (i = 0; i < 4; i++)
	{
		hex_buffer[i + addr] = 0;
	}

	// Calculate a native checksum of the little endian vector table:
	for (i = 0; i < (4 * 8);) {
		checksum += hex_buffer[i++];
		checksum += hex_buffer[i++] << 8;
		checksum += hex_buffer[i++] << 16;
		checksum += hex_buffer[i++] << 24;
	}


	checksum = (unsigned long) (0 - checksum);
	for (i = 0; i < 4; i++)
	{
		hex_buffer[i + addr] = (unsigned char)(checksum >> (8 * i));
	}

	isplib_debug(DEBUG_LEVEL+1, "Position %X patched: checksum = 0x%08lX\n", addr, checksum);
	return 0;
}


/*! \brief starts the user code and closes the UART.
 * \details This function starts the user code and then
 * closes the UART connection.
 * \return Zero on success
 */
int Lpc::prog_shutdown(){
	int err;
	isplib_debug(DEBUG_LEVEL, "Restarting the device\n");

	err = phy.reset();
	if ( err ){
		isplib_error("Could not reset device (%d)\n", err);
		return -1;
	}

	err = phy.close();
	if ( err ){
		isplib_error("Failed to close ISP interface (%d)\n", err);
		return -1;
	}

	return 0;
}

/*! @} */

