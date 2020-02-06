/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved



#include "fmt/Wav.hpp"
using namespace fmt;
using namespace sys;

Wav::Wav(const var::String & path) {
	// TODO Auto-generated constructor stub
	if( !path.is_empty() &&
		 open(
			 path,
			 fs::OpenFlags::read_only()
			 ) >= 0 ){
		if( read(
				 &m_header,
				 Size(sizeof(m_header))
				 ) < 0 ){
			close();
		} else {
			return;
		}
	}
	memset(&m_header, 0, sizeof(m_header));
}


int Wav::create(
		const var::String & path,
		IsOverwrite is_overwrite
		){
	int result = File::create(
				path,
				is_overwrite
				);
	if( result < 0 ){ return result; }
	return write(
				&m_header,
				Size(sizeof(m_header))
				);
}

void Wav::copy_header(
		const Wav & reference
		){
	set_header(
				ChannelCount(reference.channel_count()),
				SampleRate(reference.sample_rate()),
				BitsPerSample(reference.bits_per_sample()),
				SampleCount(reference.sample_count())
				);
}


void Wav::set_header(
		ChannelCount channel_count,
		SampleRate sample_rate,
		BitsPerSample bits_per_sample,
		SampleCount sample_count
		){
	m_header.riff[0] = 'R';
	m_header.riff[1] = 'I';
	m_header.riff[2] = 'F';
	m_header.riff[3] = 'F';
	m_header.size = 0; //need to calculate later
	m_header.wave[0] = 'W';
	m_header.wave[1] = 'A';
	m_header.wave[2] = 'V';
	m_header.wave[3] = 'E';
	m_header.format_description[0] = 'f';
	m_header.format_description[1] = 'm';
	m_header.format_description[2] = 't';
	m_header.format_description[3] = ' ';
	m_header.format_size = 16;
	m_header.wav_format = 1; //1 means PCM other value indicate compression
	m_header.channels = channel_count.argument();
	m_header.sample_rate = sample_rate.argument();
	m_header.bytes_per_second =
			sample_rate.argument() *
			channel_count.argument() *
			bits_per_sample.argument() / 8;
	m_header.block_alignment =
			channel_count.argument() *
			bits_per_sample.argument()/8;
	m_header.bits_per_sample =
			bits_per_sample.argument();
	m_header.data_description[0] = 'd';
	m_header.data_description[1] = 'a';
	m_header.data_description[2] = 't';
	m_header.data_description[3] = 'a';
	m_header.data_size =
			sample_count.argument() *
			channel_count.argument() *
			bits_per_sample.argument() / 8;

	m_header.size = 36 + m_header.data_size;

}
