/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef WAV_HPP_
#define WAV_HPP_

#include <mcu/types.h>
#include "../api/FmtObject.hpp"
#include "../var/ConstString.hpp"

namespace fmt {

/*! \brief WAV File format */
class Wav : public api::FmtFileObject {
public:
	/*! \details Constructs a new WAV object and open the WAV as a read-only file. */
	Wav(const var::ConstString & name);

	u32 size() const { return m_hdr.size; }
	u32 wav_size() const { return m_hdr.wav_size; }
	u32 wav_fmt() const { return m_hdr.wav_fmt; }
	u32 channels() const { return m_hdr.channels; }
	u32 sample_rate() const { return m_hdr.sample_rate; }
	u32 bytes_sec() const { return m_hdr.bytes_sec; }
	u32 bits_sample() const { return m_hdr.bits_sample; }
	u32 data_size() const { return m_hdr.data_size; }

private:
	/*! \cond */
	typedef struct {
		char riff[4];
		u32 size;
		char wave[4];
		char fmt[4];
		u32 wav_size;
		u16 wav_fmt;
		u16 channels;
		u32 sample_rate;
		u32 bytes_sec;
		u16 block_align;
		u16 bits_sample;
		char data_desc[4];
		u32 data_size;
	} wav_hdr_t;

	wav_hdr_t m_hdr;
	/*! \endcond */

};

}

#endif /* WAV_HPP_ */
