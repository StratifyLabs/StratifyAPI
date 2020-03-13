/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

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

	using BitsPerSample = arg::Argument< u16, struct WavBitsPerSampleTag > ;
	using ChannelCount = arg::Argument< u16, struct WavChannelCountTag > ;
	using SampleRate = arg::Argument< u16, struct WavSampleRateTag > ;
	using SampleCount = arg::Argument< u32, struct WavSampleCountTag > ;

	/*! \details Constructs a new WAV object and opens the WAV as a read-only file. */
	Wav(const var::String & path = var::String());

	int create(
			const var::String & path,
			IsOverwrite is_overwrite
			);

	void copy_header(
			const Wav & reference
			);

	void set_header(
			ChannelCount channel_count,
			SampleRate sample_rate,
			BitsPerSample bits_per_sample,
			SampleCount sample_count
			);


	u32 size() const { return m_header.size; }
	u32 wav_size() const { return m_header.format_size; }
	u32 wav_format() const { return m_header.wav_format; }
	/*! \cond */
	u32 channels() const { return m_header.channels; }
	/*! \endcond */
	u32 channel_count() const { return m_header.channels; }
	u32 sample_rate() const { return m_header.sample_rate; }
	u32 sample_count() const {
		return m_header.data_size /
				(m_header.channels * m_header.bits_per_sample / 8);
	}
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

	u32 m_sample_count;
	header_t m_header;
	/*! \endcond */

};

}

#endif /* SAPI_FMT_WAV_HPP_ */
