#include "hal/Drive.hpp"

using namespace hal;

Drive::Drive(){

}

int Drive::initialize(
		const arg::SourceFilePath path
		){

	if( fileno() < 0 ){
		if( open(
				 arg::FilePath(path.argument()),
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


