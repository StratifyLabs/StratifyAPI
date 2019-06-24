#include "hal/Drive.hpp"

using namespace hal;

Drive::Drive(){

}

int Drive::initialize() const {
	drive_attr_t attr;
	attr.o_flags = DRIVE_FLAG_INIT;
	return ioctl(I_DRIVE_SETATTR, &attr);
}


int Drive::powerup() const {
	drive_attr_t attr;
	attr.o_flags = DRIVE_FLAG_POWERUP;
	return ioctl(I_DRIVE_SETATTR, &attr);
}

int Drive::powerdown() const {
	drive_attr_t attr;
	attr.o_flags = DRIVE_FLAG_POWERDOWN;
	return ioctl(I_DRIVE_SETATTR, &attr);
}

int Drive::erase_blocks(u32 start, u32 end) const {
	drive_attr_t attr;
	attr.o_flags = DRIVE_FLAG_ERASE_BLOCKS;
	attr.start = start;
	attr.end = end;
	return ioctl(I_DRIVE_SETATTR, &attr);
}

int Drive::erase_device() const {
	drive_attr_t attr;
	attr.o_flags = DRIVE_FLAG_ERASE_DEVICE;
	return ioctl(I_DRIVE_SETATTR, &attr);
}

bool Drive::is_busy() const {
	int result = ioctl(I_DRIVE_ISBUSY);
	return result > 0;
}

DriveInfo Drive::get_info(){
	drive_info_t info;
	if( ioctl(I_DRIVE_GETINFO, &info) < 0 ){
		return DriveInfo();
	}
	return DriveInfo(info);
}


