/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
//Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#include <cstdlib>
#include <cstring>
#include "fmt/Bmp.hpp"
#include "sys/Appfs.hpp"

using namespace sgfx;
using namespace fmt;
using namespace sys;
using namespace fs;


Bmp::Bmp(){
	m_offset = 0;
}

Bmp::Bmp(const var::String & name){
	open_readonly(name);
}


int Bmp::open_readonly(const var::String & name){
	return open(
				name,
				fs::OpenFlags::read_only()
				);
}

int Bmp::open_readwrite(const var::String & name){
	return open(name, fs::OpenFlags::read_write());
}

int Bmp::open(
		const var::String & name,
		const fs::OpenFlags & flags
		){
	bmp_header_t hdr;

	m_dib.width = -1;
	m_dib.height = -1;
	m_dib.bits_per_pixel = 0;

	if( File::open(name, flags) < 0 ){
		return -1;
	}

	if( read(
				&hdr,
				Size(sizeof(hdr))
				) != sizeof(hdr) ){
		close();
		return -1;
	}

	if( read(
				&m_dib,
				Size(sizeof(m_dib))
				) != sizeof(m_dib) ){
		close();
		return -1;
	}

	if( seek(Location(hdr.offset), SET) != (int)hdr.offset ){
		m_dib.width = -1;
		m_dib.height = -1;
		m_dib.bits_per_pixel = 0;
		close();
		return -1;
	}

	m_offset = hdr.offset;
	return 0;
}

int Bmp::create(
		const var::String & path,
		Width width,
		Height height,
		PlaneCount planes,
		BitsPerPixel bits_per_pixel
		){

	bmp_header_t hdr;

	if( File::create(
				path,
				IsOverwrite(true)
				) < 0 ){
		return -1;
	}

	hdr.size = sizeof(hdr) +
			sizeof(m_dib) +
			(width.argument()*height.argument()*bits_per_pixel.argument() + 7) / 8;
	hdr.offset = sizeof(hdr) + sizeof(m_dib);
	hdr.signature = SIGNATURE;
	hdr.resd1 = 0;
	hdr.resd2 = 0;

	if( write(
				&hdr,
				Size(sizeof(hdr))
				) < 0 ){
		return -1;
	}

	m_dib.bits_per_pixel = bits_per_pixel.argument();
	m_dib.hdr_size = sizeof(m_dib);
	m_dib.height = height.argument();
	m_dib.width = width.argument();
	m_dib.planes = planes.argument();

	if( write(
				&m_dib,
				Size(sizeof(m_dib))
				) < 0 ){
		return -1;
	}

	return 0;
}

int Bmp::create_appfs(
		const var::String & name,
		s32 width,
		s32 height,
		u16 planes,
		u16 bits_per_pixel,
		char * img,
		u32 nbyte
		){
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

#if 0
	memcpy(img, &hdr, sizeof(hdr));
	memcpy(img + sizeof(hdr), &dib, sizeof(dib));

	DataReferenceFile source_data;

	source_data.data_reference().refer_to(
				SourceBuffer(img),
				Size(nbyte)
				);

	Appfs::create(
				FileName(name),
				SourceFile(source_data)
				);
#endif


	return 0;
}

unsigned int Bmp::calculate_row_size() const{
	return (((m_dib.bits_per_pixel/8)*m_dib.width + 3) / 4) * 4;
}

int Bmp::seek_row(s32 y) const {
	if( m_dib.height > 0 ){
		//image is upside down -- seek to beginning of row
		return seek(
					Location(m_offset + calc_row_size() * (m_dib.height - (y + 1))), SET
					);
	}

	return seek(Location(m_offset + calc_row_size() * y), SET);
}

int Bmp::read_pixel(uint8_t * pixel, u32 pixel_size, bool mono, uint8_t thres){
	u32 avg;
	u32 i;

	if( read(
				pixel,
				Size(pixel_size)
				) != (int)pixel_size ){
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

int Bmp::save(
		const var::String & path,
		const sgfx::Bitmap & bitmap,
		const sgfx::Palette & pallete
		){

	Bmp bmp;
	if( bmp.create(
				path,
				Width(bitmap.width()),
				Height(bitmap.height()),
				PlaneCount(1),
				BitsPerPixel(24)
				) < 0 ){
		return -1;
	}

	for(sg_int_t y = bitmap.height()-1; y >= 0; y--){
		var::Vector<u8> row(bmp.calculate_row_size());
		for(sg_int_t x = 0; x < bitmap.width(); x++){
			sg_color_t color = bitmap.get_pixel(sgfx::Point(x,y));
			sg_color_t color_rgb = pallete.colors().at(color);

			row.at(x*3) = color_rgb & 0xff;
			row.at(x*3+1) = color_rgb >> 8;
			row.at(x*3+2) = color_rgb >> 16;
		}

		if( bmp.write(row) < 0 ){
			return -1;
		}
	}
	bmp.close();

	return 0;
}

sgfx::Bitmap Bmp::convert_to_bitmap(
		sgfx::Bitmap::BitsPerPixel bpp
		){
	s32 i, j;
	int avg;
	u8 pixel[3];

	sgfx::Bitmap result(
				Area(width(), height()),
				bpp
				);

	result.clear();

	for(j=0; j < height(); j++){
		seek_row(j);

		for(i=0; i < width(); i++){
			read(pixel);
			avg = (pixel[0] + pixel[1] + pixel[2]) / 3;

			sg_color_t bitmap_color;
			bitmap_color = avg * (result.color_count()-1) / 255;
			if( (bitmap_color == 0) && (avg > 0) ){
				bitmap_color = 1;
			}
			//where is brightness in index
			result << Pen().set_color(bitmap_color);
			result.draw_pixel(Point(i,j));
		}
	}

	return result;
}



