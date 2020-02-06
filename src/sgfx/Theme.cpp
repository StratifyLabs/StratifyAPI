/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "sgfx/Theme.hpp"
#include "var/Json.hpp"
#include "fs/File.hpp"
#include "sys/Printer.hpp"

#define VERSION 0x0308

using namespace sgfx;
using namespace hal;

Theme::Theme(fs::File & color_file) : m_color_file(color_file){
}

size_t Theme::calculate_color_offset(enum style style, enum state state) const {
   return (style * m_color_count * (last_state + 1) + (state * m_color_count))*2 + sizeof(header_t);
}

Theme& Theme::set_bits_per_pixel(u8 bits_per_pixel){
   m_header.bits_per_pixel = bits_per_pixel;
   m_color_count = header_color_count();
   return *this;
}

int Theme::write_palette(
      enum style style,
      enum state state,
      const var::Vector<u16> colors
      ){
   if( colors.count() != m_color_count ){
      return -1;
   }

   return m_color_file.write(
            fs::File::Location(calculate_color_offset(style,state)),
            colors
            );
}

var::Vector<u16> Theme::read_palette(
      enum style style,
      enum state state
      ) const {
   var::Vector<u16> result(m_color_count);

   int offset = calculate_color_offset(style,state);
   if( m_color_file.read(
            fs::File::Location(offset),
            result
            ) < 0 ){
      return var::Vector<u16>();
   }

   return result;
}


int Theme::load(const var::String & path){

   if( m_color_file.open(path, fs::OpenFlags::read_only()) < 0 ){
      return -1;
   }

   if( m_color_file.read(m_header) < 0 ){
      return -1;
   }

   m_color_count = header_color_count();

   return 0;
}

int Theme::create(
      const var::String & path,
      fs::File::IsOverwrite is_overwrite,
      BitsPerPixel bits_per_pixel,
      PixelFormat pixel_format
      ){

   if( m_color_file.create(path, is_overwrite) < 0 ){
      return -1;
   }

   m_header.version = VERSION;
   m_header.bits_per_pixel = bits_per_pixel.argument();
   m_header.pixel_format = pixel_format.argument();
   m_color_count = 0;

   if( m_color_file.write(m_header) < 0 ){
      return -1;
   }

   m_color_count = header_color_count();


   return 0;
}

int Theme::set_display_palette(const Display & display,
      enum style style,
      enum state state
      ) const {

   var::Vector<u16> colors = read_palette(style, state);
   display_palette_t display_palette;

   display_palette.pixel_format = m_header.pixel_format;
   display_palette.colors = static_cast<void*>(colors.data());
   display_palette.count = colors.count();
   return display.set_palette(display_palette);
}

