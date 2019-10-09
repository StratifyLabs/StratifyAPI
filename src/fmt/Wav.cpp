//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved



#include "fmt/Wav.hpp"
using namespace fmt;
using namespace sys;

Wav::Wav(const arg::SourceFilePath & name) {
	// TODO Auto-generated constructor stub
	if( !name.argument().is_empty() && open(
			 arg::FilePath(name.argument()),
			 fs::OpenFlags::read_only()) >= 0 ){
		if( read(
				 arg::DestinationBuffer(&m_header),
				 arg::Size(sizeof(m_header))
				 ) < 0 ){
			close();
		} else {
			return;
		}
	}
	memset(&m_header, 0, sizeof(m_header));
}


int Wav::create(
		const arg::DestinationFilePath & path,
		arg::IsOverwrite is_overwrite
		){
	int result = File::create(
				arg::DestinationFilePath(path),
				is_overwrite
				);
	if( result < 0 ){ return result; }
	return write(
				arg::SourceBuffer(&m_header),
				arg::Size(sizeof(m_header))
				);
}

void Wav::copy_header(
		const Wav & reference
		){
	set_header(
				arg::ChannelCount(reference.channel_count()),
				arg::SampleRate(reference.sample_rate()),
				arg::BitsPerSample(reference.bits_per_sample()),
				arg::SampleCount(reference.sample_count())
				);
}


void Wav::set_header(
		arg::ChannelCount channel_count,
		arg::SampleRate sample_rate,
		arg::BitsPerSample bits_per_sample,
		arg::SampleCount sample_count
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
