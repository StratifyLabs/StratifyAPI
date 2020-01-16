#ifndef SAPI_SGFX_THEME_HPP
#define SAPI_SGFX_THEME_HPP


#include "../hal/DisplayDevice.hpp"

namespace sgfx {

class Theme {
public:

   using BitsPerPixel = Bitmap::BitsPerPixel;
   using PixelFormat = arg::Argument<u8, struct ThemePixelFormatTag>;

   Theme(fs::File & color_file);

   enum style {
      style_dark,
      style_light,
      style_brand_primary,
      style_brand_secondary,
      style_info,
      style_success,
      style_warning,
      style_danger,
      style_outline_dark,
      style_outline_light,
      style_outline_brand_primary,
      style_outline_brand_secondary,
      style_outline_info,
      style_outline_success,
      style_outline_warning,
      style_outline_danger,
      last_style = style_outline_danger
   };

   enum state {
      state_default,
      state_highlighted,
      state_disabled,
      last_state = state_disabled
   };

   Theme& set_bits_per_pixel(u8 bits_per_pixel);
   u8 bits_per_pixel() const {
      return m_header.bits_per_pixel;
   }

   u16 color_count() const {
      return m_color_count;
   }

   int create(
         const var::String & path,
         fs::File::IsOverwrite is_overwrite,
         BitsPerPixel bits_per_pixel,
         PixelFormat pixel_format
         );
   int load(const var::String & path);


   size_t calculate_color_offset(
         enum style style,
         enum state state
         ) const;


   int write_palette(
         enum style style,
         enum state state,
         const var::Vector<u16> colors
         );

   var::Vector<u16> read_palette(
         enum style style, enum state state
         ) const;

   int set_display_palette(
         const hal::Display & display,
         enum style style,
         enum state state
         ) const;

   AntiAliasFilter & antialias_filter(){
      return m_antialias_filter;
   }


   const AntiAliasFilter & antialias_filter() const {
      return m_antialias_filter;
   }

private:
   typedef struct MCU_PACK {
      u16 version;
      u8 bits_per_pixel;
      u8 pixel_format;
   } header_t;

   //holds the RGB values for all styles and states

   fs::File & m_color_file;
   AntiAliasFilter m_antialias_filter;

   header_t m_header;
   u16 m_color_count = 0;

   size_t header_color_count() const {
      return 1 << (m_header.bits_per_pixel);
   }
};

}

#endif // SAPI_SGFX_THEME_HPP
