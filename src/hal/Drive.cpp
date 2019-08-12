#include "hal/Drive.hpp"

using namespace hal;

Drive::Drive(){

}

int Drive::initialize() const {
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
	attr.o_flags = DRIVE_FLAG_ERASE_DEVICE;
	return set_attributes(attr);
}

bool Drive::is_busy() const {
	int result = ioctl(arg::IoRequest(I_DRIVE_ISBUSY));
	return result > 0;
}

DriveInfo Drive::get_info() const {
	drive_info_t info;
	if( ioctl(
			 arg::IoRequest(I_DRIVE_GETINFO),
			 arg::IoArgument(&info)
			 ) < 0 ){
		return DriveInfo();
	}
	return DriveInfo(info);
}

int Drive::set_attributes(const drive_attr_t & attributes) const {
	return ioctl(
				arg::IoRequest(I_DRIVE_SETATTR),
				arg::IoConstArgument(&attributes)
				);
}


