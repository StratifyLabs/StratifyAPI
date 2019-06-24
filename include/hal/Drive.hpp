#ifndef SAPI_HAL_DRIVE_HPP_
#define SAPI_HAL_DRIVE_HPP_

#include <sos/dev/drive.h>
#include "Device.hpp"
#include "../chrono/MicroTime.hpp"

namespace hal {

class DriveInfo : public api::HalInfoObject {
public:
	DriveInfo(){ memset(&m_info, 0, sizeof(m_info)); }
	DriveInfo(const drive_info_t & info){
		m_info = info;
	}

	bool is_valid() const {
		return m_info.num_write_blocks != 0;
	}

	u64 size() const {
		return m_info.num_write_blocks * m_info.write_block_size;
	}

	u32 o_flags() const { return m_info.o_flags; }
	u32 o_events() const { return m_info.o_events; }
	u16 address_size() const { return m_info.address_size; }
	u32 write_block_size() const { return m_info.write_block_size; }
	u32 write_block_count() const { return m_info.num_write_blocks; }
	u32 frequency() const { return m_info.bitrate; }

	chrono::MicroTime erase_device_time() const {
		return chrono::MicroTime(m_info.erase_device_time);
	}

	chrono::MicroTime erase_block_time() const {
		return chrono::MicroTime(m_info.erase_block_time);
	}


private:
	drive_info_t m_info;
};


class Drive : public Device {
public:
	Drive();


	int initialize() const;

	int powerup() const;
	int powerdown() const;
	int erase_blocks(u32 start, u32 end) const;
	int erase_device() const;
	bool is_busy() const;


	DriveInfo get_info();


};

}

#endif // SAPI_HAL_DRIVE_HPP_
