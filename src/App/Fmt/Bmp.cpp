//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include <fmt/Bmp.hpp>
#include <sys/Appfs.hpp>
using namespace fmt;
using namespace sys;


Bmp::Bmp(){
	m_offset = 0;
}

Bmp::Bmp(const char * name){
	bmp_header_t hdr;

	m_dib.width = -1;
	m_dib.height = -1;
	m_dib.bits_per_pixel = 0;

	if( this->open(name, RDWR) < 0 ){
		return;
	}

	if( read(&hdr, sizeof(hdr)) != sizeof(hdr) ){
		close();
		return;
	}

	if( read(&m_dib, sizeof(m_dib)) != sizeof(m_dib) ){
		close();
		return;
	}

	if( seek(hdr.offset) != (int)hdr.offset ){
		m_dib.width = -1;
		m_dib.height = -1;
		m_dib.bits_per_pixel = 0;
		close();
		return;
	}

	m_offset = hdr.offset;
}

int Bmp::create(const char * name, i32 width, i32 height, u16 planes, u16 bits_per_pixel){

	bmp_header_t hdr;

	if( File::create(name, true) < 0 ){
		return -1;
	}

	hdr.size = sizeof(hdr) + sizeof(m_dib) + (width*height*bits_per_pixel + 7) / 8;
	hdr.offset = sizeof(hdr) + sizeof(m_dib);
	hdr.signature = SIGNATURE;
	hdr.resd1 = 0;
	hdr.resd2 = 0;

	if( write(&hdr, sizeof(hdr)) < 0 ){
		return -1;
	}

	m_dib.bits_per_pixel = bits_per_pixel;
	m_dib.hdr_size = sizeof(m_dib);
	m_dib.height = height;
	m_dib.width = width;
	m_dib.planes = planes;

	if( write(&m_dib, sizeof(m_dib)) < 0 ){
		return -1;
	}

	return 0;
}

int Bmp::create_appfs(const char * name, i32 width, i32 height, u16 planes, u16 bits_per_pixel, char * img, size_t nbyte){
	bmp_header_t hdr;
	bmp_dib_t dib;

	hdr.size = sizeof(hdr) + sizeof(dib) + (width*height*bits_per_pixel + 7) / 8;
	hdr.offset = sizeof(hdr) + sizeof(dib);
	hdr.signature = SIGNATURE;
	hdr.resd1 = 0;
	hdr.resd2 = 0;

	dib.bits_per_pixel = bits_per_pixel;
	dib.hdr_size = sizeof(dib);
	dib.height = height;
	dib.width = width;
	dib.planes = planes;

	memcpy(img, &hdr, sizeof(hdr));
	memcpy(img + sizeof(hdr), &dib, sizeof(dib));


	Appfs::create(name, img, nbyte);



	return 0;
}

unsigned int Bmp::calc_row_size() const{
	return (((m_dib.bits_per_pixel/8)*m_dib.width + 3) / 4) * 4;
}

int Bmp::seek_row(i32 y){
	if( m_dib.height > 0 ){
		//image is upside down -- seek to beginning of row
		return seek(m_offset + calc_row_size() * (m_dib.height - (y + 1)));
	}

	return seek(m_offset + calc_row_size() * y);
}

int Bmp::read_pixel(uint8_t * pixel, ssize_t pixel_size, bool mono, uint8_t thres){
	u32 avg;
	ssize_t i;

	if( read(pixel, pixel_size) != pixel_size ){
		return -1;
	}

	if( mono ){
		avg = 0;
		for(i=0; i < pixel_size; i++){
			avg += pixel[i];
		}
		avg = avg / 8;
		if( avg > thres ){
			return 1;
		} else {
			return 0;
		}
	}

	return 0;
}



