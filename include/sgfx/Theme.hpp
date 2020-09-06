/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#ifndef SAPI_SGFX_THEME_HPP
#define SAPI_SGFX_THEME_HPP

#include "../hal/Display.hpp"
#include "../sys/Printer.hpp"
#include "Bitmap.hpp"
#include "Palette.hpp"

namespace sgfx {

class Theme : public PaletteFlags {
public:
  using BitsPerPixel = Bitmap::BitsPerPixel;

  Theme(fs::File &color_file);

  enum styles {
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
    first_style = style_dark,
    last_style = style_outline_danger
  };

  enum states {
    state_default,
    state_highlighted,
    state_disabled,
    first_state = state_default,
    last_state = state_disabled
  };

  Theme &set_bits_per_pixel(u8 bits_per_pixel);
  u8 bits_per_pixel() const { return m_header.bits_per_pixel; }

  Theme &set_primary_font_name(const var::String &name) {
    m_primary_font_name = name;
    return *this;
  }

  Theme &set_primary_icon_font_name(const var::String &name) {
    m_primary_icon_font_name = name;
    return *this;
  }

  const var::String &primary_font_name() const { return m_primary_font_name; }

  const var::String &primary_icon_font_name() const {
    return m_primary_icon_font_name;
  }

  u16 color_count() const { return m_color_count; }

  int create(
    const var::String &path,
    fs::File::IsOverwrite is_overwrite,
    BitsPerPixel bits_per_pixel,
    enum pixel_format pixel_format);
  int load(const var::String &path);

  size_t calculate_color_offset(enum styles style, enum states state) const;

  int write_palette(
    enum styles style,
    enum states state,
    const Palette &palette);

  Palette read_palette(enum styles style, enum states state) const;

  int set_display_palette(
    const hal::Display &display,
    enum styles style,
    enum states state) const;

  static var::String get_state_name(enum states value);
  static var::String get_style_name(enum styles value);

  const chrono::MicroTime &button_hold_duration() const {
    return m_button_hold_duration;
  }

  sg_color_t background_color() const { return 0; }
  sg_color_t color() const { return 1; }
  sg_color_t text_color() const;
  sg_color_t border_color() const;

  AntiAliasFilter &antialias_filter() { return m_antialias_filter; }

  const AntiAliasFilter &antialias_filter() const { return m_antialias_filter; }

private:
  typedef struct MCU_PACK {
    u16 version;
    u8 bits_per_pixel;
    u8 pixel_format;
  } header_t;

  // holds the RGB values for all styles and states

  fs::File &m_color_file;
  AntiAliasFilter m_antialias_filter;
  var::String m_primary_font_name;
  var::String m_primary_icon_font_name;
  chrono::MicroTime m_button_hold_duration = chrono::Milliseconds(750);

  header_t m_header = {0};
  u16 m_color_count = 0;

  size_t header_color_count() const { return 1 << (m_header.bits_per_pixel); }
};

} // namespace sgfx

#endif // SAPI_SGFX_THEME_HPP
