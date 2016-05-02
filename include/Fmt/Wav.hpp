/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef WAV_HPP_
#define WAV_HPP_

#include <stdint.h>

#include "../Sys/File.hpp"

namespace Fmt {

/*! \brief WAV File format */
class Wav : public Sys::File {
public:
	Wav(const char * name);

	inline uint32_t size() const { return hdr_.size; }
	inline uint32_t wav_size() const { return hdr_.wav_size; }
	inline uint32_t wav_fmt() const { return hdr_.wav_fmt; }
	inline uint32_t channels() const { return hdr_.channels; }
	inline uint32_t sample_rate() const { return hdr_.sample_rate; }
	inline uint32_t bytes_sec() const { return hdr_.bytes_sec; }
	inline uint32_t bits_sample() const { return hdr_.bits_sample; }
	inline uint32_t data_size() const { return hdr_.data_size; }

private:
	typedef struct {
		char riff[4];
		uint32_t size;
		char wave[4];
		char fmt[4];
		uint32_t wav_size;
		uint16_t wav_fmt;
		uint16_t channels;
		uint32_t sample_rate;
		uint32_t bytes_sec;
		uint16_t block_align;
		uint16_t bits_sample;
		char data_desc[4];
		uint32_t data_size;
	} wav_hdr_t;

	wav_hdr_t hdr_;

};

};

#endif /* WAV_HPP_ */
