//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved



#include "fmt/Wav.hpp"
using namespace fmt;
using namespace sys;

Wav::Wav(const var::ConstString & name) {
	// TODO Auto-generated constructor stub
	if( !name.is_empty() && open(name, File::READONLY) >= 0 ){
		if( read(&m_header, sizeof(m_header)) < 0 ){
			close();
		} else {
			return;
		}
	}
	memset(&m_header, 0, sizeof(m_header));
}


int Wav::create(const var::ConstString & path){
	int result = File::create(path);
	if( result < 0 ){ return result; }
	return write(&m_header, sizeof(m_header));
}

void Wav::set_header(u16 channels,
							u32 sample_rate,
							u16 bits_per_sample,
							u32 samples){
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
	m_header.channels = channels;
	m_header.sample_rate = sample_rate;
	m_header.bytes_per_second = sample_rate * channels * bits_per_sample / 8;
	m_header.block_alignment = channels*bits_per_sample/8;
	m_header.bits_per_sample = bits_per_sample;
	m_header.data_description[0] = 'd';
	m_header.data_description[1] = 'a';
	m_header.data_description[2] = 't';
	m_header.data_description[3] = 'a';
	m_header.data_size = samples * channels * bits_per_sample / 8;

	m_header.size = 36 + m_header.data_size;

}
