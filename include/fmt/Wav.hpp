/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef WAV_HPP_
#define WAV_HPP_

#include <mcu/types.h>

#include <sys/File.hpp>

namespace fmt {

/*! \brief WAV File format */
class Wav : public sys::File {
public:
	Wav(const char * name);

	inline u32 size() const { return m_hdr.size; }
	inline u32 wav_size() const { return m_hdr.wav_size; }
	inline u32 wav_fmt() const { return m_hdr.wav_fmt; }
	inline u32 channels() const { return m_hdr.channels; }
	inline u32 sample_rate() const { return m_hdr.sample_rate; }
	inline u32 bytes_sec() const { return m_hdr.bytes_sec; }
	inline u32 bits_sample() const { return m_hdr.bits_sample; }
	inline u32 data_size() const { return m_hdr.data_size; }

private:
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

};

};

#endif /* WAV_HPP_ */
