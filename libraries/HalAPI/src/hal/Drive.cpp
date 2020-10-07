/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "hal/Drive.hpp"
#include "sys/Printer.hpp"

sys::Printer& sys::operator << (sys::Printer& printer, const hal::DriveInfo & a){
	printer.key("addressableSize", "%ld", a.addressable_size());
	printer.key("eraseBlockSize", "%ld", a.erase_block_size());
	printer.key("eraseBlockTime", "%ldus", a.erase_block_time().microseconds());
	printer.key("eraseDeviceTime", "%ldms", a.erase_device_time().milliseconds());
	printer.key("frequency", "%ld", a.frequency());
	printer.key("events", "0x%lX", a.o_events());
	printer.key("flags", "0x%lX", a.o_flags());
	printer.key("pageProgramSize", "%ld", a.page_program_size());
	printer.key("writeBlockCount", "%ld", a.write_block_count());
	printer.key("writeBlockSize", "%ld", a.write_block_size());
	return printer;
}


using namespace hal;

Drive::Drive(){

}

int Drive::initialize(
		const var::String & path
		){

	if( fileno() < 0 ){
		if( open(
				 path,
				 fs::OpenFlags::read_write()
				 ) < 0 ){
			return return_value();
		}
	}

	drive_attr_t attr;
	attr.o_flags = DRIVE_FLAG_INIT;
	return set_attributes(attr);
}


int Drive::powerup() const {
	drive_attr_t attr;
	attr.o_flags = DRIVE_FLAG_POWERUP;
	return set_attributes(attr);
}

int Drive::powerdown() const {
	drive_attr_t attr;
	attr.o_flags = DRIVE_FLAG_POWERDOWN;
	return set_attributes(attr);
}

int Drive::reset() const {
	drive_attr_t attr;
	attr.o_flags = DRIVE_FLAG_RESET;
	return set_attributes(attr);
}


int Drive::protect() const {
	drive_attr_t attr;
	attr.o_flags = DRIVE_FLAG_PROTECT;
	return set_attributes(attr);
}

int Drive::unprotect() const {
	drive_attr_t attr;
	attr.o_flags = DRIVE_FLAG_UNPROTECT;
	return set_attributes(attr);
}

int Drive::erase_blocks(u32 start, u32 end) const {
	drive_attr_t attr;
	attr.o_flags = DRIVE_FLAG_ERASE_BLOCKS;
	attr.start = start;
	attr.end = end;
	return set_attributes(attr);
}

int Drive::erase_device() const {
	drive_attr_t attr;
	DriveInfo info = get_info();
	if( info.o_flags() & DRIVE_FLAG_ERASE_DEVICE ){
		attr.o_flags = DRIVE_FLAG_ERASE_DEVICE;
		return set_attributes(attr);
	}

	u32 address = 0;
	while( address < info.write_block_count() ){
		if( erase_blocks(address, info.write_block_count()) < 0 ){
			return return_value();
		}
		address += return_value();
		while( is_busy() ){
			chrono::wait(
						info.erase_block_time()
						);
		}
	}
	return 0;
}

bool Drive::is_busy() const {
	int result = ioctl(IoRequest(I_DRIVE_ISBUSY));
	return result > 0;
}

DriveInfo Drive::get_info() const {
	drive_info_t info;
	if( ioctl(
			 IoRequest(I_DRIVE_GETINFO),
			 IoArgument(&info)
			 ) < 0 ){
		return DriveInfo();
	}
	return DriveInfo(info);
}

int Drive::set_attributes(const drive_attr_t & attributes) const {
	return ioctl(
				IoRequest(I_DRIVE_SETATTR),
				IoConstArgument(&attributes)
				);
}



