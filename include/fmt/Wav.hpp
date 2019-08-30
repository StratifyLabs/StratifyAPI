/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_FMT_WAV_HPP_
#define SAPI_FMT_WAV_HPP_

#include <mcu/types.h>
#include "../api/FmtObject.hpp"
#include "../var/ConstString.hpp"
#include "../arg/Argument.hpp"

namespace fmt {

/*! \brief WAV File format */
class Wav : public fs::File {
public:
	/*! \details Constructs a new WAV object and open the WAV as a read-only file. */
	Wav(const arg::SourceFilePath & name = arg::SourceFilePath(""));

	int create(const arg::DestinationFilePath & path);

	void set_header(u16 channels, u32 sample_rate, u16 bits_per_sample, u32 samples);


	u32 size() const { return m_header.size; }
	u32 wav_size() const { return m_header.format_size; }
	u32 wav_format() const { return m_header.wav_format; }
	u32 channels() const { return m_header.channels; }
	u32 sample_rate() const { return m_header.sample_rate; }
	u32 bytes_per_second() const { return m_header.bytes_per_second; }
	u32 bits_per_sample() const { return m_header.bits_per_sample; }
	u32 data_size() const { return m_header.data_size; }

	const void * header() const{ return &m_header; }
	u32 header_size() const { return sizeof(header_t); }

private:
	/*! \cond */
	typedef struct {
		char riff[4];
		u32 size;
		char wave[4];
		char format_description[4];
		u32 format_size;
		u16 wav_format;
		u16 channels;
		u32 sample_rate;
		u32 bytes_per_second;
		u16 block_alignment;
		u16 bits_per_sample;
		char data_description[4];
		u32 data_size;
	} header_t;

	header_t m_header;
	/*! \endcond */

};

}

#endif /* SAPI_FMT_WAV_HPP_ */
